/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * amp
 * Copyright (C) The Amp Team 2012 <luisito.gonzalez72@gmail.com>
 * 
 * amp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * amp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "amp-viewer.h"
#include <vector>
#include <iostream>
#include <gdkmm/pixbufanimation.h>
#include <glibmm/ustring.h>

#define MAX_ZOOM_FACTOR                 20
#define MIN_ZOOM_FACTOR                 0.02

//Critical Stuff
AmpViewer::AmpViewer () {
	_interp_type_in = Cairo::FILTER_BILINEAR;
	_interp_type_out = Cairo::FILTER_BILINEAR;
	_current_image = 0;
	_transp_color = 0;
	_zoom_multiplier = 1.05;
	_zoom = 1.0;
	_upscale = false;
	_scroll_wheel_zoom = false;
	_transp_style = Amp::Viewer::TRANSP_COLOR;
	_zoom_mode = Amp::Viewer::ZOOM_MODE_SHRINK_TO_FIT;
//	_image = new Gtk::Image ();
	_alloc = new Gtk::Allocation ();

	_hadj = get_hadjustment();
	_hadj->configure(0, 100, 0, 10, 10, 100);
	_hadj->signal_value_changed().connect(sigc::mem_fun(*this, 
	                                                    &AmpViewer::adjustment_changed_cb));
	_hbar = get_hscrollbar();
	_hbar->set_adjustment(_hadj);

	_vadj = get_vadjustment();
	_vadj->configure(0, 100, 0, 10, 10, 100);
	_vadj->signal_value_changed().connect(sigc::mem_fun(*this,
	                                                    &AmpViewer::adjustment_changed_cb));
	_vbar = get_vscrollbar();
	_vbar->set_adjustment(_vadj);

//	_display = new Gtk::DrawingArea();
	_display.set_can_focus(true);
	_display.add_events(Gdk::EXPOSURE_MASK
	                     | Gdk::BUTTON_PRESS_MASK
	                     | Gdk::BUTTON_RELEASE_MASK
	                     | Gdk::SCROLL_MASK
	                     | Gdk::KEY_PRESS_MASK);
	_display.signal_configure_event().connect (sigc::mem_fun(*this,
	                                              &AmpViewer::on_size_changed));
	_display.signal_draw().connect (sigc::mem_fun(*this,
	                                               &AmpViewer::on_draw));
	_display.signal_map_event().connect (sigc::mem_fun(*this,
	                                                 &AmpViewer::on_map_event));
	_display.signal_button_press_event().connect (sigc::mem_fun(*this,
	                                        &AmpViewer::on_button_press_event));
	_display.signal_button_release_event().connect (sigc::mem_fun(*this,
	                                      &AmpViewer::on_button_release_event));
	_display.signal_scroll_event().connect (sigc::mem_fun(*this,
	                                              &AmpViewer::on_scroll_event));
	_display.signal_key_press_event().connect (sigc::mem_fun(*this,
	                                           &AmpViewer::on_key_press_event));
	add (_display);
}

AmpViewer::~AmpViewer () {
}

bool AmpViewer::is_zoomed_in () {
	return _zoom -1.0 > 1e-6;
}

bool AmpViewer::is_zoomed_out () {
	return 1e-6 + _zoom - 1.0 < 0.0;
}

bool AmpViewer::is_image_moveable () {
	return _hbar->get_visible() || _vbar->get_visible();
}

void AmpViewer::set_image (Glib::ustring file) {
	//_edata = exif_data_new_from_file (file.c_str());
	_file = Gio::File::create_for_path(file);
	_anim = Gdk::PixbufAnimation::create_from_file(file);
	if (_anim->is_static_image()) {
		try {
			_pixbuf = _anim->get_static_image();
		} catch (Glib::Error ex) {
			std::cout << ex.what() << std::endl;
			_pixbuf = Gdk::Pixbuf::create_from_file(file);
		}
	} else {
		_pixbuf = Gdk::Pixbuf::create_from_file(file);
	}
	_surface = create_surface_from_pixbuf ();
}

