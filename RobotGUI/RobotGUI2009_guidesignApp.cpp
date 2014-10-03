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


#include <wx/msgdlg.h>
#include "RobotGUI2009_guidesignApp.h"

//(*AppHeaders
#include "RobotGUI2009_guidesignMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(RobotGUI2009_guidesignApp)

#include <string>
#include <stdexcept>
#include <iostream>

using namespace std;
using namespace mrpt;


RobotGUI2009_guidesignApp::RobotGUI2009_guidesignApp() :
	theMOOSApp		(this),
	sMissionFile 	("Mission.moos"),
	sMOOSName 		("iRobotGUI2010"),
	m_theFrame 		(NULL)
{
}

bool RobotGUI2009_guidesignApp::OnInit()
{
	// Starting in wxWidgets 2.9.0, we must reset numerics locale to "C",
	//  if we want numbers to use "." in all countries. The App::OnInit() is a perfect place to undo
	//  the default wxWidgets settings. (JL @ Sep-2009)
	wxSetlocale(LC_NUMERIC,wxString(wxT("C")));

    switch(argc)
    {
    case 3:
        sMOOSName = string( wxString(argv[2]).mbc_str() );
    case 2:
        sMissionFile = string( wxString(argv[1]).mbc_str() );
    }

	// Before launching the MOOS APP, load the mission file from the (this) main thread:
    m_pcr.SetAppName(sMOOSName);
    if (!m_pcr.SetFile(sMissionFile))
		cerr << mrpt::format("iRobotGUI: Error parsing mission file:'%s'\n",sMissionFile.c_str());


    wxInitAllImageHandlers();
	m_theFrame = new RobotGUI2009_guidesignFrame(this, NULL);
	m_theFrame->Show();
	SetTopWindow(m_theFrame);
	//m_theFrame->Notebook2->ChangeSelection(2);
	//m_theFrame->SetWindowStyleFlag( m_theFrame->GetWindowStyleFlag() | wxSTAY_ON_TOP);
	m_theFrame->SetWindowStyleFlag( m_theFrame->GetWindowStyleFlag() | wxMAXIMIZE);
	
    // launch MOOS APP thread:
	m_moos_thread = mrpt::system::createThreadFromObjectMethod(this,&RobotGUI2009_guidesignApp::moos_app_thread);

    return true;
}

int RobotGUI2009_guidesignApp::OnExit()
{
	theMOOSApp.RequestQuit();
	cout << "[iRobotGUI] Waiting for MOOS-app thread to exit." << endl;
	mrpt::system::joinThread(m_moos_thread);
	return wxApp::OnExit();
}

void RobotGUI2009_guidesignApp::moos_app_thread()
{
	try
	{
		cout << "[iRobotGUI] Starting MOOS-app thread." << endl;

		this->theMOOSApp.Run(sMOOSName.c_str(),sMissionFile.c_str());

		cout << "[iRobotGUI] Exiting MOOS-app thread." << endl;
	}
	catch (std::exception &e)
	{
		wxMessageBox( _U(e.what()), _("Exception in MOOS-app thread") );
	}
	catch (...)
	{
		wxMessageBox( _("Untyped exception"), _("Exception in MOOS-app thread") );
	}
}
