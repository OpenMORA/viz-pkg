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

#include "GUIMOOSApp.h"
#include "RobotGUI2009_guidesignApp.h"

using namespace std;
using namespace mrpt;

bool GUIMOOSApp::DoRegistrations()
{
	//! @moos_subscribe	JOYSTICK_MODE
	//! @moos_publish	JOYSTICK_MODE A string either "0" or "1" to enable manual control with a Joystick

	AddMOOSVariable_OpenMORA("JOYSTICK_MODE", 0);

	//! @moos_subscribe LOCALIZATION, LOCALIZATION_COV, LOCALIZATION_PARTICLES
	AddMOOSVariable_OpenMORA("LOCALIZATION", ROBOTGUI_MAIN_SCREEN_REFRESH_PERIOD);
	AddMOOSVariable_OpenMORA("LOCALIZATION_COV", 0.5);
	AddMOOSVariable_OpenMORA("LOCALIZATION_PARTICLES", 0.2);

	//! @moos_subscribe LASER1, LASER2, LASER3
	for (int i=1;i<=3;i++)
	{
		const string s=format("LASER%i",i);
		AddMOOSVariable(s,s,s, ROBOTGUI_MAIN_SCREEN_REFRESH_PERIOD );
	}

	//! @moos_subscribe KINECT1
	AddMOOSVariable("KINECT1", "KINECT1", "KINECT1", ROBOTGUI_MAIN_SCREEN_REFRESH_PERIOD);

	//! @moos_subscribe GRIDPOINTS
	AddMOOSVariable("GRIDPOINTS", "GRIDPOINTS", "GRIDPOINTS", ROBOTGUI_MAIN_SCREEN_REFRESH_PERIOD);

	//! @moos_subscribe SONAR1
	AddMOOSVariable("SONAR1", "SONAR1", "SONAR1", ROBOTGUI_MAIN_SCREEN_REFRESH_PERIOD);
	//! @moos_subscribe SONAR2
	AddMOOSVariable("SONAR2", "SONAR2", "SONAR2", ROBOTGUI_MAIN_SCREEN_REFRESH_PERIOD);

	//! @moos_subscribe INFRARED1
	AddMOOSVariable("INFRARED1", "INFRARED1", "INFRARED1", ROBOTGUI_MAIN_SCREEN_REFRESH_PERIOD);

	//! @moos_subscribe VIRTUAL_WORLD By now it only loads a fixed virtual world from disk
	AddMOOSVariable("VIRTUAL_WORLD", "VIRTUAL_WORLD", "VIRTUAL_WORLD", ROBOTGUI_MAIN_SCREEN_REFRESH_PERIOD);

	//! @moos_subscribe AVAILABLE_ACTIONS
	AddMOOSVariable("AVAILABLE_ACTIONS", "AVAILABLE_ACTIONS", "AVAILABLE_ACTIONS", ROBOTGUI_MAIN_SCREEN_REFRESH_PERIOD);


	//! @moos_publish GUI_VISIBLE_LASER1  Whether to show (="1") or not (="0") this sensor in the GUI.
	//! @moos_publish GUI_VISIBLE_LASER2  Whether to show (="1") or not (="0") this sensor in the GUI.
	//! @moos_publish GUI_VISIBLE_LASER3  Whether to show (="1") or not (="0") this sensor in the GUI.
	//! @moos_publish GUI_VISIBLE_SONAR1  Whether to show (="1") or not (="0") this sensor in the GUI.
	//! @moos_publish GUI_VISIBLE_SONAR2  Whether to show (="1") or not (="0") this sensor in the GUI.
	//! @moos_publish GUI_VISIBLE_INFRARED1  Whether to show (="1") or not (="0") this sensor in the GUI.
	//! @moos_publish GUI_VISIBLE_VIRTUAL_OBS  Whether to show (="1") or not (="0") the navigator global virtual obstacles.
	//! @moos_publish GUI_VISIBLE_TOPOLOGICAL_MAP  Whether to show (="1") or not (="0") the topological map.
	//! @moos_publish GUI_VISIBLE_METRIC_MAP  Whether to show (="1") or not (="0") the metric map.
	//! @moos_publish GUI_VISIBLE_PARTICLES  Whether to show (="1") or not (="0") the particles of the robot localization.
	//! @moos_publish GUI_VISIBLE_GAS_MEAN_ONLINE_MAP Whether to show (="1") or not (="0") the mean_gasmap map.
	//! @moos_publish GUI_VISIBLE_GAS_VAR_ONLINE_MAP Whether to show (="1") or not (="0") the var_gasmap map.
	//! @moos_publish GUI_VISIBLE_WIND_ONLINE_MAP Whether to show (="1") or not (="0") the mean_gasmap map.

	//! @moos_subscribe GUI_VISIBLE_LASER1, GUI_VISIBLE_LASER2, GUI_VISIBLE_LASER3, GUI_VISIBLE_SONAR1, GUI_VISIBLE_SONAR2, GUI_VISIBLE_INFRARED1
	//! @moos_subscribe GUI_VISIBLE_VIRTUAL_OBS, GUI_VISIBLE_TOPOLOGICAL_MAP, GUI_VISIBLE_METRIC_MAP GUI_VISIBLE_VIRTUAL_WORLD
	//! @moos_subscribe GUI_VISIBLE_PARTICLES
	//! @moos_subscribe GUI_VISIBLE_BUMPERS, GUI_VISIBLE_WIFI
	//! @moos_subscribe GUI_VISIBLE_GASMAP_SIMUL
	//! @moos_subscribe GUI_VISIBLE_GAS_MEAN_ONLINE_MAP
	//! @moos_subscribe GUI_VISIBLE_GAS_VAR_ONLINE_MAP
	//! @moos_subscribe GUI_VISIBLE_WIND_ONLINE_MAP

	AddMOOSVariable_OpenMORA("GUI_VISIBLE_PARTICLES", 0);
	AddMOOSVariable_OpenMORA("GUI_VISIBLE_LASER1",  0);
	AddMOOSVariable_OpenMORA("GUI_VISIBLE_LASER2",  0);
	AddMOOSVariable_OpenMORA("GUI_VISIBLE_LASER3",  0);
	AddMOOSVariable_OpenMORA("GUI_VISIBLE_SONAR1",  0);
	AddMOOSVariable_OpenMORA("GUI_VISIBLE_SONAR2",  0);
	AddMOOSVariable_OpenMORA("GUI_VISIBLE_INFRARED1",  0);
	AddMOOSVariable_OpenMORA("GUI_VISIBLE_VIRTUAL_OBS",  0);
	AddMOOSVariable_OpenMORA("GUI_VISIBLE_TOPOLOGICAL_MAP",  0);
	AddMOOSVariable_OpenMORA("GUI_VISIBLE_METRIC_MAP",  0);
	AddMOOSVariable_OpenMORA("GUI_VISIBLE_GASMAP_SIMUL",  0);
	AddMOOSVariable_OpenMORA("GUI_VISIBLE_VIRTUAL_WORLD",  0);
	AddMOOSVariable_OpenMORA("GUI_VISIBLE_BUMPERS",  0);
	AddMOOSVariable_OpenMORA("GUI_VISIBLE_WIFI",  0);
	AddMOOSVariable_OpenMORA("GUI_VISIBLE_DOCK_STATION",  0);
	AddMOOSVariable_OpenMORA("GUI_VISIBLE_GAS_MEAN_ONLINE_MAP",  0);
	AddMOOSVariable_OpenMORA("GUI_VISIBLE_GAS_VAR_ONLINE_MAP",  0);
	AddMOOSVariable_OpenMORA("GUI_VISIBLE_WIND_ONLINE_MAP",  0);

	//! @moos_subscribe GUI_SET_CAMERA
	//! @moos_var GUI_SET_CAMERA Sets the GUI camera parameters for the scene 3D representation, with this format: <br>
	//!       <code>AZIM=12.34,ELEV=45.0,DIST=20.0,X=0,Y=0,Z=0,ANIM=1.0</code>  <br>
	//!   The camera model is that of pointing to a 3D point with a given azimuth, elevation and distance to that point,
	//!   with these fields (all of them are optional, and separated by commas):
	//!     <ul>
	//!     <li> X, Y and Z: Coordinates of the "looking-at" point.</li>
	//!     <li> AZIM: Azimuth in degrees.</li>
	//!     <li> ELEV: Elevation in degrees.</li>
	//!     <li> DIST: Distance of the camera to the "looking-at" points (the "zoom").</li>
	//!     <li> ANIM: In seconds, the time in seconds the transition will take. Default = 0.</li>
	//!     </ul>
	AddMOOSVariable("GUI_SET_CAMERA","GUI_SET_CAMERA","GUI_SET_CAMERA", 0);

	//! @moos_subscribe MORA_GLOBAL_LOG
	//! @moos_var MORA_GLOBAL_LOG Low-traffic text lines intended to be shown to the user in one central GUI from all the modules.
	AddMOOSVariable("MORA_GLOBAL_LOG", "MORA_GLOBAL_LOG", "MORA_GLOBAL_LOG", 0);


	//! @moos_publish ADD_NODE
	AddMOOSVariable_OpenMORA("ADD_NODE", 1.0);

	//! @moos_publish MOVE_NODE
	AddMOOSVariable_OpenMORA("MOVE_NODE", 1.0);

	//! @moos_publish SAVE_GRAPH
	AddMOOSVariable_OpenMORA("SAVE_GRAPH", 1.0);

	//! @moos_publish SAVE_GRAPH
	AddMOOSVariable_OpenMORA("LOAD_GRAPH", 1.0);


	//! @moos_publish NAVIGATE_TARGET
	AddMOOSVariable_OpenMORA("NAVIGATE_TARGET", 1.0);

	//! @moos_subscribe CURRENT_MAP_GRIDMAP
	AddMOOSVariable_OpenMORA("CURRENT_MAP_GRIDMAP", 0);

	//! @moos_subscribe CURRENT_MAP_GAS_SIMUL
	AddMOOSVariable_OpenMORA("CURRENT_MAP_GAS_SIMUL", 0);

	//! @moos_subscribe GAS_MEAN_ONLINE_MAP
	AddMOOSVariable_OpenMORA("GAS_MEAN_ONLINE_MAP", 0);
	//! @moos_subscribe GAS_VAR_ONLINE_MAP
	AddMOOSVariable_OpenMORA("GAS_VAR_ONLINE_MAP", 0);
	//! @moos_subscribe WIND_ONLINE_MAP
	AddMOOSVariable_OpenMORA("WIND_ONLINE_MAP", 0);



	//! @moos_subscribe NAVIGATION_VIRTUAL_OBS
	AddMOOSVariable_OpenMORA("NAVIGATION_VIRTUAL_OBS",  0);

	//! @moos_subscribe IN_EXECUTION
	AddMOOSVariable_OpenMORA("IN_EXECUTION",  0);

	//! @moos_subscribe GUI_POS_SIZE
	AddMOOSVariable_OpenMORA("GUI_POS_SIZE",  0);

	//! @moos_subscribe ALL_NODES, ALL_ARCS
	AddMOOSVariable_OpenMORA("ALL_NODES", 0);
	AddMOOSVariable_OpenMORA("ALL_ARCS",  0);

	AddMOOSVariable_OpenMORA("GRAPH",  0);

	AddMOOSVariable_OpenMORA("BATTERY_V",0);
	AddMOOSVariable_OpenMORA("Battery_Level",0);
	AddMOOSVariable_OpenMORA("Is_Charging",0);
	AddMOOSVariable_OpenMORA("Battery1_Voltage",0);

	AddMOOSVariable_OpenMORA("FBUMPERS",0);
	AddMOOSVariable_OpenMORA("WIFI_POWER",0);
	AddMOOSVariable_OpenMORA("DOCK_STATION",0);
	AddMOOSVariable_OpenMORA("LOCAL_MAP_LIMITS",0);

	AddMOOSVariable_OpenMORA("PLAN_FINISHED",0);


	RegisterMOOSVariables();
	RegisterMOOSVariables_OpenMORA();

    return true;
}