void AmpViewer::set_files (std::vector<Glib::ustring> files) {
	if (files.size() != 0) {
		_files.swap(files);
		set_image (_files[_current_image]);
	}
}

void AmpViewer::next_image() {
	if (_files.size() > 1) {
		if (_current_image >= _files.size() - 1) {
			_current_image = 0;
		} else {
			_current_image++;
		}
		set_image (_files[_current_image]);
	}
}

void AmpViewer::previous_image(){
	if (_files.size() > 1) {
		if (_current_image == 0) {
			_current_image = _files.size() - 1;
		} else {
			_current_image--;
		}
		set_image(_files[_current_image]);
	}
}

void AmpViewer::set_focus (bool focus) {
	if (focus) {
		_display.grab_focus();
	}
}

Glib::ustring AmpViewer::get_path() {
	if (_file) {
		return _file->get_path();
	} else {
		return "None";
	}
}

Glib::ustring AmpViewer::get_uri() {
	if (_file) {
		return _file->get_uri();
	} else {
		return "None";
	}
}

Glib::ustring AmpViewer::get_basename() {
	if (_file) {
		return _file->get_basename();
	} else {
		return "None";
	}
}

Glib::RefPtr<Gdk::Pixbuf> AmpViewer::get_pixbuf () {
	return _pixbuf;
}

//<---------------- Don't pay attention to anything under here ---------------->

//Scrolling Stuff
bool AmpViewer::scroll_event (GdkEventScroll* event) {
	double zoom_factor;
	int xofs, yofs;

	xofs = _hadj->get_page_increment() / 2;
	yofs = _vadj->get_page_increment() / 2;
	switch (event->direction) {
		case GDK_SCROLL_UP:
			zoom_factor = _zoom_multiplier;
			xofs = 0;
			yofs = -yofs;
			break;
		case GDK_SCROLL_LEFT:
			zoom_factor = 1.0 / _zoom_multiplier;
			xofs = -xofs;
			yofs = 0;
			break;
		case GDK_SCROLL_DOWN:
			zoom_factor = 1.0 / _zoom_multiplier;
			xofs = 0;
			yofs = yofs;
			break;
		case GDK_SCROLL_RIGHT:
			zoom_factor = _zoom_multiplier;
			xofs = xofs;
			yofs = 0;
			break;
		default:
			return false;
	}
	if (_scroll_wheel_zoom) {
		if (event->state & GDK_SHIFT_MASK) {
			this->scroll_by(yofs, xofs);
		} else if (event->state & GDK_CONTROL_MASK) {
			this->scroll_by(xofs, yofs);
		} else {
			this->set_zoom (_zoom * zoom_factor,TRUE, event->x, event->y);
		}
	} else {
		if (event->state & GDK_SHIFT_MASK) {
			this->scroll_by (yofs, xofs);
		} else if (event->state & GDK_CONTROL_MASK) {
			this->set_zoom (_zoom * zoom_factor, true, event->x, event->y);
		} else {
			this->scroll_by (xofs, yofs);
		}
	}
	return true;
}

void AmpViewer::scroll_by(int xofs, int yofs) {
	scroll_to (_xofs+xofs, _yofs + yofs, true);
}

