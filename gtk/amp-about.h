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

#ifndef _AMP_ABOUT_H_
#define _AMP_ABOUT_H_

#include <gtkmm/box.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/button.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/stockid.h>
#include <gtkmm/stock.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/textview.h>
#include <gtkmm/textbuffer.h>
#include <gtkmm/window.h>
#include <glibmm/ustring.h>
#include <gtkmm/hvseparator.h>

class AmpAbout: public Gtk::Window 
{
public:
        AmpAbout();
        virtual ~AmpAbout();
        void set_program_name (Glib::ustring name);
        void set_version (Glib::ustring version);
        void set_authors (Glib::ustring authors);
        void set_license (Glib::ustring license);       
        void set_description (Glib::ustring desc);
        void set_icon (Glib::ustring filename);
        void set_icon (Gtk::StockID stock_id);
        Glib::ustring get_program_name ();
        Glib::ustring get_version ();
        Glib::ustring get_authors ();
        Glib::ustring get_license ();

protected:
  //Signal handlers:
        void on_button_quit();
        void on_button_authors ();
        void on_button_license ();
        void on_button_desc ();

  //Child widgets:
        Gtk::Box        _VBox;
        Gtk::ButtonBox  _ButtonBox;
        
        Gtk::Image*     _Icon;
        Gtk::Label      _Lbl;
        Gtk::Button     _Button_Authors;
        Gtk::Button     _Button_License;
        Gtk::Button     _Button_Description;
        Gtk::Button     _Button_Quit;
        Gtk::ScrolledWindow             _ScrolledWindow;
        Gtk::TextView   _TextView;
        Glib::RefPtr <Gtk::TextBuffer> _TextBuffer;
        
private:
        Glib::ustring _prg_name;
        Glib::ustring _Version;
        Glib::ustring _Authors;
        Glib::ustring _license;
        Glib::ustring _Description;
        void set_label_text ();
};

#endif // _AMP_ABOUT_H_
