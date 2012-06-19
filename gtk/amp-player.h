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

#ifndef _AMP_PLAYER_H_
#define _AMP_PLAYER_H_

#include <glibmm.h>

#include <giomm/file.h>
#include <glibmm/main.h>
#include <gstreamermm/bus.h>
#include <gstreamermm/xvimagesink.h>
#include <gstreamermm/playbin2.h>

class AmpPlayer
{
	public:
		AmpPlayer();
		virtual ~AmpPlayer();

		void set_state (Gst::State state);
		void set_position (double position);
		void set_file (Glib::ustring filepath);
		void play();
		void pause ();
		bool print_position ();
		
		gint64 position, duration;

	protected:
//		Glib::ustring	current_file;	//this should go in library manager
		Glib::RefPtr<Gio::File> current_file;

	private:
		//Basic Necessities
		Glib::RefPtr<Glib::MainLoop> loop;
		Glib::RefPtr<Gst::PlayBin2> playbin2;
		Glib::RefPtr<Gst::Bus> bus;
		Glib::RefPtr<Gst::XvImageSink> xoverlay;
};

#endif // _AMP_PLAYER_H_
