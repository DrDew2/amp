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

#include "amp-player.h"
#include "amp-library-manager.h"

extern AmpLibraryManager * lib_manager;
extern AmpGUIManager*		guimanager;

bool bus_call(const Glib::RefPtr<Gst::Bus> &bus, const Glib::RefPtr<Gst::Message> &message, Glib::RefPtr<Glib::MainLoop> &loop) {
	switch (message->get_message_type ()) {
		case Gst::MESSAGE_EOS:
			loop->quit();
			std::cout << "End of Stream\nPipeline Terminated" << std::endl;
			break;
		case Gst::MESSAGE_ERROR:
			/*	Glib::ustring debug;
			 Glib::Error error;
			 * 
			 message->
			 * 
			 gst_message_parse_error (msg, &error, &debug);
			 g_free (debug);

			 g_error ("Pipeline recieved error:\t%s", error->message);
			 g_error_free (error);
			 */
			lib_manager->error("There was an error in the Player");
			loop->quit();
			break;
		default:
			break;
	}
	return true;
}

AmpPlayer::AmpPlayer () {
	loop = Glib::MainLoop::create();
	playbin2 = Gst::PlayBin2::create();
	bus = playbin2->get_bus();
	xoverlay = Gst::XvImageSink::create("videosink");
	playbin2->set_property ("video-sink", xoverlay);
	
	bus->add_watch(sigc::bind(sigc::ptr_fun(&bus_call), loop));
	Glib::signal_timeout().connect(sigc::mem_fun(*this, &AmpPlayer::print_position), 10);
	
	lib_manager->debug ("Player created succesfully");
}

AmpPlayer::~AmpPlayer () {
	playbin2->set_state(Gst::STATE_NULL);
	loop->quit ();
	lib_manager->debug("Player destroyed succesfully");
}


void AmpPlayer::set_file (Glib::ustring filepath) {
	current_file = Gio::File::create_for_uri(Glib::filename_to_uri (filepath));
	guimanager->set_controls_sensitive (true);
}


void AmpPlayer::set_state (Gst::State state) {
	playbin2->set_state(state);
}

void AmpPlayer::play ()  {
	playbin2->set_property("uri", current_file->get_uri());
	playbin2->set_state(Gst::STATE_PLAYING);
	loop->run();
	playbin2->set_state(Gst::STATE_NULL);
	guimanager->set_controls_sensitive (false);
}

void AmpPlayer::pause () {
	playbin2->set_state(Gst::STATE_PAUSED);
}

bool AmpPlayer::print_position () {
	Gst::Format fmt = Gst::FORMAT_TIME;
	if (playbin2->query_position (fmt, position) && playbin2->query_duration (fmt, duration)) {
		std::cout << "\r " << position;
		std::cout << " / " << duration;
	}
	return true;
}

