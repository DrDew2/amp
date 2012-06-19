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

#ifndef _AMP_CHOOSER_H_
#define _AMP_CHOOSER_H_

#include <gtkmm/filechooserdialog.h>
#include <gtkmm/filefilter.h>
#include <glibmm/ustring.h>
#include <gtkmm/image.h>

class AmpChooser
{
public:
	AmpChooser(const Glib::ustring& title, Gtk::FileChooserAction action);
	virtual ~AmpChooser();
	void set_running(bool running);
	bool get_running();
	void add_filter(const Glib::RefPtr<Gtk::FileFilter>& filter);
	Glib::ustring get_filename();
	void hide();

	int run();
	
protected:
	Gtk::FileChooserDialog* chooser;
	bool is_running;
	Gtk::Image*	preview;
	Gtk::IconSize icon_size;
	void update_preview_cb();

};

#endif // _AMP_CHOOSER_H_