void AmpViewer::scroll_to (int x, int y, bool change_adjustments) {
	Gtk::Allocation allocation;
	int xofs, yofs;
	Glib::RefPtr<Gdk::Window> window;

	if (_hbar->get_visible()) {
		x = CLAMP (x, 0, _hadj->get_upper() - _hadj->get_page_size());
		xofs = x - _xofs;
	} else {
		xofs = 0;
	}

	if (_vbar->get_visible()) {
		y = CLAMP (y, 0, _vadj->get_upper() - _vadj->get_page_size());
		yofs = y - _yofs;
	} else {
		yofs = 0;
	}

	if (xofs == 0 && yofs == 0) {
		return;
	}

	_xofs = x;
	_yofs = y;

	if (!_display.get_is_drawable()) {
		goto out;
	}

	allocation = _display.get_allocation();

	if (abs (xofs) >= allocation.get_width() || abs (yofs) >= allocation.get_height()) {
		_display.queue_draw();
		goto out;
	}

	window = _display.get_window();

	window->scroll(-xofs, -yofs);
	window->process_updates(true);

	out:
		if (!change_adjustments) {
			return;
		}
		_hadj->set_value(x);
		_vadj->set_value(y);
}

void AmpViewer::adjustment_changed_cb() {
	scroll_to(_hadj->get_value(), _vadj->get_value(), false);
}

//Zoom Stuff
void AmpViewer::set_zoom_mode (Amp::Viewer::ZoomMode mode) {
	if (_zoom_mode == mode) {
		return;
	}

	if (mode == Amp::Viewer::ZOOM_MODE_SHRINK_TO_FIT) {
		zoom_fit ();
	} else {
		_zoom_mode = mode;
	}
}

void AmpViewer::zoom_fit() {
	set_zoom_fit ();
	check_scrollbar_visibility (NULL);
	_display.queue_draw();
}

void AmpViewer::set_zoom_fit () {
	Gtk::Allocation allocation;
	double new_zoom;

	_zoom_mode = Amp::Viewer::ZOOM_MODE_SHRINK_TO_FIT;

	if (this->get_mapped()) {
		return;
	}

	if (!_pixbuf) {
		return;
	}

	allocation = _display.get_allocation();

	new_zoom = zoom_fit_scale (allocation.get_width(), allocation.get_height(),
	                           _pixbuf->get_width(),
	                           _pixbuf->get_height(), _upscale);

	if (new_zoom > MAX_ZOOM_FACTOR) {
		new_zoom = MAX_ZOOM_FACTOR;
	} else if (new_zoom < MIN_ZOOM_FACTOR) {
		new_zoom = MIN_ZOOM_FACTOR;
	}

	_zoom = new_zoom;
	_xofs = 0;
	_yofs = 0;

	//g_signal_emit (view, view_signals [SIGNAL_ZOOM_CHANGED], 0, _zoom);
}

void AmpViewer::zoom_fit_size (unsigned int dest_width, unsigned int dest_height,
                               unsigned int src_width, unsigned int src_height,
                               bool upscale_smaller, unsigned int *width,
                               unsigned int *height)
{
	unsigned int w, h;
	if (width != NULL || height != NULL) {
		return;
	}
	if (src_width == 0 || src_height == 0) {
		*width = 0;
		*height = 0;
	}
	if (src_width <= dest_width && src_height <= dest_height && !upscale_smaller) {
		*width = src_width;
		*height = src_height;
		return;
	}
	w = dest_width;
	h = floor ((double) (src_height * w) / src_width + 0.5);
	if (h > dest_height) {
		h = dest_height;
		w = floor ((double) (src_width * h) / src_height + 0.5);
	}
	*width = w;
	*height = h;
}

double AmpViewer::zoom_fit_scale (unsigned int dest_width, unsigned int dest_height,
                                  unsigned int src_width, unsigned int src_height,
                                  bool upscale_smaller)
{
	unsigned int w, h;
	double wfactor, hfactor;
	double factor;
	if (src_width == 0 || src_height == 0) {
		return 1.0;
	}
	if (dest_width == 0 || dest_height == 0) {
		return 0.0;
	}
	zoom_fit_size (dest_width, dest_height, src_width, src_height, upscale_smaller, &w, &h);

	wfactor = (double) w / src_width;
	hfactor = (double) h / src_height;

	factor = std::min (wfactor, hfactor);

	return factor;
}

