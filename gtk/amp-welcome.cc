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

#include "amp-welcome.h"

AmpWelcome::AmpWelcome ()
:       _welcome_lbl ("Hello\nWorld"),
_h_separator (Gtk::ORIENTATION_HORIZONTAL),
_v_button_box (Gtk::ORIENTATION_VERTICAL)
{
	_welcome_lbl.set_justify (Gtk::JUSTIFY_CENTER);

	pack_start (_welcome_lbl, false, false, 2);
	pack_start (_h_separator, false, false, 4);
	pack_start (_v_button_box, false, false, 0);
}

void AmpWelcome::add_button (Gtk::Widget& btn) {
	_v_button_box.add (btn);
}

void AmpWelcome::set_text (Glib::ustring text="Hello\nWorld") {
	_welcome_lbl.set_label (text);
}
