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

#include "amp-library-manager.h"
#include <iostream>
#define AMP_SETTINGS_SCHEMA     "org.mcgrath.amp"

extern AmpGUIManager* guimanager;

AmpLibraryManager::AmpLibraryManager () {
	music_folder = Gio::File::create_for_path (
			Glib::build_filename (Glib::get_home_dir (),
					"/Music"));
	if (!music_folder->query_exists ()) {
		if (music_folder->make_directory ()) {
			debug ("Made the user's music directory");
		} else {
			error ("Could not make the music directory");
		}
	}
	data_folder = Gio::File::create_for_path (
			Glib::build_filename (Glib::get_user_data_dir (),
					"/Amp"));
	if (!data_folder->query_exists ()) {
		if (data_folder->make_directory ()) {
			debug ("Made the data directory");
		} else {
			error ("Could not make the data directory");
		}
	}
	//get_user_dir_data ();
	debug ("Library Manager created succesfully");
}

AmpLibraryManager::~AmpLibraryManager () {
	debug ("Library Manager destroyed succesfully");
}

void AmpLibraryManager::debug (Glib::ustring bug) {
	std::cout << "\e[32m** Debug[" << get_time() << "]:\t" << bug << "\e[m" << std::endl;
}

std::string AmpLibraryManager::get_time() {
	time(&raw_time);
	timeinfo = localtime (&raw_time);

	std::string str(asctime(timeinfo));
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	return str;
}

void AmpLibraryManager::error (Glib::ustring err) {
	std::cout << "\e[31m** ERROR[" << get_time() << "]:\t" << err << ". Please report this. \e[m" << std::endl;
}

void AmpLibraryManager::warning (Glib::ustring warning) {
	std::cout << "\e[33m** Warning["<< get_time() << "]:\t\e[m" << warning << std::endl;
}

void AmpLibraryManager::get_user_dir_data () {
	std::cout << "\n";
	std::cout << "User Name:\t\t\t" << Glib::get_user_name () << std::endl;
	std::cout << "User Real Name:\t\t\t" << Glib::get_real_name () << std::endl;
	std::cout << "User Home Directory:\t\t" << Glib::get_home_dir () << std::endl;
	std::cout << "User Cache Directory:\t\t" << Glib::get_user_cache_dir () << std::endl;
	std::cout << "User Config Directory:\t\t" << Glib::get_user_config_dir () << std::endl;
	std::cout << "User Data Directory:\t\t" << Glib::get_user_data_dir () << std::endl;
	std::cout << "User Temporary Directory:\t" << Glib::get_tmp_dir () << "hi" << std::endl;
}

std::vector<Glib::ustring> AmpLibraryManager::count_image_files (Glib::RefPtr<Gio::File> folder) {
	std::vector<Glib::ustring> storage;
	Glib::RefPtr<Gio::File> file;
	Glib::RefPtr<Gio::FileInfo> info;
	Glib::RefPtr<Gio::FileEnumerator> enumerator = folder->enumerate_children ();
	Glib::ustring path;
	try {
		while ((info = enumerator->next_file ()) != 0) {
			path = Glib::build_filename (folder->get_path (), "/", info->get_name ());
			file = Gio::File::create_for_path (path);
			if (info->get_file_type () == Gio::FILE_TYPE_REGULAR && is_image_file (info->get_name ())) {
				storage.push_back (file->get_path ());
			}
		}
		debug (Glib::ustring::compose("Successfully scanned folder %1", folder->get_path()));
	} catch (Glib::Error er) {
		warning	(Glib::ustring::compose ("Failed to scan folder: %1", folder->get_path ()));
	}
	std::cout << "about to return storage" << std::endl;
	return storage;
}

