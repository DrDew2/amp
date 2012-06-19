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

#ifndef _AMP_LIBRARY_MANAGER_H_
#define _AMP_LIBRARY_MANAGER_H_
#include <iostream>
#include <vector>
#include <time.h>
#include <glibmm/ustring.h>
#include <giomm/file.h>
#include "amp-gui-manager.h"

class AmpLibraryManager
{
public:
	AmpLibraryManager();
	virtual ~AmpLibraryManager();
	Glib::ustring get_music_folder();
	void music_library_chooser_cb();
	void debug(Glib::ustring bug);
	void error(Glib::ustring err);
	void warning(Glib::ustring warning);
	bool is_music_file(Glib::ustring file);
	bool is_image_file(Glib::ustring file);
	void count_music_files();
	std::vector<Glib::ustring> count_image_files(Glib::RefPtr<Gio::File> folder);
	//std::vector<Glib::ustring> image_files; //Only for the Viewer to read. Not others

protected:
	time_t raw_time;
	struct tm * timeinfo;
	Glib::RefPtr<Gio::File> 	music_folder;
	Glib::RefPtr<Gio::File>		data_folder;
	//Glib::RefPtr<Gio::Settings> settings;	//TODO needs research
	std::vector<Glib::ustring> files_count;
	int files_count_int;
	void scan_folder (Glib::RefPtr<Gio::File> folder);

	void count_sub_dir (Glib::RefPtr<Gio::File> sub_folder);

private:
	bool have_music_folder;
	void get_user_dir_data ();

	std::string get_time();
};

#endif // _AMP_LIBRARY_MANAGER_H_
