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

#include "amp-toolbar.h"
#include "amp-gui-manager.h"
#include <iostream>

extern AmpGUIManager* guimanager;

AmpToolbar::AmpToolbar () {
	//Widgets
	main_tool_item	= new Gtk::ToolItem();
	prev_btn = new Gtk::ToolButton(Gtk::Stock::GO_BACK);
	next_btn = new Gtk::ToolButton(Gtk::Stock::GO_FORWARD);

	set_image_widgets ();
	set_media_widgets ();

	prev_btn->signal_clicked().connect (sigc::mem_fun (*this,
	                                                   &AmpToolbar::on_previous_clicked));

	//Toolbar
	set_toolbar_style (Gtk::TOOLBAR_ICONS);
	set_icon_size(Gtk::ICON_SIZE_SMALL_TOOLBAR);

	set_view_mode (HOME_VIEW_MODE);
}

AmpToolbar::~AmpToolbar() {
}

void AmpToolbar::set_view_mode (AmpToolbarViewMode mode) {
	_mode = mode;
	switch (mode) {
		case WEB_VIEW_MODE:
			//set_web_mode ();
			break;
		case HOME_VIEW_MODE:
			set_home_mode ();
			break;
		case IMAGE_VIEW_MODE:
			set_image_mode ();
			break;
		case MEDIA_VIEW_MODE:
			set_media_mode ();
			break;
		default:
			//set_media_mode ();
			break;
	}
}

AmpToolbarViewMode AmpToolbar::get_view_mode () {
	return _mode;
}

void AmpToolbar::set_home_mode () {
	hide ();        
}

void AmpToolbar::set_image_mode () {
	clear();
	
	Gtk::SeparatorToolItem* sep = new Gtk::SeparatorToolItem();	

	main_tool_item->remove();

	//main_box = Gtk::manage(new Gtk::Box ());
	//main_box->pack_start (*artwork, false, false, 12);

	main_tool_item->add (*artwork);
	main_tool_item->set_border_width (6);
	main_tool_item->set_expand(false);

	sep->set_expand(true);
	sep->set_draw(false);

	insert (*main_tool_item, 0);
	insert (*sep, 1);
	insert (*prev_btn, 2);
	insert (*next_btn, 3);

	for (int i=0; i<image_list.size(); i++) {
		insert (*image_list[i], i+4);
	}

	show_all();
}

void AmpToolbar::set_media_mode () {
	clear();
	
	insert (*prev_btn, 0);
	insert (*media_list[0], 1);
	insert (*next_btn, 2);
	
	for (int i=1; i < media_list.size(); i++) {
		insert(*media_list[i], i+2);
	}

	show_all();	
}

void AmpToolbar::set_image_widgets() {
	zoom_in_btn = new Gtk::ToolButton(Gtk::Stock::ZOOM_IN);
	zoom_out_btn = new Gtk::ToolButton(Gtk::Stock::ZOOM_OUT);
	zoom_one_btn = new Gtk::ToolButton(Gtk::Stock::ZOOM_100);
	zoom_fit_btn = new Gtk::ToolButton(Gtk::Stock::ZOOM_FIT);

	image_list.push_back (zoom_in_btn);
	image_list.push_back (zoom_out_btn);
	image_list.push_back (zoom_one_btn);
	image_list.push_back (zoom_fit_btn);
}

void AmpToolbar::set_media_widgets(){
	artwork = new Gtk::Image(Gtk::Stock::CDROM, Gtk::ICON_SIZE_LARGE_TOOLBAR);
	play_btn = new Gtk::ToggleToolButton(Gtk::Stock::MEDIA_PLAY);
	progress = new Gtk::Scale(Gtk::ORIENTATION_HORIZONTAL);
	progress_item = new Gtk::ToolItem();
	loop_btn = new Gtk::ToolButton(Gtk::Stock::REFRESH);
	shuffle_btn = new Gtk::ToolButton(Gtk::Stock::UNDO);
	v_item = new Gtk::ToolItem();
	v_btn = new Gtk::VolumeButton();
	v_item->add(*v_btn);
	

	progress->set_range(0.00, 100.00);
	progress->set_value_pos(Gtk::POS_LEFT);
	progress_item->add (*progress);
	progress_item->set_expand(true);

	media_list.push_back (play_btn); //0
	media_list.push_back (progress_item); //1
	media_list.push_back (loop_btn); //2
	media_list.push_back (shuffle_btn); //3
	media_list.push_back (v_item); //4
}

void AmpToolbar::clear() {
	std::vector<Gtk::Widget*> list = this->get_children();
	if (list.size() > 1) {
		for (int i=0;i<list.size(); i++) {
			this->remove (*list[i]);
		}
	}
}

void AmpToolbar::set_album_artwork (Glib::RefPtr<Gdk::Pixbuf>& pixbuf) {
	artwork->clear();
	artwork->set (pixbuf);
}

void AmpToolbar::set_album_artwork (Glib::ustring filename) {
	artwork->clear();
	artwork->set(Gdk::Pixbuf::create_from_file(filename, 35, 35, true));
}

void AmpToolbar::on_previous_clicked() {
	switch (get_view_mode()) {
		case WEB_VIEW_MODE:
			break;
		case HOME_VIEW_MODE:
			break;
		case IMAGE_VIEW_MODE:
			guimanager->next_image ();
			break;
		case MEDIA_VIEW_MODE:
			break;
		default:
			break;
	}
}
