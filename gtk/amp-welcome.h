/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * amp
 * Copyright (C) The Amp Team 2012 <luisito.gonzalez72@gmail.com>
 * 
amp is free software: you can redistribute it and/or modify it
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

#ifndef _AMP_WELCOME_H_
#define _AMP_WELCOME_H_
#include <gtkmm.h>

class AmpWelcome: public Gtk::VBox 
{
public:
        AmpWelcome ();
        void add_button (Gtk::Widget& btn);
        void set_text (Glib::ustring text);
        
protected:
        Gtk::Label              _welcome_lbl;
        Gtk::Separator  _h_separator;
        Gtk::ButtonBox  _v_button_box;
        
private:

};

#endif // _AMP_WELCOME_H_
