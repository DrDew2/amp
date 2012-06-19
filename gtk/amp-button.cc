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

#include "amp-button.h"

AmpButton::AmpButton () {
	Gtk::HBox               *main_box = new Gtk::HBox (false, 4);
	Gtk::VSeparator *v_sep = new Gtk::VSeparator ();

	size = new Gtk::IconSize (5);
	icon = new Gtk::Image (Gtk::Stock::APPLY, *size);
	label = new Gtk::Label ("Hello\nWorld");
	label->set_justify (Gtk::JUSTIFY_LEFT);

	main_box->pack_start (*icon, false, false, 0);
	main_box->pack_start (*v_sep, false, false, 4);
	main_box->pack_start (*label, false, false, 0);

	this->add (*main_box);
}

AmpButton::AmpButton(const Gtk::StockID& stock_id=Gtk::Stock::APPLY, const Glib::ustring& text="Hello\nWorld") {
	Gtk::HBox*			main_box = new Gtk::HBox (false, 4);
	Gtk::VSeparator	v_sep;

	size = new Gtk::IconSize (5);
	icon = new Gtk::Image (Gtk::Stock::APPLY, *size);
	label = new Gtk::Label ("Hello\nWorld");
	icon->set(stock_id, *size);
	label->set_justify (Gtk::JUSTIFY_LEFT);
	label->set_label (text);

	main_box->pack_start (*icon, false, false, 0);
	main_box->pack_start (v_sep, false, false, 4);
	main_box->pack_start (*label, false, false, 0);

	this->add (*main_box);
}

AmpButton::~AmpButton() {
}

void AmpButton::set_icon(const Gtk::StockID& stock_id=Gtk::Stock::APPLY) {
	icon->set (stock_id, *size);
}

void AmpButton::set_text (const Glib::ustring& str) {
	label->set_label (str);
}

Glib::ustring AmpButton::get_text () {
	return label->get_label ();
}
