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

#ifndef _AMP_BUTTON_H_
#define _AMP_BUTTON_H_

#include <gtkmm/button.h>
#include <gtkmm/hvbox.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/stockid.h>
#include <gtkmm/stock.h>
#include <glibmm/ustring.h>
#include <gtkmm/hvseparator.h>

class AmpButton: public Gtk::Button 
{
	public:
		AmpButton ();
		AmpButton(const Gtk::StockID& stock_id, const Glib::ustring& text);
		virtual ~AmpButton();
		void set_icon(const Gtk::StockID& icon);
		void set_text (const Glib::ustring& text);
		Glib::ustring get_text ();
		
	private:
		Gtk::Image      *icon;
		Gtk::Label *label;
		Gtk::IconSize *size;
};

#endif // _AMP_BUTTON_H_