//Transparency Stuff
void AmpViewer::get_transparency_params(int* size, unsigned int* color1, unsigned int* color2) {
	Gdk::RGBA color;
	switch (_transp_style) {
		case Amp::Viewer::TRANSP_BG: {
			color = this->get_style_context()->get_background_color(Gtk::STATE_FLAG_NORMAL);
			*color1 = *color2 = (((color.get_red_u() & 0xff00) << 8)
			                     | (color.get_green_u() & 0xff00)
			                     | ((color.get_blue_u() & 0xff00) >> 8));
			break;
		}
		case Amp::Viewer::TRANSP_CHECKED:
			*color1 = 0x00808080;
			*color2 = 0x00cccccc;
			break;
		case Amp::Viewer::TRANSP_COLOR:
			*color1 = *color2 = _transp_color;
			break;
		default:
			break;
	}
	*size = 8;
}
Cairo::RefPtr<Cairo::Surface> AmpViewer::create_background_surface() {
	int check_size;
	unsigned int check_1 = 0;
	unsigned int check_2 = 0;
	Cairo::RefPtr<Cairo::Surface> surface = Cairo::Surface::create(this->get_window()->create_similar_surface(Cairo::CONTENT_COLOR, check_size * 2, check_size * 2), Cairo::CONTENT_COLOR, check_size * 2, check_size * 2);
	get_transparency_params(&check_size, &check_1, &check_2);
	Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);
	cr->set_source_rgba(((check_1 & 0xff0000) >> 16) / 255.,
	                    ((check_1 & 0x00ff00) >> 8) / 255.,
	                    (check_1 & 0x0000ff)/255., 1.);
	cr->rectangle(0, 0, check_size, check_size);
	cr->rectangle(check_size, check_size, check_size, check_size);
	cr->fill();
	cr->set_source_rgba(((check_1 & 0xff0000) >> 16) / 255.0,
	                    ((check_2 & 0x00ff00) >> 8) / 255.0,
	                    (check_2 & 0x0000ff)/255.0, 1.0);
	cr->rectangle(0, check_size, check_size, check_size);
	cr->rectangle(check_size, 0, check_size, check_size);
	cr->fill();
	return surface;
}

