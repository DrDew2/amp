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

#ifndef _AMP_TOOLBAR_H_
#define _AMP_TOOLBAR_H_
#include <gtkmm.h>
#include <gtkmm/volumebutton.h>

#include "amp-viewer.h"

typedef enum {
	WEB_VIEW_MODE,
	HOME_VIEW_MODE,
	IMAGE_VIEW_MODE,
	MEDIA_VIEW_MODE
} AmpToolbarViewMode;

class AmpToolbar: public Gtk::Toolbar 
{
	public:
		AmpToolbar();
		virtual ~AmpToolbar ();

		void set_view_mode (AmpToolbarViewMode mode);
		AmpToolbarViewMode get_view_mode ();

		void set_name (Glib::ustring name);
		void set_album (Glib::ustring name);
		void set_genre (Glib::ustring name);
		void set_year (Glib::ustring name);
		void set_artist (Glib::ustring name);
		void set_length (double value);
		void set_bit_rate (float value);

		void set_album_artwork (Glib::RefPtr<Gdk::Pixbuf>& pixbuf);
		void set_album_artwork (Glib::ustring filename);

		void set_controls_sensitive (bool sensitive);
		bool get_controls_sensitive ();

	protected:
		Gtk::Box*	main_box;
		AmpToolbarViewMode _mode;
		//General Child Widgets
		Gtk::ToolItem*  		main_tool_item;
		Gtk::ToolButton* prev_btn;
		Gtk::ToolButton* next_btn;

		//Image Mode Widgets
		Gtk::ToolButton* zoom_in_btn;
		Gtk::ToolButton* zoom_out_btn;
		Gtk::ToolButton* zoom_one_btn;
		Gtk::ToolButton* zoom_fit_btn;

		//Media Mode Widgets
		Gtk::Image*				artwork;
		Gtk::ToggleToolButton* play_btn;
		Gtk::ToolItem* progress_item;
		Gtk::Scale* progress;
		Gtk::ToolButton* loop_btn;
		Gtk::ToolButton* shuffle_btn;
		Gtk::ToolItem* v_item;
		Gtk::VolumeButton* v_btn;

		//Vectors to control everything
		std::vector<Gtk::ToolItem*> image_list;
		std::vector<Gtk::ToolItem*> media_list;
		//std::vector<Gtk::Widget*> web_list;

		void set_image_widgets();
		void set_media_widgets();
		void clear();
		//void set_web_widgets();

		//Events
		void on_previous_clicked();

	private:

		//functions when the mode is changed
		void set_media_mode ();
		void set_web_mode ();
		void set_image_mode ();
		void set_home_mode ();
};

#endif // _AMP_TOOLBAR_H_
