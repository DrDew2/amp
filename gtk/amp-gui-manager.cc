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

#include "amp-gui-manager.h"
extern AmpLibraryManager* lib_manager;
extern AmpPlayer*	player;

AmpGUIManager::AmpGUIManager () {
	message_area = new Gtk::InfoBar();
	amp_main_box = Gtk::manage(new Gtk::VBox ());
	amp_tab_bar = new Gtk::HBox ();
	amp_menu_bar = new Gtk::MenuBar ();
	amp_video_area = new Gtk::DrawingArea ();
	
	amp_viewer.set_policy (Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	create_menu_bar ();

	//Working on the Welcome Screen
	amp_welcome.set_text ("Welcome to Amp\nPlease Select a Button");

	Gtk::Button*    home_tab = new Gtk::Button ("Home");
	Gtk::Button*    image_tab = new Gtk::Button ("Images");
	Gtk::Button*	video_tab = new Gtk::Button ("Video");
	AmpButton*      filesystem_btn = new AmpButton (Gtk::Stock::DIRECTORY,
	                                                "Import\nFrom Filesystem");
	AmpButton*      mp3_player_btn = new AmpButton (Gtk::Stock::GO_DOWN, 
	                                                "Import\nfrom iPod / MP3 Player");
	AmpButton*      internet_btn = new AmpButton (Gtk::Stock::CONNECT,
	                                              "Download\nfrom Internet");
	AmpButton*      ripper_btn = new AmpButton (Gtk::Stock::CDROM, 
	                                            "Rip\nfrom CD / DVD");
	mp3_player_btn->set_sensitive (false);
	internet_btn->set_sensitive (false);
	ripper_btn->set_sensitive (false);

	filesystem_btn->set_tooltip_text ("Find media files in you Hard Drive");
	mp3_player_btn->set_tooltip_text ("Feature not implemented yet...");
	internet_btn->set_tooltip_text ("Feature not implemented yet...");
	ripper_btn->set_tooltip_text ("Feature not implemented yet...");

	// Putting it all together
	amp_welcome.add_button (*filesystem_btn);
	amp_welcome.add_button (*mp3_player_btn);
	amp_welcome.add_button (*internet_btn);
	amp_welcome.add_button (*ripper_btn);

	amp_tab_bar->pack_start (*home_tab, true, true, 0);
	amp_tab_bar->pack_start (*image_tab, true, true, 0);
	amp_tab_bar->pack_start (*video_tab, true, true, 0);

	amp_main_box->pack_start (*amp_menu_bar, false, false, 0);
	amp_main_box->pack_start (*amp_tab_bar, false, false, 0);
	amp_main_box->pack_start (amp_toolbar, false, false, 0);//Everything goes under here
	
	amp_main_box->pack_end (amp_statusbar, false, false, 0);//Everything goes above here
	amp_main_box->pack_end (*amp_video_area, false, false, 0);
	amp_main_box->pack_end (amp_viewer, true, true, 0);
	amp_main_box->pack_end (amp_welcome, false, false, 0);

	add (*amp_main_box);

	// Working on the window
	set_title ("Amp");
	set_default_size (800, 360);
	set_has_resize_grip (false);
	add_events(Gdk::KEY_PRESS_MASK);
	try {
		set_icon_from_file ("/home/luis/amp/amp_128.svg");
	} catch (Glib::FileError er) {
		//lib_manager->error ("Icon File does not exist");
		lib_manager->error (er.what());
	}

	show_all ();
	amp_viewer.hide ();
	message_area->hide ();
	amp_toolbar.set_view_mode (HOME_VIEW_MODE);

	// Connecting Events & Signals
	signal_key_press_event().connect(sigc::mem_fun(*this,
			&AmpGUIManager::on_key_press_event));

	filesystem_btn->signal_clicked().connect(sigc::mem_fun(*this,
										  &AmpGUIManager::select_music_folder));

	home_tab->signal_clicked().connect(sigc::mem_fun(*this,
	                                      &AmpGUIManager::on_home_tab_clicked));

	image_tab->signal_clicked().connect(sigc::mem_fun(*this, 
	                                     &AmpGUIManager::on_image_tab_clicked));
	
	video_tab->signal_clicked().connect(sigc::mem_fun(*this,
										 &AmpGUIManager::on_video_tab_clicked));

	resize (800, 360);
	amp_statusbar.set_sensitive (false);
	lib_manager->debug("GUIManager created succesfully");
}

AmpGUIManager::~AmpGUIManager () {
	if (!has_focus()) {
		std::cout << "does not have focus" <<std::endl;
		grab_focus();
	}
	if (has_focus()) {		
		std::cout << "does have focus" <<std::endl;
	}
	delete player;
	lib_manager->debug ("GUIManager destroyed succesfully");
	delete lib_manager;
}

void AmpGUIManager::next_image() {
	amp_viewer.next_image();
}

void AmpGUIManager::previous_image (){
	amp_viewer.previous_image ();
}

void AmpGUIManager::set_controls_sensitive (bool sensitive) {
	amp_statusbar.set_sensitive (sensitive);
}

void AmpGUIManager::create_menu_bar () {
	//Menus
	Gtk::Menu* help_menu = new Gtk::Menu ();
	Gtk::Menu* tools_menu = new Gtk::Menu ();
	Gtk::Menu* edit_menu = new Gtk::Menu ();
	Gtk::Menu* media_menu = new Gtk::Menu ();

	//Media Menu
	Gtk::MenuItem*	media_menu_item = new Gtk::MenuItem("_Media", true);
	Gtk::MenuItem*	open_song_item =  new Gtk::MenuItem("_Open new Song", true);
	Gtk::MenuItem*  open_image_item = new Gtk::MenuItem("Open new Image", true);
	Gtk::MenuItem*	open_image_folder = new Gtk::MenuItem("Open Image Folder", true);
	media_menu->append (*open_image_item);
	media_menu->append (*open_image_folder);
	media_menu->append (*open_song_item);
	media_menu_item->set_submenu (*media_menu);

	
	open_image_item->signal_activate().connect (sigc::mem_fun(*this,
	                                        &AmpGUIManager::select_image_file));
		
	open_image_folder->signal_activate().connect(sigc::mem_fun(*this,
			&AmpGUIManager::select_image_folder));
	open_song_item->signal_activate().connect (sigc::mem_fun (*lib_manager,
			&AmpLibraryManager::count_music_files));

	//Edit Menu
	Gtk::MenuItem* edit_menu_item = new Gtk::MenuItem ("_Edit", true);
	Gtk::MenuItem* prefs_item = new Gtk::MenuItem ("_Preferences", true);
	edit_menu->append (*prefs_item);
	edit_menu_item->set_submenu (*edit_menu);
	
	//Tools Menu
	Gtk::MenuItem* tool_menu_item = new Gtk::MenuItem ("_Tools", true);
	Gtk::MenuItem* desktop_item = new Gtk::MenuItem ("Set as _Desktop Wallpaper",
			true);
	tools_menu->append (*desktop_item);
	tool_menu_item->set_submenu (*tools_menu);

	desktop_item->signal_activate().connect(sigc::mem_fun(*this,
		&AmpGUIManager::set_wallpaper));

	//Help Menu
	Gtk::MenuItem* help_menu_item = new Gtk::MenuItem ("_Help", true);
	Gtk::MenuItem* about_item = new Gtk::MenuItem ("_About", true);
	help_menu->append (*about_item);
	help_menu_item->set_submenu (*help_menu);

	about_item->signal_activate().connect(sigc::mem_fun (*this,
			&AmpGUIManager::on_about_clicked));

	//Putting all the Menu's on the MenuBar
	amp_menu_bar->append (*media_menu_item);
	amp_menu_bar->append (*edit_menu_item);
	amp_menu_bar->append (*tool_menu_item);
	amp_menu_bar->append (*help_menu_item);
}

void AmpGUIManager::set_wallpaper () {
	message_area = new Gtk::InfoBar();
	
	Glib::RefPtr<Gio::Settings> settings;
	Gtk::Image   image(Gtk::Stock::DIALOG_QUESTION, Gtk::ICON_SIZE_DIALOG);
	Gtk::Label	 label("");
	label.set_markup ("<b>The image " + amp_viewer.get_basename() + " has been set as Desktop Background."
				  "\nWould you like to modify its appearance?</b>");
	label.set_justify(Gtk::JUSTIFY_CENTER);
	
	Gtk::HBox*	 hbox = (Gtk::HBox *) message_area->get_content_area();
	
	settings = Gio::Settings::create ("org.gnome.desktop.background");
	settings->set_string ("picture-uri", amp_viewer.get_uri());
	
	message_area->add_button ("_Open Background Preferences", Gtk::RESPONSE_YES);
	message_area->add_button ("_Hide", Gtk::RESPONSE_NO);
	message_area->set_default_response (Gtk::RESPONSE_YES);
	
	message_area->set_message_type (Gtk::MESSAGE_QUESTION);
	
	hbox->pack_start (image, false, false, 0);
	image.set_alignment (0.5, 0);
	hbox->pack_start (label, true, true, 0);
	label.set_alignment (0, 0.5);

	//Gtk::Box* box = (Gtk::Box *) info_bar->get_content_area();
	
	//box->pack_start (*hbox, true, true, 0);
	message_area->signal_response ().connect (sigc::mem_fun (*this,
	                              &AmpGUIManager::wallpaper_info_bar_response));
	message_area->show();
	amp_main_box->pack_start(*message_area, false, false, 0);
	if (message_area->get_visible()) {
		std::cout << "is visible" << std::endl;
	}
}
//Callbacks
void AmpGUIManager::on_home_tab_clicked () {
	amp_viewer.hide ();
	amp_welcome.show ();
	grab_focus();
	amp_toolbar.set_view_mode (HOME_VIEW_MODE);
	resize (800, 360);
}

void AmpGUIManager::on_image_tab_clicked () {
	amp_welcome.hide ();
	amp_video_area->hide();
	amp_viewer.show ();
	amp_viewer.grab_focus();
	amp_toolbar.set_view_mode (IMAGE_VIEW_MODE);
}

void AmpGUIManager::on_video_tab_clicked () {
	amp_welcome.hide ();
	amp_viewer.hide ();
	amp_video_area->show();
	amp_toolbar.set_view_mode (MEDIA_VIEW_MODE);
}

void AmpGUIManager::on_about_clicked () {
	AmpAbout	about;
	about.set_program_name ("Amp");
	about.set_icon ("/home/luis/amp/amp_128.svg");       //this should change...
	about.set_authors ("Luis Gonzalez - Head Developer\n\
	Amit Prayaga - Idea Guy and Developer\n\
	Daniel Harrison - Fan\n\
	Seth Turner     - Beta Tester\n\
	Miguel Gonzalez - Coding Consultant");
	about.set_license ("Amp is free software: you can redistribute it and/or modify it\n\
	under the terms of the GNU General Public License as published by the\n\
	Free Software Foundation, either version 3 of the License, or\n\
	(at your option) any later version.\n\
	\n\
	Amp is distributed in the hope that it will be useful, but\n\
	WITHOUT ANY WARRANTY; without even the implied warranty of\n\
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\
	See the GNU General Public License for more details.\n\
	\n\
	You should have received a copy of the GNU General Public License along\n\
	with this program.  If not, see <http://www.gnu.org/licenses/>.");
	about.set_description ("An All-Around Media Player");

	Gtk::Main::run(about);
}

void AmpGUIManager::select_music_folder () {
	amp_chooser = new AmpChooser("Select Music File",
			Gtk::FILE_CHOOSER_ACTION_OPEN);
	
	Glib::RefPtr<Gtk::FileFilter> filter = Gtk::FileFilter::create ();
	filter->add_mime_type ("audio/mpeg");
	filter->add_mime_type ("audio/mp4");
	filter->set_name ("Music Files");

	amp_chooser->add_filter (filter);
	amp_chooser->set_running (true);

	switch (amp_chooser->run ()) {
		case Gtk::RESPONSE_ACCEPT:
			if (lib_manager->is_music_file ((amp_chooser->get_filename ()))) {
				player->set_file (amp_chooser->get_filename ());
				lib_manager->debug (Glib::ustring::compose("Got the song: %1", amp_chooser->get_filename()));
			} else {
				Gtk::MessageDialog* err_dialog = new Gtk::MessageDialog ("The \
file you chose is not a song. \n Please select another file.", false,
				Gtk::MESSAGE_INFO,
				Gtk::BUTTONS_OK,
				true);
				err_dialog->run ();
				err_dialog->hide ();
				amp_chooser->hide ();
				amp_chooser->set_running (false);
				select_music_folder();
			}
			break;
		default:
			break;
	}
	amp_chooser->hide();
	amp_chooser->set_running (false);
}

void AmpGUIManager::select_image_folder () {
	amp_chooser = new AmpChooser("Select Image Folder", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
	std::vector<Glib::ustring> qt;

	amp_chooser->set_running (true);
	switch (amp_chooser->run ()) {
		case Gtk::RESPONSE_ACCEPT:
			amp_viewer.set_files(lib_manager->count_image_files(Gio::File::create_for_path(amp_chooser->get_filename())));
			break;
		default:
			break;
	}

	amp_chooser->hide();
	amp_chooser->set_running (false);
}

void AmpGUIManager::select_image_file() {
	Glib::RefPtr<Gtk::FileFilter> filter = Gtk::FileFilter::create();
	filter->set_name("Images");
	filter->add_pixbuf_formats();
	
	amp_chooser = new AmpChooser ("Select Image File", Gtk::FILE_CHOOSER_ACTION_OPEN);	
	amp_chooser->set_running(true);
	amp_chooser->add_filter(filter);
	
	switch (amp_chooser->run ()) {
		case Gtk::RESPONSE_ACCEPT:
			if (lib_manager->is_image_file (amp_chooser->get_filename ())) {
				amp_viewer.set_image (amp_chooser->get_filename ());
				amp_toolbar.set_album_artwork (amp_chooser->get_filename ());
				lib_manager->debug (Glib::ustring::compose("Got the image: %1", amp_chooser->get_filename()));
			} else {
				Gtk::MessageDialog* err_dialog = new Gtk::MessageDialog ("The \
file you chose is not an image. \n Please select another file.", false,
				Gtk::MESSAGE_INFO,
				Gtk::BUTTONS_OK,
				true);
				err_dialog->run ();
				err_dialog->hide ();
				amp_chooser->hide ();
				amp_chooser->set_running(false);
				select_image_file ();
			}
			break;
		default:
			break;
	}

	amp_chooser->hide();
	amp_chooser->set_running (false);
}

void AmpGUIManager::wallpaper_info_bar_response (int response) {
	if (response == Gtk::RESPONSE_YES) {
		Glib::RefPtr<Gio::AppInfo> app_info = Gio::AppInfo::create_from_commandline ("gnome-control-center background",
		                                                                             "System Settings",
		                                                                             Gio::APP_INFO_CREATE_NONE);
		if (app_info) {
			Glib::RefPtr<Gio::AppLaunchContext> context;
			Glib::RefPtr<Gdk::Display> display;
			Glib::RefPtr<Gio::File> file = Gio::File::create_for_path (amp_viewer.get_path());

			display = this->get_display();
			context = display->get_app_launch_context();
			
			if (app_info->launch (file, context) == false) {
				lib_manager->error ("Failed to launch System Settings");
			} else {
				lib_manager->debug ("Launched System Settings");
			}
		}
	}
	if (message_area->get_visible()) message_area->hide();
}


bool AmpGUIManager::on_key_press_event (GdkEventKey* event) {
	//std::cout << "A window button was pressed" << std::endl;
	switch (event->keyval) {
		case GDK_KEY_Escape:
			/*if (event->state & GDK_SOMETHING_MASK) {
				do_some_thing;
			}*/
			hide ();
			break;
		case GDK_KEY_h:
			if (event->state & GDK_MOD1_MASK) {
				on_home_tab_clicked ();
			}
			break;
		case GDK_KEY_i:
			if (event->state & GDK_MOD1_MASK) {
				if (!amp_viewer.get_visible ()) {
					on_image_tab_clicked ();
				}
			}
			break;
		case GDK_KEY_o:
			if (event->state & GDK_CONTROL_MASK) {
				if (!chooser_is_running) {
					select_music_folder ();
				}
			}
			break;
		case GDK_KEY_space:
			if (amp_statusbar.get_sensitive ()) {
				//amp_statusbar.set_toggled (!amp_statusbar.get_toggled ());
			}
			break;
		case GDK_KEY_Left:
			if (event->state & GDK_CONTROL_MASK) {
				if (amp_viewer.get_visible()) {
					amp_viewer.previous_image();
				}
			}
			break;
		case GDK_KEY_Right:
			if (amp_viewer.get_visible()) {
				amp_viewer.next_image();
			}
			break;
		case GDK_KEY_F11:
			if (_fullscreen) {
				unfullscreen ();
				_fullscreen = false;
			} else {
				fullscreen ();
				_fullscreen = true;
			}
			break;
		default:
			break;
	}
	return true;
}

void AmpGUIManager::set_value(int64_t value) {
	//amp_statusbar.set_value (static_cast<double>(value));
}