Cairo::RefPtr<Cairo::Surface> AmpViewer::create_surface_from_pixbuf() {
	Cairo::RefPtr<Cairo::Surface> surface;
	Cairo::RefPtr<Cairo::Context> cr;

	surface = _display.get_window()->create_similar_surface(Cairo::CONTENT_COLOR_ALPHA,
															 _pixbuf->get_width(),
															 _pixbuf->get_height());

	cr = Cairo::Context::create(surface);

	Gdk::Cairo::set_source_pixbuf(cr, _pixbuf, 0, 0);
	cr->paint();

	return surface;
}
//Events
bool AmpViewer::on_size_changed (GdkEventConfigure* event) {
	if (_zoom_mode == Amp::Viewer::ZOOM_MODE_SHRINK_TO_FIT) {
		Gtk::Allocation alloc;
		alloc.set_width (event->width);
		alloc.set_height (event->height);
		set_zoom_fit();
		check_scrollbar_visibility (&alloc);
		_display.queue_draw();
	} else {
		int scaled_width, scaled_height;
		int x_offset = 0, y_offset=0;
		compute_scaled_size(_zoom, &scaled_width, &scaled_height);
		if (_xofs + event->width > scaled_width) {
			x_offset = scaled_width - event->width - _xofs;
		}
		if (_yofs + event->height > scaled_height) {
			y_offset = scaled_height - event->height - _yofs;
		}
		scroll_by(x_offset, y_offset);
	}
	update_scrollbar_values();
}
bool AmpViewer::on_draw (const Cairo::RefPtr<Cairo::Context>& cr) {
	Gtk::Allocation allocation = get_allocation();
  	const int width = allocation.get_width();
  	const int height = allocation.get_height();
	int xofs, yofs;
	int scaled_width, scaled_height;

	get_image_coords (&xofs, &yofs, &scaled_width, &scaled_height);
	
	//Paint the Background
	cr->set_source (this->get_window()->get_background_pattern());
	cr->rectangle (0,0, width, height);
	cr->rectangle (MAX (0, xofs),MAX (0, yofs), scaled_width, scaled_height);
	cr->set_fill_rule (Cairo::FILL_RULE_EVEN_ODD);
	cr->fill();

	if (_pixbuf) {
		Cairo::RefPtr<Cairo::SurfacePattern> pattern;
		pattern = Cairo::SurfacePattern::create (cr->get_target ());
		if (_pixbuf->get_has_alpha ()) {
			//Transparency Layer
			if (!_background_surface) {
				_background_surface = create_background_surface ();
			}
			cr->set_source (_background_surface, xofs, yofs);
			pattern->set_extend(Cairo::EXTEND_REPEAT);
			cr->set_source (pattern);
			cr->rectangle (xofs, yofs, scaled_width, scaled_height);
			cr->fill();
		}
		cr->rectangle (xofs, yofs, scaled_width, scaled_height);
		cr->clip();
		cr->scale (_zoom, _zoom); //Exciting!
		cr->set_source (_surface, xofs/_zoom, yofs/_zoom);
		pattern->set_extend(Cairo::EXTEND_PAD);

		if (is_zoomed_in ()) {
			pattern->set_filter(_interp_type_in);
		} else if (is_zoomed_out ()) {
			pattern->set_filter(_interp_type_out);
		}
		cr->set_source (pattern);
		
		Gdk::Cairo::set_source_pixbuf(cr, _pixbuf, (width - _pixbuf->get_width())/2, 
										(height - _pixbuf->get_height())/2);
	}
	cr->paint();

	return true;
}
bool AmpViewer::on_map_event (GdkEventAny* event) {
	set_zoom_fit ();
	check_scrollbar_visibility (NULL);
	_display.queue_draw();
}
bool AmpViewer::on_button_press_event (GdkEventButton* event) {
	if (_dragging) {
		return false;
	}
	
	if (!_display.has_focus()) {
		_display.grab_focus();
	}
	
	switch (event->button) {
		case 1:
		case 2:
			if (event->button == 1 && !_scroll_wheel_zoom && 
			    !(event->state & GDK_CONTROL_MASK)) {
					break;
				}
			if (this->is_image_moveable ()) {
				//eog_scroll_view_set_cursor (view, EOG_SCROLL_VIEW_CURSOR_DRAG);

				_dragging = true;
				_drag_anchor_x = event->x;
				_drag_anchor_y = event->y;

				_drag_ofs_x = _xofs;
				_drag_ofs_y = _yofs;

				return true;
			}
		default:
			break;
	}

	return false;
}
bool AmpViewer::on_button_release_event (GdkEventButton* event) {
	if (!_dragging)
		return false;

	switch (event->button) {
		case 1:
		case 2:
			drag_to (event->x, event->y);
			_dragging = false;

			//eog_scroll_view_set_cursor (view, EOG_SCROLL_VIEW_CURSOR_NORMAL);
			break;
		default:
			break;
	}

	return true;
}
bool AmpViewer::on_scroll_event (GdkEventScroll* event) {
	double zoom_factor;
	int xofs, yofs;

	/* Compute zoom factor and scrolling offsets; we'll only use either of them */
	xofs = _hadj->get_page_increment() / 2;
	yofs = _vadj->get_page_increment() / 2;

	switch (event->direction) {
		case GDK_SCROLL_UP:
			zoom_factor = _zoom_multiplier;
			xofs = 0;
			yofs = -yofs;
			break;
		case GDK_SCROLL_LEFT:
			zoom_factor = 1.0 / _zoom_multiplier;
			xofs = -xofs;
			yofs = 0;
			break;
		case GDK_SCROLL_DOWN:
			zoom_factor = 1.0 / _zoom_multiplier;
			xofs = 0;
			yofs = yofs;
			break;
		case GDK_SCROLL_RIGHT:
			zoom_factor = _zoom_multiplier;
			xofs = xofs;
			yofs = 0;
			break;
		default:
			return false;
	}

	if (_scroll_wheel_zoom) {
		if (event->state & GDK_SHIFT_MASK) {
			scroll_by (yofs, xofs);
		} else if (event->state & GDK_CONTROL_MASK) {
			scroll_by (xofs, yofs);
		} else {
			set_zoom (_zoom * zoom_factor, true, event->x, event->y); 
		}
	} else {
		if (event->state & GDK_SHIFT_MASK) {
			scroll_by (yofs, xofs);
		} else if (event->state & GDK_CONTROL_MASK) {
			set_zoom (_zoom * zoom_factor, true, event->x, event->y);
		} else {
			scroll_by (xofs, yofs);
		}
	}

	return true;
}
bool AmpViewer::on_key_press_event (GdkEventKey* event) {
	Gtk::Allocation allocation = get_allocation();
	bool do_zoom=false;
	bool do_scroll=false;
	double zoom=1.0;
	int xofs=0, yofs=0;

	if (event->state & GDK_MOD1_MASK) return false;
	if (!_display.has_focus()) _display.grab_focus();

	switch (event->keyval) {
		case GDK_KEY_Up:
			std::cout << "you pressed up" << std::endl;
			do_scroll = true;
			xofs = 0;
			yofs = -32;
			break;

		case GDK_KEY_Page_Up:
			std::cout << "you pressed page up" << std::endl;
			do_scroll = true;
			if (event->state & GDK_CONTROL_MASK) {
				xofs = -(allocation.get_width() * 3) / 4;
				yofs = 0;
			} else {
				xofs = 0;
				yofs = -(allocation.get_height() * 3) / 4;
			}
			break;

		case GDK_KEY_Down:
			std::cout << "you pressed down" << std::endl;
			do_scroll = true;
			xofs = 0;
			yofs = 32;
			break;

		case GDK_KEY_Page_Down:
			std::cout << "you pressed page down" << std::endl;
			do_scroll = true;
			if (event->state & GDK_CONTROL_MASK) {
				xofs = (allocation.get_width() * 3) / 4;
				yofs = 0;
			} else {
				xofs = 0;
				yofs = (allocation.get_height() * 3) / 4;
			}
			break;

		case GDK_KEY_Left:
			std::cout << "you pressed left" << std::endl;
			do_scroll = true;
			xofs = -32;
			yofs = 0;
			break;

		case GDK_KEY_Right:
			do_scroll = true;
			xofs = 32;
			yofs = 0;
			break;

		case GDK_KEY_plus:
		case GDK_KEY_equal:
		case GDK_KEY_KP_Add:
			do_zoom = true;
			zoom = _zoom * _zoom_multiplier;
			break;

		case GDK_KEY_minus:
		case GDK_KEY_KP_Subtract:
			do_zoom = true;
			zoom = _zoom / _zoom_multiplier;
			break;

		case GDK_KEY_1:
			do_zoom = true;
			zoom = 1.0;
			break;

		default:
			std::cout << "You pressed a button" << std::endl;
			break;
	}

	if (do_zoom) {
		Glib::RefPtr< Gdk::DeviceManager > device_manager;
		Glib::RefPtr<Gdk::Device> device;
		int x, y;

		device_manager = _display.get_display()->get_device_manager();
		device = device_manager->get_client_pointer();

		device->get_position(x, y);

		set_zoom (zoom, true, x, y);
	}

	if (do_scroll) scroll_by (xofs, yofs);

	return true;
}