void AmpLibraryManager::count_music_files () {
	Glib::RefPtr<Gio::File> file;
	Glib::RefPtr<Gio::FileInfo> info;
	Glib::RefPtr<Gio::FileEnumerator> enumerator = music_folder->enumerate_children ();
	Glib::ustring path;
	try {
		while ((info = enumerator->next_file ()) != 0) {
			path = Glib::build_filename (music_folder->get_path (),
					"/", info->get_name ());

			file = Gio::File::create_for_path (path);
			if (info->get_file_type () == Gio::FILE_TYPE_REGULAR && is_music_file (info->get_name ())) {
				files_count_int++;
				files_count.push_back (file->get_uri ());
				std::cout << path << " is a media file" << std::endl;
			} else if (info->get_file_type () == Gio::FILE_TYPE_REGULAR && is_image_file (info->get_name ())){
				std::cout << path << " is an image" << std::endl;
			} else if (info->get_file_type () == Gio::FILE_TYPE_DIRECTORY) {
				std::cout << path << " is an directory" << std::endl;
				count_sub_dir (file);
			}
		}
		warning ("Done");
	} catch (Glib::Error er) {
		warning	(Glib::ustring::compose ("Could not pre-scan folder:", music_folder->get_path ()));
	}
}

void AmpLibraryManager::count_sub_dir (Glib::RefPtr<Gio::File> folder) {
	Glib::RefPtr<Gio::File> file;
	Glib::RefPtr<Gio::FileInfo> info;
	Glib::RefPtr<Gio::FileEnumerator> enumerator = folder->enumerate_children ();
	Glib::ustring path;
	try {
		while ((info = enumerator->next_file ()) != 0) {
			path = Glib::build_filename (folder->get_path (),
					"/", info->get_name ());
			file = Gio::File::create_for_path (path);

			if (info->get_file_type () == Gio::FILE_TYPE_REGULAR &&
					is_music_file (info->get_name ())) {
				files_count_int++;
				files_count.push_back (file->get_uri ());
				std::cout << path << " is a media file" << std::endl;
			} else if (info->get_file_type () == Gio::FILE_TYPE_REGULAR &&
					is_image_file (info->get_name ())){
				std::cout << path << " is an image" << std::endl;
			} else if (info->get_file_type () == Gio::FILE_TYPE_DIRECTORY) {
				count_sub_dir (file);
			}
		}
	} catch (Glib::Error er) {
		warning	(Glib::ustring::compose ("Could not pre-scan folder: %s", folder->get_path ()));
	}
}

bool AmpLibraryManager::is_music_file (Glib::ustring file) {
	return (Glib::str_has_suffix (file, ".mp3") || Glib::str_has_suffix (file, ".m4a") ||
			Glib::str_has_suffix (file, ".ogg") || Glib::str_has_suffix (file, ".flac") ||
			Glib::str_has_suffix (file, ".wav") || Glib::str_has_suffix (file, ".aif") ||
			Glib::str_has_suffix (file, ".m4b") || Glib::str_has_suffix (file, ".mid") ||
			Glib::str_has_suffix (file, ".mkv") || Glib::str_has_suffix (file, ".m4v") ||
			Glib::str_has_suffix (file, ".mov") || Glib::str_has_suffix (file, ".mp4") ||
			Glib::str_has_suffix (file, ".avi") || Glib::str_has_suffix (file, ".flv") ||
			Glib::str_has_suffix (file, ".mkv") || Glib::str_has_suffix (file, ".ogm") ||
			Glib::str_has_suffix (file, ".ogv") || Glib::str_has_suffix (file, ".swf") ||
			Glib::str_has_suffix (file, ".mpg") || Glib::str_has_suffix (file, ".wmv"));
}

bool AmpLibraryManager::is_image_file (Glib::ustring file) {
	return (Glib::str_has_suffix (file, ".jpg") || Glib::str_has_suffix (file, ".JPG") ||
			Glib::str_has_suffix (file, ".jpeg") || Glib::str_has_suffix (file, ".JPEG") ||
			Glib::str_has_suffix (file, ".png") || Glib::str_has_suffix (file, ".PNG") ||
			Glib::str_has_suffix (file, ".svg") || Glib::str_has_suffix (file, ".SVG") ||
			Glib::str_has_suffix (file, ".gif") || Glib::str_has_suffix (file, ".GIF"));
}

Glib::ustring AmpLibraryManager::get_music_folder () {
	return music_folder->get_path ();
}
