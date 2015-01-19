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



/**  @moos_module A GUI application to visualize and manipulate the mobile robot.
  */

#include <wx/msgdlg.h>  // Needed to enable windows

#include "GUIMOOSApp.h"
#include "RobotGUI2009_guidesignApp.h"
#include "RobotGUI2009_guidesignMain.h"
#include <mrpt/utils/CTicTac.h>

#include <sstream>
#include <iomanip>
#include <iostream>

using namespace std;

using namespace mrpt;
using namespace mrpt::utils;
using namespace mrpt::synch;
using namespace mrpt::gui;
using namespace mrpt::poses;


GUIMOOSApp::GUIMOOSApp( RobotGUI2009_guidesignApp *the_wx_app ) :
	m_theWxApp(the_wx_app)
{
}

GUIMOOSApp::~GUIMOOSApp()
{
}

bool GUIMOOSApp::OnStartUp()
{

	// READ ACTIONS FROM MISSION FILE
	std::string actions_names;
	std::vector<std::string> actionList;
	m_MissionReader.GetConfigurationParam("pCommon","actions_names",actions_names);

	        char *actBuff;
        char actChar[1024];
        strcpy(actChar,actions_names.c_str());
        actBuff = strtok(actChar,"@");
        while (actBuff)
        {
            actionList.push_back(*(new string(actBuff)));
            actBuff = strtok(NULL,"@");
        }
        // set the action list (first, convert it!)
        std::vector<std::string>::iterator action = actionList.begin();
        std::vector<wxString> actions;
        wxString action2;
		int num = 0;
        while (action!=actionList.end()){
            action2 = wxString::FromAscii((*action).c_str());
            actions.push_back(action2);
            action++;
			MOOSTrace("STATECHARTSEDIT %i\n",num);
			num++;
        }
		MOOSTrace("\n");
		fflush(0);
 m_theWxApp->m_theFrame->boardSCE->setActions(actions);

	// /READ ACTIONS

	//! @moos_param robot_3DS_model_file If this variable is present, a 3D Studio file (.3ds, .3ds.gz) will be loaded to represent the robot in 3D.
	//!              If it's not present, a default 3D model will be used. See also: robot_3D_model
	string file_3ds;

	if (m_MissionReader.GetConfigurationParam("robot_3DS_model_file",file_3ds))
	{
		try
		{
			cout << "[iRobotGUI] Loading 3DS model: " << file_3ds << endl;
			mrpt::opengl::C3DSScenePtr obj =  mrpt::opengl::C3DSScene::Create();
			obj->loadFrom3DSFile(file_3ds);
			//obj->enableExtraAmbientLight(true);
			// Replace current 3D model:
			m_theWxApp->m_theFrame->m_gl_robot_model->clear();
			m_theWxApp->m_theFrame->m_gl_robot_model->insert(obj);
			cout << "[iRobotGUI] 3DS model loaded OK." << endl;
		}
		catch (std::exception &)
		{
		}
	}
	std::string read_mission;
	if(m_MissionReader.GetConfigurationParam("mission1",read_mission))
		(m_theWxApp->m_theFrame->missionList).push_back(read_mission);

	if(m_MissionReader.GetConfigurationParam("mission2",read_mission))
		(m_theWxApp->m_theFrame->missionList).push_back(read_mission);

	if(m_MissionReader.GetConfigurationParam("mission3",read_mission))
		(m_theWxApp->m_theFrame->missionList).push_back(read_mission);

	if(m_MissionReader.GetConfigurationParam("mission4",read_mission))
		(m_theWxApp->m_theFrame->missionList).push_back(read_mission);

	if(m_MissionReader.GetConfigurationParam("mission5",read_mission))
		(m_theWxApp->m_theFrame->missionList).push_back(read_mission);

	if(m_MissionReader.GetConfigurationParam("mission6",read_mission))
		(m_theWxApp->m_theFrame->missionList).push_back(read_mission);

	if(m_MissionReader.GetConfigurationParam("stop_mission",read_mission))
		(m_theWxApp->m_theFrame->missionList).push_back(read_mission);

	if(m_MissionReader.GetConfigurationParam("mission1_title",read_mission))
		m_theWxApp->m_theFrame->mission1->SetLabel(wxString(read_mission.c_str(), wxConvUTF8));

	if(m_MissionReader.GetConfigurationParam("mission2_title",read_mission))
		m_theWxApp->m_theFrame->mission2->SetLabel(wxString(read_mission.c_str(), wxConvUTF8));

	if(m_MissionReader.GetConfigurationParam("mission3_title",read_mission))
		m_theWxApp->m_theFrame->mission3->SetLabel(wxString(read_mission.c_str(), wxConvUTF8));

	if(m_MissionReader.GetConfigurationParam("mission4_title",read_mission))
		m_theWxApp->m_theFrame->mission4->SetLabel(wxString(read_mission.c_str(), wxConvUTF8));

	if(m_MissionReader.GetConfigurationParam("mission5_title",read_mission))
		m_theWxApp->m_theFrame->mission5->SetLabel(wxString(read_mission.c_str(), wxConvUTF8));

	if(m_MissionReader.GetConfigurationParam("mission6_title",read_mission))
		m_theWxApp->m_theFrame->mission6->SetLabel(wxString(read_mission.c_str(), wxConvUTF8));

	return DoRegistrations();
}