void AmpViewer::compute_scaled_size (double zoom, int *width, int *height) {
	if (_pixbuf) {
		*width = floor (_pixbuf->get_width() * zoom + 0.5);
		*height = floor (_pixbuf->get_height() * zoom + 0.5);
	} else {
		*width = *height = 0;
	}
}
void AmpViewer::check_scrollbar_visibility (Gtk::Allocation *alloc) {
	int bar_height;
	int bar_width;
	int img_width;
	int img_height;
	Gtk::Requisition req;
	int width, height;
	bool hbar_visible, vbar_visible;
	if (alloc) {
		width = alloc->get_width();
		height = alloc->get_height();
	} else {
		Gtk::Allocation allocation = get_allocation();
		width = allocation.get_width();
		height = allocation.get_height();
	}
	compute_scaled_size (_zoom, &img_width, &img_height);
	_hbar->get_preferred_size (req, req);
	bar_height = req.height;
	_vbar->get_preferred_size (req, req);
	bar_width = req.width;
	hbar_visible = vbar_visible = false;
	if (_zoom_mode == Amp::Viewer::ZOOM_MODE_SHRINK_TO_FIT) {
		hbar_visible = vbar_visible = false;
	} else if (img_width <= width && img_height <= height) {
		hbar_visible = vbar_visible = false;
	} else if (img_width > width && img_height > height) {
		hbar_visible = vbar_visible = true;
	} else if (img_width > width) {
		hbar_visible = true;
		if (img_height <= (height - bar_height)) {
			vbar_visible = false;
		} else {
			vbar_visible = true;
		}
	} else if (img_height > height) {
		vbar_visible = true;
		if (img_width <= (width - bar_width)) {
			hbar_visible = false;
		} else {
			hbar_visible = true;
		}
	}
	if (hbar_visible != _hbar->get_visible ()) { 
		_hbar->set_visible(hbar_visible);
	}
	if (vbar_visible != _vbar->get_visible ()) {
		_vbar->set_visible(vbar_visible);
	}
}
void AmpViewer::update_scrollbar_values () {
	int scaled_width, scaled_height;
	double page_size, page_increment, step_increment;
	double lower, upper;
	Gtk::Allocation allocation;
	if (_hbar->get_visible() && !_vbar->get_visible()) {
		return;
	}
	compute_scaled_size (_zoom, &scaled_width, &scaled_height);
	allocation = _display.get_allocation();
	if (_hbar->get_visible ()) {
		/* Set scroll increments */
		page_size = MIN (scaled_width, allocation.get_width());
		page_increment = allocation.get_width() / 2;
		step_increment = 32;
		/* Set scroll bounds and new offsets */
		lower = 0;
		upper = scaled_width;
		_xofs = CLAMP (_xofs, 0, upper - page_size);
		_hadj->configure (_xofs, lower, upper, step_increment, page_increment, 
		                  page_size);
	}
	if (_vbar->get_visible ()) {
		page_size = MIN (scaled_height, allocation.get_height());
		page_increment = allocation.get_height() / 2;
		step_increment = 32;
		lower = 0;
		upper = scaled_height;
		_yofs = CLAMP (_yofs, 0, upper - page_size);
		_vadj->configure (_yofs, lower, upper, step_increment, page_increment, 
		                  page_size);
	}
}

