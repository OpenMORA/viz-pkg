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


#ifndef ROBOTGUI2009_GUIDESIGNAPP_H
#define ROBOTGUI2009_GUIDESIGNAPP_H

#include "GUIMOOSApp.h"

#include <queue>

#include <mrpt/synch.h>
#include <mrpt/system/threads.h>


// JL: Funny Win32-wxWidgets collisions...
#ifdef Yield
#undef Yield
#endif
#ifdef GetClassInfo
#undef GetClassInfo
#endif
#ifdef CreateDialog
#undef CreateDialog
#endif
#ifdef CreateFont
#undef CreateFont
#endif
#ifdef CreateWindow
#undef CreateWindow
#endif
#ifdef LoadMenu
#undef LoadMenu
#endif
#ifdef FindText
#undef FindText
#endif
#ifdef LoadIcon
#undef LoadIcon
#endif
#ifdef LoadBitmap
#undef LoadBitmap
#endif
#include <mrpt/gui/WxUtils.h>
#include <wx/app.h>

#include <MOOS/libMOOS/Utils/ProcessConfigReader.h> // for CProcessConfigReader


#define ROBOTGUI_MAIN_SCREEN_REFRESH_PERIOD   0.1    // In seconds


class RobotGUI2009_guidesignFrame;

class RobotGUI2009_guidesignApp : public wxApp
{
    public:
		RobotGUI2009_guidesignApp();

        virtual bool OnInit();
		virtual int OnExit();

		GUIMOOSApp theMOOSApp;   //!< The MOOS App object
		void moos_app_thread(); //!< The thread theMOOSApp runs in.
		mrpt::system::TThreadHandle 	m_moos_thread;

		std::string sMissionFile;
		std::string sMOOSName;
		CProcessConfigReader  m_pcr; //!< A parser of the mission file, independent of that one inside the MOOSAPP object


		// ================  Shared objects between wxApp and MOOSApp ================
		std::queue< std::pair<std::string,std::string> > 	m_lstToSendToMOOS;
		mrpt::synch::CCriticalSection 						m_lstToSendToMOOS_cs;

		std::map<std::string,std::string> 	m_lstFromMOOS;  // var name -> most recent value
		mrpt::synch::CCriticalSection 		m_lstFromMOOS_cs;
		// ===========================================================================


		RobotGUI2009_guidesignFrame *m_theFrame;

};

#endif // ROBOTGUI2009_GUIDESIGNAPP_H
