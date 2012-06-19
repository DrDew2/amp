/* amp
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

#ifndef _AMP_GUI_MANAGER_H_
#define _AMP_GUI_MANAGER_H_
//#include <gtkmm.h>
#include "amp-about.h"
#include "amp-button.h"
#include "amp-statusbar.h"
#include "amp-toolbar.h"
#include "amp-viewer.h"
#include "amp-welcome.h"
#include "amp-library-manager.h"
#include "amp-player.h"
#include "amp-chooser.h"

#include <gtkmm/drawingarea.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/hvbox.h>
#include <gtkmm/image.h>
#include <gtkmm/menubar.h>
#include <gtkmm/menutoolbutton.h>
#include <gtkmm/toggletoolbutton.h>
#include <gtkmm/toolitem.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/window.h>

class AmpGUIManager: public Gtk::Window 
{
public:
	AmpGUIManager ();
	virtual ~AmpGUIManager ();
	void set_controls_sensitive (bool sensitive);
	void set_value(int64_t value);
	void next_image();
	void previous_image ();

protected:
	void	set_wallpaper ();
	//Events
	void	on_home_tab_clicked ();
	void    on_image_tab_clicked ();
	void	on_video_tab_clicked ();
	void	select_music_folder ();
	void	select_image_folder ();
	void	select_image_file();
//	void	select_video_folder ();  //TODO To be created...
	void    on_about_clicked ();
	bool    on_key_press_event (GdkEventKey* event);
	void	wallpaper_info_bar_response (int response);

	void    update_preview_cb ();
		
	bool    chooser_is_running, _fullscreen;
	Gtk::DrawingArea*		amp_video_area;

private:
	//Child Widgets
	Gtk::VBox*				amp_main_box;
	Gtk::MenuBar*  			amp_menu_bar;
	AmpToolbar              amp_toolbar;
	Gtk::HBox*              amp_tab_bar;
	AmpWelcome              amp_welcome;
	AmpViewer               amp_viewer; //for images
	AmpStatusbar 			amp_statusbar;
	AmpChooser*				amp_chooser;
	Gtk::InfoBar*			message_area;
	///Gtk::ToolButton*        prev,   *next;
	//Gtk::ToggleToolButton*  play;
	//Gtk::SeparatorToolItem* sep;
	//Gtk::ToolItem*          state_area;
	//Gtk::MenuToolButton*    pref_tool;

	//subprograms to make managing the window easier
	void create_toolbar ();
	void create_menu_bar ();
	void create_file_chooser ();
};

#endif // _AMP_GUI_MANAGER_H_
