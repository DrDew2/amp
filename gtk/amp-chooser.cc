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

#include "amp-chooser.h"
#include "amp-library-manager.h"
extern AmpLibraryManager* lib_manager;

AmpChooser::AmpChooser (const Glib::ustring& title, Gtk::FileChooserAction action=Gtk::FILE_CHOOSER_ACTION_OPEN) {
	chooser = new Gtk::FileChooserDialog (title, action);
	chooser->set_title(title);
	chooser->set_action (action);
	preview = new Gtk::Image();
	icon_size = Gtk::IconSize::register_new ("icon_size_DIALOG", 150, 150);
	
	//Preparing the chooser for use
	chooser->set_local_only(true);
	chooser->set_use_preview_label(false);
	chooser->set_preview_widget(*preview);

	//Giving it functionality
	chooser->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	chooser->add_button(Gtk::Stock::APPLY, Gtk::RESPONSE_ACCEPT);
	chooser->set_current_folder(Glib::get_home_dir ());
	chooser->set_select_multiple(false);
	chooser->signal_update_preview().connect(sigc::mem_fun(*this,
			&AmpChooser::update_preview_cb));

	chooser->set_show_hidden (false);
	chooser->set_default_size(300, 500);
	chooser->set_size_request(300, 500);
	chooser->resize(300, 500);
}

AmpChooser::~AmpChooser () {
}

int AmpChooser::run() {
	return chooser->run();
}
void AmpChooser::hide() {
	chooser->hide();
}
void AmpChooser::add_filter (const Glib::RefPtr<Gtk::FileFilter>& filter) {
	chooser->add_filter (filter);
}

Glib::ustring AmpChooser::get_filename() {
	return chooser->get_filename();
}
void AmpChooser::update_preview_cb () {
	bool have_preview = false;
	Glib::ustring   filename = chooser->get_preview_filename ();
	Gtk::Image*     preview = (Gtk::Image *) chooser->get_preview_widget ();	

	Glib::RefPtr<Gdk::PixbufAnimation> anim;
	Glib::RefPtr<Gdk::Pixbuf> pixbuf;
	Glib::RefPtr<Gio::File>   file = Gio::File::create_for_path (filename);
	
	Gio::FileType filetype = file->query_file_type (Gio::FILE_QUERY_INFO_NONE);

	switch (filetype) {
		case Gio::FILE_TYPE_NOT_KNOWN:
			lib_manager->debug ("The filetype is unknown");
			pixbuf = chooser->render_icon_pixbuf (Gtk::Stock::DIALOG_QUESTION, icon_size);
			break;
		case Gio::FILE_TYPE_REGULAR:
			if (lib_manager->is_image_file (filename)) {
				anim = Gdk::PixbufAnimation::create_from_file (filename);
				if (anim->is_static_image()) {
					try {
						pixbuf = Gdk::Pixbuf::create_from_file (filename, 150, 150, true);
					} catch (Gdk::PixbufError ex) {
						Gtk::MessageDialog* err_dialog = new Gtk::MessageDialog ("Error", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
						switch (ex.code ()) {
							case  Gdk::PixbufError::CORRUPT_IMAGE:
								preview->set (filename);
								have_preview = true;
								break;
							case Gdk::PixbufError::FAILED:
								err_dialog->set_message ("Creating Image Failed");
								err_dialog->set_secondary_text ("Please choose another image.");
								break;
							case Gdk::PixbufError::INSUFFICIENT_MEMORY:
								err_dialog->set_message ("Not enough memory");
								break;
							case Gdk::PixbufError::BAD_OPTION:
								err_dialog->set_message ("Bad Option");
								err_dialog->set_secondary_text ("Please choose another image.");
								break;
							case Gdk::PixbufError::UNKNOWN_TYPE:
								err_dialog->set_message ("The type of this image is Unknown");
								err_dialog->set_secondary_text ("Please choose another image.");
								break;
							default:
								err_dialog->set_message ("Error");
								err_dialog->set_secondary_text ("Please choose another image.");
								break;
						}
						if (!have_preview) {
							err_dialog->run ();
							err_dialog->hide ();
						}
						delete err_dialog;
					}
				} else {
						preview->set (anim);
						have_preview = true;
				}
				break;
			} else if (lib_manager->is_music_file (filename)) {
				pixbuf = chooser->render_icon_pixbuf (Gtk::Stock::CDROM, icon_size);
				break;
			} else {
				pixbuf = chooser->render_icon_pixbuf (Gtk::Stock::FILE, icon_size);
				break;
			}
		case Gio::FILE_TYPE_DIRECTORY:
			pixbuf = chooser->render_icon_pixbuf (Gtk::Stock::DIRECTORY, icon_size);
			break;
		case Gio::FILE_TYPE_MOUNTABLE:
			pixbuf = chooser->render_icon_pixbuf (Gtk::Stock::HARDDISK, icon_size);
			break;
		default:
			pixbuf = chooser->render_icon_pixbuf (Gtk::Stock::DIALOG_WARNING, icon_size);
			break;
	}
	if (!have_preview) {
		preview->set (pixbuf);
	}

	have_preview = true;

	chooser->set_preview_widget_active (have_preview);
	return;
}

void AmpChooser::set_running (bool running) {
	is_running = running;
}

bool AmpChooser::get_running () {
	return is_running;
}