bool AmpViewer::get_image_coords (int*x, int* y, int* width, int* height) {
	Gtk::Allocation allocation;
	int scaled_width, scaled_height, xofs, yofs;

	compute_scaled_size (_zoom, &scaled_width, &scaled_height);

	if (width) {
		*width = scaled_width;
	}
	if (height) {
		*height = scaled_height;
	}

	/* If only width and height are needed stop here. */
	if (x == NULL && y == NULL) {
		return true;
	}

	allocation = _display.get_allocation();

	/* Compute image offsets with respect to the window */
	if (scaled_width <= allocation.get_width()) {
		xofs = (allocation.get_width() - scaled_width) / 2;
	} else { 
		xofs = -_xofs;
	}

	if (scaled_height <= allocation.get_height()) {
		yofs = (allocation.get_height() - scaled_height) / 2;
	} else {
		yofs = -_yofs;
	}

	if (x) {
		*x = xofs;
	}
	if (y) {
		*y = yofs;
	}

	return true;
}

void AmpViewer::drag_to(int x, int y) {
	int dx, dy;

	dx = _drag_anchor_x - x;
	dy = _drag_anchor_y - y;

	x = _drag_ofs_x + dx;
	y = _drag_ofs_y + dy;

	scroll_to (x, y, true);
}