bool GUIMOOSApp::OnCommandMsg( CMOOSMsg Msg )
{
    if(Msg.IsSkewed(MOOSTime()))
        return true;

    if(!Msg.IsString())
        return MOOSFail("iRobotGUI only accepts string command messages\n");

    std::string sCmd = Msg.GetString();

    MOOSTrace("COMMAND RECEIVED: %s\n",sCmd.c_str());

    return true;
}

bool GUIMOOSApp::Iterate()
{
	// Send vars GUI -> MOOS
	{
		CCriticalSectionLocker lock(& m_theWxApp->m_lstToSendToMOOS_cs );
		pair<string,string>  d;

		while (! m_theWxApp->m_lstToSendToMOOS.empty() )
		{
			// Pop:
			d = m_theWxApp->m_lstToSendToMOOS.front();
			m_theWxApp->m_lstToSendToMOOS.pop();

			// Send to MOOS:
			m_Comms.Notify(d.first,d.second);

//			cout << "Sending to MOOS: " << d.first << " -> " << d.second << endl;
		}
	}

	// Only at a small frequency, query meta-information on existing modules, etc.
	static CTicTac tim;
	string LIST_MODULES_DATA;
	if (tim.Tac()>5.0)
	{
		tim.Tic();
		if(m_Comms.IsConnected())
		{
			MOOSMSG_LIST InMail;
			if(m_Comms.ServerRequest("PROC_SUMMARY",InMail, 3.0 /* timeout */, false /* dont clear main */ ))
			{
				for(MOOSMSG_LIST::const_iterator p =InMail.begin();p!=InMail.end();++p)
				{
					const CMOOSMsg & rMsg = *p;
					if ("PROC_SUMMARY"==rMsg.GetKey())
					{
						const string &val = rMsg.m_sVal;
						// Typical string: "pLocalizationPF:SUBSCRIBED=...,PLOCALIZATIONPF_STATUS"
						size_t i;
						if ( (i=val.find(":"))!=string::npos)
						{
							const string mod_name = val.substr(0,i);
							LIST_MODULES_DATA+=mod_name;
							LIST_MODULES_DATA+=",";
							//cout << "module: " << mod_name  << endl;

							// Subscribe, if we are not already, to the status of that module:
							const string sVarName = mrpt::system::upperCase(mod_name)+string("_STATUS");
							if (AddMOOSVariable(sVarName,sVarName,sVarName, 2.0 /* min. seconds between updates */) )
								RegisterMOOSVariables(); // Only update if the variable is new.
						}
					}
				} // end for each msg
			}
		}
	} // end meta-information


	// Process changes in MOSS -> GUI
	{
		CCriticalSectionLocker lock(& m_theWxApp->m_lstFromMOOS_cs);
		// MOOS vars:
		for (MOOSVARMAP::iterator it=m_MOOSVars.begin();it!=m_MOOSVars.end();it++)
			if ( it->second.IsFresh() )
			{
				it->second.SetFresh(false);
				if (it->second.IsDouble())
						m_theWxApp->m_lstFromMOOS[it->first] = format("%f",it->second.GetDoubleVal());
				else	m_theWxApp->m_lstFromMOOS[it->first] = it->second.GetStringVal();
			}
		// MAPIR vars:
		for (MOOSVARMAP::iterator it=m_OpenMORA_subscribedVars.begin();it!=m_OpenMORA_subscribedVars.end();it++)
			if ( it->second.IsFresh() )
			{
				it->second.SetFresh(false);
				if (it->second.IsDouble())
						m_theWxApp->m_lstFromMOOS[it->first] = format("%f",it->second.GetDoubleVal());
				else	m_theWxApp->m_lstFromMOOS[it->first] = it->second.GetStringVal();
			}

		if (!LIST_MODULES_DATA.empty())
			m_theWxApp->m_lstFromMOOS["LIST_MODULES"] = LIST_MODULES_DATA;
	}

    return true;
}

bool GUIMOOSApp::OnConnectToServer()
{
    DoRegistrations();
    return true;
}



bool GUIMOOSApp::OnNewMail(MOOSMSG_LIST &NewMail)
{
    UpdateMOOSVariables(NewMail);
    UpdateMOOSVariables_OpenMORA(NewMail);
    return true;
}
