/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.cc
 * Copyright (C) 2012 The Amp Team <luisito.gonzalez72@gmail.com>
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

#include <gtkmm.h>
#include <iostream>

#include "config.h"

#ifdef ENABLE_NLS
#  include <libintl.h>
#endif

#include "amp-gui-manager.h"
#include "amp-library-manager.h"
#include "amp-player.h"

#include <gtkmm/application.h>
#include <gstreamermm/init.h>

AmpLibraryManager* lib_manager;
AmpGUIManager*	guimanager;
AmpPlayer*		player;
   
int
main (int argc, char *argv[])
{
	int r;
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, 
	                                                         "org.mcgrath.amp");
	Gst::init(argc, argv);

	lib_manager = new AmpLibraryManager ();
	guimanager = new AmpGUIManager ();
	player = new AmpPlayer ();

	r = app->run(*guimanager);
	
	delete guimanager;
	return r;
}