void AmpViewer::set_zoom (double zoom, bool have_anchor, int anchorx, int anchory) {
	Gtk::Allocation allocation;
	int xofs, yofs;
	double x_rel, y_rel;

	if (!_pixbuf) {
		return;
	}

	if (zoom > MAX_ZOOM_FACTOR) {
		zoom = MAX_ZOOM_FACTOR;
	} else if (zoom < MIN_ZOOM_FACTOR) {
		zoom = MIN_ZOOM_FACTOR;
	}
	if (_zoom == zoom) {
		return;
	}
	if (_zoom == _min_zoom && zoom < _zoom) {
		return;
	}

	//eog_scroll_view_set_zoom_mode (view, EOG_ZOOM_MODE_FREE);
	set_zoom_mode (Amp::Viewer::ZOOM_MODE_FREE);

	allocation = get_allocation();

	/* compute new xofs/yofs values */
	if (have_anchor) {
		x_rel = (double) anchorx / allocation.get_width();
		y_rel = (double) anchory / allocation.get_height();
	} else {
		x_rel = 0.5;
		y_rel = 0.5;
	}

	compute_center_zoom_offsets (_zoom, zoom,
				     allocation.get_width(), allocation.get_height(),
				     x_rel, y_rel,
				     &xofs, &yofs);

	/* set new values */
	_xofs = xofs; /* (img_width * x_rel * zoom) - anchorx; */
	_yofs = yofs; /* (img_height * y_rel * zoom) - anchory; */

	if (zoom <= _min_zoom) {
		_zoom = _min_zoom;
	} else {
		_zoom = zoom;
	}

	/* we make use of the new values here */
	check_scrollbar_visibility (NULL);
	update_scrollbar_values ();

	/* repaint the whole image */
	_display.queue_draw();

	//g_signal_emit (view, view_signals [SIGNAL_ZOOM_CHANGED], 0, _zoom);
}

void AmpViewer::compute_center_zoom_offsets (double old_zoom, double new_zoom,
                                             int width, int height,
		                             double zoom_x_anchor, double zoom_y_anchor,
                                             int *xofs, int *yofs) 
{
int old_scaled_width, old_scaled_height;
	int new_scaled_width, new_scaled_height;
	double view_cx, view_cy;

	compute_scaled_size (old_zoom, &old_scaled_width, &old_scaled_height);

	if (old_scaled_width < width) {
		view_cx = (zoom_x_anchor * old_scaled_width) / old_zoom;
	} else {
		view_cx = (_xofs + zoom_x_anchor * width) / old_zoom;
	}

	if (old_scaled_height < height) {
		view_cy = (zoom_y_anchor * old_scaled_height) / old_zoom;
	} else {
		view_cy = (_yofs + zoom_y_anchor * height) / old_zoom;
	}

	compute_scaled_size (new_zoom, &new_scaled_width, &new_scaled_height);

	if (new_scaled_width < width) {
		*xofs = 0;
	} else {
		*xofs = floor (view_cx * new_zoom - zoom_x_anchor * width + 0.5);
		if (*xofs < 0)
			*xofs = 0;
	}

	if (new_scaled_height < height)
		*yofs = 0;
	else {
		*yofs = floor (view_cy * new_zoom - zoom_y_anchor * height + 0.5);
		if (*yofs < 0)
			*yofs = 0;
	}
}
