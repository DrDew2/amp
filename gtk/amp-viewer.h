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

#ifndef _AMP_VIEWER_H_
#define _AMP_VIEWER_H_
#include <gtkmm.h>
#include <gdkmm/devicemanager.h>
#include <math.h>
#if HAVE_EXIF
#include <libexif/exif-data.h>
#endif /*HAVE_EXIF*/
namespace Amp {
	namespace Viewer {
		typedef enum {
			TRANSP_BG,
			TRANSP_CHECKED,
			TRANSP_COLOR
		} TransparencyStyle;
		typedef enum {
			ZOOM_MODE_FREE,
			ZOOM_MODE_SHRINK_TO_FIT
		} ZoomMode;
	}
}

class AmpViewer: public Gtk::ScrolledWindow 
{
	public:
		AmpViewer ();
		virtual ~AmpViewer ();
		
		void set_image (Glib::ustring file);
		Glib::ustring get_path();
		Glib::ustring get_uri();
		Glib::ustring get_basename();
		Glib::RefPtr<Gdk::Pixbuf> get_pixbuf ();
		
		void set_scroll_wheel_zoom(bool zoom);
		void set_zoom_upscale(bool upscale);
		void set_zoom_multiplier(double multiplier);
		void set_zoom_mode (Amp::Viewer::ZoomMode mode);
		Amp::Viewer::ZoomMode get_zoom_mode ();
		void set_antialiasing_in (bool state);
		void set_antialiasing_out (bool state);
		void set_transparency_color (Gdk::Color color);
		void set_transparency (Amp::Viewer::TransparencyStyle style);
		bool scrollbars_visible();
		void set_popup (Gtk::Menu menu);
		void set_background_color (Gdk::Color color);
		void set_background_color (Gdk::RGBA color);
		void override_bg_color (Gdk::Color color);
		void override_bg_color (Gdk::RGBA color);
		void set_use_bg_color (bool use);
		void zoom_in(bool smooth);
		void zoom_out(bool smooth);
		void set_zoom (double zoom);
		double get_zoom ();
		bool get_zoom_is_min();
		bool get_zoom_is_max();
		void show_cursor ();
		void hide_cursor ();
		
		
		void next_image();
		void previous_image();
		void set_files(std::vector<Glib::ustring> files);
		bool is_zoomed_out ();
		bool is_zoomed_in ();
		bool is_image_moveable ();
		bool event_is_over_image (GdkEvent* event);

		//Zooming Stuff
		void set_zoom (double zoom, bool have_anchor, int anchorx, int anchory);
		void set_zoom_fit ();
		void zoom_fit ();
		void set_focus(bool focus);

		//Scrolling Stuff
		bool scroll_event (GdkEventScroll* event);
		void scroll_by (int xofs, int yofs);
		void scroll_to (int x, int y, bool change_adjustments);

	protected:
		//The Part to show
		Gtk::DrawingArea				_display;
		Glib::RefPtr<Gtk::Adjustment> 	_hadj, _vadj;
		Gtk::Scrollbar					*_hbar, *_vbar;
		Gtk::Menu						_menu;
		
		//The Actual Image
		Glib::RefPtr<Gio::File>		_file;
		bool is_monitored;
		Glib::RefPtr<Gdk::Pixbuf>	_image_pixbuf;


		//Image Necessities
		unsigned int 					_image_changed_id;
		unsigned int 					_frame_changed_id;
		Glib::RefPtr<Gdk::Pixbuf>       _pixbuf;		
		Cairo::RefPtr<Cairo::Surface> 	_surface;

		//Zooming stuff
		Amp::Viewer::ZoomMode			_zoom_mode;
		bool 							_upscale;
		double 							_zoom, _min_zoom;
		Cairo::Filter 					_interp_type_in;
		Cairo::Filter					_interp_type_out;

		//Scroll-Wheel-Zoom
		bool							_scroll_wheel_zoom;
		double 							_zoom_multiplier;

		//Scrolling Stuff
		int 	_xofs, _yofs;

		Glib::RefPtr<Gdk::PixbufAnimation> _anim;
		Gtk::Allocation*		        _alloc;
		unsigned int   					_width, _height;
		unsigned int 					_current_image;
		unsigned int					_transp_color;
		unsigned int					_idle_id;

		//Dragging Stuff
		int 							_drag_anchor_x, _drag_anchor_y;
		int 							_drag_ofs_x, _drag_ofs_y;
		unsigned int 					_dragging;
		
		std::vector<Glib::ustring> 		_files;
#if HAVE_EXIF
		ExifData						_exif;
#endif

		//Events
		void adjustment_changed_cb();
		bool on_size_changed (GdkEventConfigure* event);
		bool on_draw (const Cairo::RefPtr<Cairo::Context>& cr);
		//bool on_draw_2 (const Cairo::RefPtr<Cairo::Context>& cr);
		bool on_map_event (GdkEventAny* event);
		bool on_button_press_event (GdkEventButton* event);
		bool on_button_release_event (GdkEventButton* event);
		bool on_scroll_event (GdkEventScroll* event);
		bool on_key_press_event (GdkEventKey* event);
		
		void compute_scaled_size (double zoom, int *width, int *height);
		void check_scrollbar_visibility (Gtk::Allocation *alloc);
		void update_scrollbar_values();
	private:
		//Dragging Stuff
		void drag_to (int x, int y);
		
		//Transparency Stuff
		Amp::Viewer::TransparencyStyle			_transp_style;
		Cairo::RefPtr<Cairo::Surface> 	_background_surface;
		Cairo::RefPtr<Cairo::Surface> 	create_background_surface();
		Cairo::RefPtr<Cairo::Surface> 	create_surface_from_pixbuf();
		
		//Scrolling stuff
		bool get_image_coords (int*x, int* y, int* width, int* height);

		//Zooming Stuff
		void compute_center_zoom_offsets (double old_zoom, double new_zoom,
		                                  int width, int height, 
		                                  double zoom_x_anchor, double zoom_y_anchor,
		                                  int *xofs, int *yofs);
		void zoom_fit_size (unsigned int dest_width, unsigned int dest_height,
		                    unsigned int src_width, unsigned int src_height,
		                    bool upscale_smaller, unsigned int *width,
		                    unsigned int *height);
		double zoom_fit_scale (unsigned int dest_width, unsigned int dest_height,
		                       unsigned int src_width, unsigned int src_height,
		                       bool upscale_smaller);
		void get_transparency_params(int* size, unsigned int* color1, unsigned int* color2);
};

#endif // _AMP_VIEWER_H_
