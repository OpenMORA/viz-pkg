/* +---------------------------------------------------------------------------+
   |                 Open MORA (MObile Robot Arquitecture)                     |
   |                                                                           |
   |                        http://babel.isa.uma.es/mora/                      |
   |                                                                           |
   |   Copyright (C) 2010  University of Malaga                                |
   |                                                                           |
   |    This software was written by the Machine Perception and Intelligent    |
   |      Robotics (MAPIR) Lab, University of Malaga (Spain).                  |
   |    Contact: Jose-Luis Blanco  <jlblanco@ctima.uma.es>                     |
   |                                                                           |
   |  This file is part of the MORA project.                                   |
   |                                                                           |
   |     MORA is free software: you can redistribute it and/or modify          |
   |     it under the terms of the GNU General Public License as published by  |
   |     the Free Software Foundation, either version 3 of the License, or     |
   |     (at your option) any later version.                                   |
   |                                                                           |
   |   MORA is distributed in the hope that it will be useful,                 |
   |     but WITHOUT ANY WARRANTY; without even the implied warranty of        |
   |     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         |
   |     GNU General Public License for more details.                          |
   |                                                                           |
   |     You should have received a copy of the GNU General Public License     |
   |     along with MORA.  If not, see <http://www.gnu.org/licenses/>.         |
   |                                                                           |
   +---------------------------------------------------------------------------+ */

#ifndef CMapirArtProvider_H
#define CMapirArtProvider_H

#include <wx/artprov.h>

// ---------------------------------------------------------
// Include all icons, in XPM format (24x24)
//  Some of the icons come from "Crystal Clear - Wikimedia"
// ----------------------------------------------------------
#include "icons/reset.xpm"
#include "icons/relocalize.xpm"
#include "icons/joy.xpm"
#include "icons/navigate.xpm"
#include "icons/stop.xpm"
#include "icons/zoom_in.xpm"
#include "icons/zoom_out.xpm"
#include "icons/quit.xpm"

#include "icons/resume.xpm"
#include "icons/place_robot.xpm"

#include "icons/battery_dead.xpm"
#include "icons/battery_l1.xpm"
#include "icons/battery_l2.xpm"
#include "icons/battery_full.xpm"
#include "icons/battery_charging.xpm"
/** wxWidgets generator for custom icons.
  *  Add to any wxWidgets frame with:
  *    wxArtProvider::Push(new CMapirArtProvider);
  */
class CMapirArtProvider : public wxArtProvider
{
protected:
    virtual wxBitmap CreateBitmap(const wxArtID& id,
                                  const wxArtClient& client,
                                  const wxSize& size)
	{
		if (id == wxART_MAKE_ART_ID(ICON_RESET)) return wxImage(reset_xpm);
		if (id == wxART_MAKE_ART_ID(ICON_RELOCALIZE)) return wxImage(relocalize_xpm);
		if (id == wxART_MAKE_ART_ID(ICON_JOY)) return wxImage(joy_xpm);
		if (id == wxART_MAKE_ART_ID(ICON_NAVIGATE)) return wxImage(navigate_xpm);
		if (id == wxART_MAKE_ART_ID(ICON_STOP)) return wxImage(stop_xpm);
		if (id == wxART_MAKE_ART_ID(ICON_ZOOM_IN)) return wxImage(zoom_in_xpm);
		if (id == wxART_MAKE_ART_ID(ICON_ZOOM_OUT)) return wxImage(zoom_out_xpm);
		if (id == wxART_MAKE_ART_ID(ICON_QUIT)) return wxImage(quit_xpm);

		if (id == wxART_MAKE_ART_ID(ICON_RESUME)) return wxImage(resume_xpm);    // JL: ADD THIS ONE! resume_xpm);
		if (id == wxART_MAKE_ART_ID(PLACE_ROBOT)) return wxImage(place_robot_xpm); // JL: ADD THIS ONE! place_robot_xpm);
		if (id == wxART_MAKE_ART_ID(BATTERY_DEAD)) return wxImage(battery_dead_xpm);
		if (id == wxART_MAKE_ART_ID(BATTERY_L1)) return wxImage(battery_l1_xpm);
		if (id == wxART_MAKE_ART_ID(BATTERY_L2)) return wxImage(battery_l2_xpm);
		if (id == wxART_MAKE_ART_ID(BATTERY_FULL)) return wxImage(battery_full_xpm);
		if (id == wxART_MAKE_ART_ID(BATTERY_CHARGING)) return wxImage(battery_charging_xpm);

		// Any wxWidgets icons not implemented here will be provided by the default art provider.
		return wxNullBitmap;
	}
};

#endif
