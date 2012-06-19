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

#include "amp-about.h"

AmpAbout::AmpAbout()
:  _VBox(Gtk::ORIENTATION_VERTICAL),
_Button_Quit("Exit"),
_Button_Authors("Authors"),
_Button_License("License"),
_Button_Description("Description"),
_ScrolledWindow (),
_TextView ()
{
	_Icon = new Gtk::Image (Gtk::Stock::APPLY, Gtk::ICON_SIZE_DIALOG);

	_prg_name = "Program";
	_Version = "0.0.1";
	set_label_text ();
	_Lbl.set_justify (Gtk::JUSTIFY_LEFT);

	_Authors = "Authors:\nNo one has made this program";
	_license = "License:\nThis program has no license";
	_Description = "Description:\nThis Program has no description";

	Gtk::Box*                       _HBox = new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL);
	Gtk::Box*                       _VBox2 = new Gtk::Box(Gtk::ORIENTATION_VERTICAL);
	Gtk::VSeparator*        _Sep = new Gtk::VSeparator ();

	_TextBuffer = Gtk::TextBuffer::create ();

	_TextView.set_buffer (_TextBuffer);
	_TextView.set_sensitive (false);
	_TextView.set_wrap_mode (Gtk::WRAP_WORD_CHAR);

	_ScrolledWindow.set_policy (Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	_ScrolledWindow.add (_TextView);

	_VBox2->pack_start (_Lbl, Gtk::PACK_EXPAND_WIDGET);
	_VBox2->pack_start (_ScrolledWindow, Gtk::PACK_EXPAND_WIDGET);

	_HBox->pack_start (*_Icon, false, false, 7);
	_HBox->pack_start (*_Sep, false, false, 7);
	_HBox->pack_start (*_VBox2, Gtk::PACK_EXPAND_WIDGET);

	set_title("About Program");
	set_border_width(7);
	set_default_size(430, 200);
	set_has_resize_grip (false);
	set_modal (true);
	//set_resizable (false);

	_ButtonBox.pack_start(_Button_Description, Gtk::PACK_SHRINK);
	_ButtonBox.pack_start(_Button_Authors, Gtk::PACK_SHRINK);
	_ButtonBox.pack_start(_Button_License, Gtk::PACK_SHRINK);
	_ButtonBox.pack_start(_Button_Quit, Gtk::PACK_SHRINK);

	_ButtonBox.set_border_width(6);
	_ButtonBox.set_layout(Gtk::BUTTONBOX_END);

	_Button_Quit.signal_clicked().connect( sigc::mem_fun(*this,
	                                                     &AmpAbout::on_button_quit) );
	_Button_Authors.signal_clicked ().connect ( sigc::mem_fun (*this,
	                                                           &AmpAbout::on_button_authors));
	_Button_License.signal_clicked ().connect ( sigc::mem_fun (*this,
	                                                           &AmpAbout::on_button_license));
	_Button_Description.signal_clicked ().connect ( sigc::mem_fun (*this,
	                                                               &AmpAbout::on_button_desc ));


	_VBox.pack_start(*_HBox, Gtk::PACK_EXPAND_WIDGET);
	_VBox.pack_start(_ButtonBox, Gtk::PACK_SHRINK);

	add(_VBox);

	_TextBuffer->set_text (_Description);
	show_all();
}


AmpAbout::~AmpAbout() {
}

void AmpAbout::set_label_text () {
	_Lbl.set_label (_prg_name + "\n" + _Version);
	_Lbl.set_justify (Gtk::JUSTIFY_LEFT);
}
//callbacks
void AmpAbout::on_button_quit() {
	hide();
}

void AmpAbout::on_button_authors () {
	_TextBuffer->set_text (_Authors);
}

void AmpAbout::on_button_license () {
	_TextBuffer->set_text (_license);
}

void AmpAbout::on_button_desc () {
	_TextBuffer->set_text (_Description);
}

// all the properties
void AmpAbout::set_program_name (Glib::ustring name) {
	_prg_name = name;
	set_title ("About" + _prg_name);
	set_label_text ();
}

void AmpAbout::set_version (Glib::ustring version) {
	_Version = version;     
	set_label_text ();
}       

void AmpAbout::set_authors (Glib::ustring authors) {
	_Authors = "Authors:\n" + authors;
	_TextBuffer->set_text (_Authors);
}

void AmpAbout::set_license (Glib::ustring license) {
	_license = "License:\n" + license;
	_TextBuffer->set_text (_license);
}

void AmpAbout::set_icon (Glib::ustring filename) {
	Glib::RefPtr<Gio::File> file = Gio::File::create_for_path (filename);

	if (file->query_exists ()) {            //Gio::file::query exists test
		_Icon->clear ();
		_Icon->set (filename); //this could fail
	} else {
		set_icon (Gtk::Stock::APPLY);
	}
}

void AmpAbout::set_icon (Gtk::StockID stock_id = Gtk::Stock::APPLY) {
	_Icon->clear ();
	_Icon->set (stock_id, Gtk::ICON_SIZE_DIALOG);
}

void AmpAbout::set_description (Glib::ustring desc) {
	_Description = "Description:\n" + desc;
}

Glib::ustring AmpAbout::get_program_name () {
	return _prg_name;
}

Glib::ustring AmpAbout::get_version () {
	return _Version;
}
