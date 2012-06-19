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

#ifndef _AMP_STATUSBAR_H_
#define _AMP_STATUSBAR_H_

#include <gtkmm/statusbar.h>
#include <gtkmm/button.h>
#include <gtkmm/image.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/scale.h>
#include <gtkmm/stock.h>

class AmpStatusbar: public Gtk::Statusbar 
{
public:
	AmpStatusbar();
	virtual ~AmpStatusbar();

	void set_controls_sensitive (bool sensitive);
	bool get_controls_sensitive ();
	void set_value(int64_t value);
	bool get_toggled ();
	void set_toggled (bool toggled);

protected:
	Gtk::Button* prev, *next;
	Gtk::ToggleButton* _play;	
	Gtk::Image* play_icon;
	Gtk::Scale* progress;

	void play ();

private:

};

#endif // _AMP_STATUSBAR_H_
