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
#include "RobotGUI2009_guidesignMain.h"

#include <mrpt/opengl/CCylinder.h>
#include <mrpt/opengl/CSphere.h>
#include <mrpt/opengl/CSetOfObjects.h>
#include <mrpt/opengl/CSetOfLines.h>
#include <mrpt/opengl/CSimpleLine.h>

#include <mrpt/obs/CObservation2DRangeScan.h>
#include <mrpt/obs/CObservation3DRangeScan.h>
#include <mrpt/obs/CObservationRange.h>
#include <mrpt/obs/CObservationBatteryState.h>

#include <mrpt/utils/CFileGZInputStream.h>

#include <wx/string.h>
#include <wx/intl.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/artprov.h>
#include <wx/colour.h>


#define TOPO_NODES_HEIGHT  0.0


using namespace std;
using namespace mrpt;
using namespace mrpt::synch;
using namespace mrpt::math;
using namespace mrpt::utils;
using namespace mrpt::poses;
using namespace mrpt::opengl;
using namespace mrpt::obs;
using namespace mrpt::maps;



void RobotGUI2009_guidesignFrame::processNewMOOSVar(const std::string &var, const std::string &value)
{

	if (var=="LOCALIZATION")
	{

		//CPose2D   newP; //-->it would be nice to globally access to the pose of the robot (cga)
		newP.fromString(value);
		m_gl_robot->setPose(newP);
		wxRect rect;
		std::ostringstream s;

		if (this->cbCameraFollowsRobot->IsChecked())
		{
			this->m_glMap->cameraPointingX = newP.x();
			this->m_glMap->cameraPointingY = newP.y();
			this->m_glMap->cameraAzimuthDeg = RAD2DEG(newP.phi()+PI);
			this->m_glMap->cameraZoomDistance = 20;
			this->m_glMap->cameraElevationDeg = 90;
		}

		if(m_glMap)
		{
			rect = m_glMap->GetScreenRect();
			s << rect.GetX() <<','<< rect.GetY() <<','<< rect.GetWidth() <<','<< rect.GetHeight() << endl;
			m_theWxApp->m_lstToSendToMOOS.push( make_pair<string,string>("GUI_POS_SIZE",s.str().c_str()));
		}

		m_refresh_3d=true;
	}
	else if (var=="LOCALIZATION_COV")
	{
//		CMatrixDouble COV;
//		if (COV.fromMatlabStringFormat(value))
//		{
//			// ...
//		}
	}
	else if (var=="LOCALIZATION_PARTICLES")
	{
		CMatrixDouble parts_display;
		if (parts_display.fromMatlabStringFormat(value) &&
			size(parts_display,1)>0 && size(parts_display,2)==3 )
		{
			m_gl_parts->setPose( CPose3D(0,0,0.04,0,0,0) );
			m_gl_parts->setPointSize(3.0);
			m_gl_parts->clear();
			m_gl_parts->setColor(0,0,1);

			const size_t N =size(parts_display,1);
			for(size_t i=0;i<N;++i)
				m_gl_parts->insertPoint( parts_display.get_unsafe(i,0),parts_display.get_unsafe(i,1),0);

			m_refresh_3d = true;
		}
	}
	else if(var=="CURRENT_MAP_GRIDMAP")
	{
		CFileGZInputStream(value) >> m_curmap_grid;

		// Update the image in the 3D:
		m_gl_metricmap->clear();
		m_curmap_grid.getAs3DObject(m_gl_metricmap);

		m_refresh_3d = true;
	}
	else if(var=="CURRENT_MAP_GAS_SIMUL")
	{
		if (m_gl_gasmap_simul->isVisible() )
		{
			CFileGZInputStream fil(value);
			fil.ReadObject(m_gl_gasmap_simul.pointer());

			// Update the image in the 3D:
			m_refresh_3d = true;
		}
	}
	else if(var=="GAS_MEAN_ONLINE_MAP")
	{
		if ( m_gl_gas_mean_online_map->isVisible() )
		{
			CFileGZInputStream fil(value);
			fil.ReadObject(m_gl_gas_mean_online_map.pointer());

			// Update the image in the 3D:
			m_refresh_3d = true;
		}
	}
	else if(var=="GAS_VAR_ONLINE_MAP")
	{
		if ( m_gl_gas_var_online_map->isVisible() )
		{
			CFileGZInputStream fil(value);
			fil.ReadObject(m_gl_gas_var_online_map.pointer());

			// Update the image in the 3D:
			m_refresh_3d = true;
		}
	}

	else if(var=="WIND_ONLINE_MAP")
	{
		if ( m_gl_wind_online_map->isVisible() )
		{
			CFileGZInputStream fil(value);
			fil.ReadObject(m_gl_wind_online_map.pointer());

			// Update the image in the 3D:
			m_refresh_3d = true;
		}
	}

	else if(var=="LASER1" || var=="LASER2" || var=="LASER3")
	{
		const unsigned int idx = atoi(&var[5])-1;  // LASER1 -> 0, etc...

		if (idx<m_gl_robot_lasers.size() && m_gl_robot_lasers[idx]->isVisible())
		{
			mrpt::utils::CSerializablePtr obj;
			//mrpt::utils::StringToObject(value, obj);  (deprecated) CGenericSensor now employs ObjectToOctetVector() to serialize objects to strings
			mrpt::utils::RawStringToObject(value, obj);
			if (IS_CLASS(obj, CObservation2DRangeScan))
			{
				if (m_scan[idx].present())
					m_gl_robot_lasers[idx]->setScan(*m_scan[idx]);
				m_scan[idx] = CObservation2DRangeScanPtr(obj);

				m_refresh_3d = true;
			}
		}
	}
	else if(var=="KINECT1")
	{
		if (m_gl_robot_kinect->isVisible())
		{
			CSerializablePtr obj;
			mrpt::utils::StringToObject(value, obj);
			if (IS_CLASS(obj, CSimplePointsMap))
			{
				if (m_theMap.size() != 0)
				{
					m_gl_robot_kinect->clear();
					m_gl_robot_kinect->loadFromPointsMap( &m_theMap );
				}
				m_theMap = *CSimplePointsMapPtr(obj);

				m_refresh_3d = true;
			}
		}
	}
	else if (var == "GRIDPOINTS")
	{
		if (m_gl_gridpoints->isVisible())
		{
			CSerializablePtr obj;
			mrpt::utils::StringToObject(value, obj);
			if (IS_CLASS(obj, CSimplePointsMap))
			{
				mrpt::maps::CSimplePointsMap gridmap;
				gridmap = *CSimplePointsMapPtr(obj);
				m_gl_gridpoints->clear();
				m_gl_gridpoints->loadFromPointsMap( &gridmap );

				m_refresh_3d = true;
			}
		}
	}
	//	if (IS_CLASS(obj, CObservation3DRangeScan))
	//	{
	//		CObservation3DRangeScanPtr scan = CObservation3DRangeScanPtr(obj);
	//		CSimplePointsMap theMap;
	//		theMap.insertObservationPtr( scan );
	//		m_gl_robot_kinect->clear();
	//		m_gl_robot_kinect->loadFromPointsMap( &theMap );
	//		m_gl_robot_kinect->setColor( 0, 1, 0 );
	//		m_gl_robot_kinect->setPointSize( 3.0 );

	//		m_refresh_3d = true;
	//	}
	//	else if (IS_CLASS(obj, CObservation2DRangeScan ) )
	//	{
	//		/*CObservation2DRangeScanPtr scan = CObservation2DRangeScanPtr(obj);
	//		CSimplePointsMap theMap;
	//		theMap.insertObservationPtr( scan );
	//		m_gl_robot_kinect->clear();
	//		m_gl_robot_kinect->loadFromPointsMap( &theMap );
	//		m_gl_robot_kinect->setColor( 0, 1, 0 );
	//		m_gl_robot_kinect->setPointSize( 3.0 );

	//		m_refresh_3d = true;*/

	//		m_gl_robot_kinect_2D_scan->setVisibility();

	//		CObservation2DRangeScanPtr scan = CObservation2DRangeScanPtr(obj);
	//		m_gl_robot_kinect_2D_scan->setScan(*scan);
	//		m_refresh_3d = true;
	//	}
	//}
	else if (var=="FBUMPERS")
	{
		size_t n=0;
		/////I know, bumpers belong to the robot and they should be attached to it.
		//but by now and for testing....
		for (;;)
		{
			CRenderizablePtr i = m_gl_bumpers->getByClass<mrpt::opengl::CSphere>();
			if (!i) break; // No more objects
			m_gl_bumpers->removeObject(i);
		}
		do
		{
			n=value.find("1",n+1);
			if (n!=string::npos)
			{
				this->edSystemLog->AppendText( _U((format("Triggered bumper %d\n", (int)n)).c_str() ));
				CSpherePtr b = CSphere::Create();
				b->setRadius(0.01);
				b->setLocation(newP.x()+0.1,newP.y()+(0.025*(n-5)),0.1);
				b->setColor(0,1,0);
				m_gl_bumpers->insert(b);

			}

		}while (n!=string::npos);
		m_refresh_3d = true;

	}
	else if (var=="WIFI_POWER")
	{
		size_t log=0;
		for (;;)
		{
			log++;
			CRenderizablePtr i = m_gl_wifi->getByClass<mrpt::opengl::CDisk>();
			if (!i) break; // No more objects
			if (log>5) m_gl_wifi->removeObject(i);
		}

		float power=atof(value.c_str());
		this->edSystemLog->AppendText( _U((format("Triggered bumper %f\n", power)).c_str() ));

		CDiskPtr d = CDisk::Create();

		d->setDiskRadius(2);
		d->setLocation(newP.x(),newP.y(),0);

		d->setColor(0,0,1,power/150);
		m_gl_wifi->insert(d);

		m_refresh_3d = true;


	}

	else if (var=="DOCK_STATION")
	{
		CPoint3D start_point, end_point;

		// Process the message format
		char buff[128];
		char coord[32];
		strcpy(buff, value.c_str());

		strcpy(coord, strtok(buff,","));
		start_point.x(atof(coord));

		strcpy(coord, strtok(NULL,","));
		start_point.y(atof(coord));

		strcpy(coord, strtok(NULL,","));
		end_point.x(atof(coord));


		strcpy(coord, strtok(NULL,","));
		end_point.y(atof(coord));


		size_t log=0;
		for (;;)
		{
			log++;
			CRenderizablePtr i = m_gl_dock_station->getByClass<mrpt::opengl::CSetOfLines>();
			if (!i) break; // No more objects
			if (log>5) m_gl_dock_station->removeObject(i);
		}

		CSetOfLinesPtr line = CSetOfLines::Create();
		line->appendLine(start_point.x(),start_point.y(),0.0,end_point.x(),end_point.y(),0.0);
		line->setColor(0.0,0.0,0.0);
		line->setLineWidth(2.0);

		m_gl_dock_station->insert(line);

		m_refresh_3d = true;


	}

	else if (var=="LOCAL_MAP_LIMITS")
	{
		if ( m_gl_local_map_limits->isVisible() )
		{

			// Process the message format (x1=x.xxy1=y.yyx2=x.xxy2=y.yy)
			char buff[50];
			char * pch;
			double x1,y1,x2,y2;

			strcpy(buff, value.c_str());
			pch = strtok(buff,"abcd=");

			x1 = atof(pch);
			pch = strtok (NULL, "abcd=");
			y1 = atof(pch);
			pch = strtok (NULL, "abcd=");
			x2 = atof(pch);
			pch = strtok (NULL, "abcd=");
			y2 = atof(pch);

			size_t log=0;
			for (;;)
			{
				log++;
				CRenderizablePtr i = m_gl_local_map_limits->getByClass<mrpt::opengl::CSetOfLines>();
				if (!i) break; // No more objects
				//if (log>5) m_gl_local_map_limits->removeObject(i);
				m_gl_local_map_limits->removeObject(i);
			}

			// Create set of lines (square)
			CSetOfLinesPtr soLines = CSetOfLines::Create();
			soLines->setColor(0.0,0.0,0.0);
			soLines->setLineWidth(4.0);

			soLines->appendLine(x1,y1,0.0,x1,y2,0.0);
			//m_gl_local_map_limits->insert(soLines);
			soLines->appendLine(x1,y2,0.0,x2,y2,0.0);
			//m_gl_local_map_limits->insert(soLines);
			soLines->appendLine(x2,y2,0.0,x2,y1,0.0);
			//m_gl_local_map_limits->insert(soLines);
			soLines->appendLine(x2,y1,0.0,x1,y1,0.0);
			m_gl_local_map_limits->insert(soLines);

			//m_gl_local_map_limits->insert<CSetOfLines>(soLines->begin(),soLines->end());

			// Update the image in the 3D:
			m_refresh_3d = true;
		}
	}




	else if(var=="SONAR1" || var=="SONAR2" || var=="INFRARED1")
	{
		size_t idx=size_t(-1);
		if (var=="SONAR1") idx=0;
		else if (var=="SONAR2") idx=1;
		else if (var=="INFRARED1") idx=2;

		if (idx<m_gl_sonars.size())
		{
			if (m_gl_sonars[idx]->isVisible())
			{
				CSerializablePtr obj;
				mrpt::utils::StringToObject(value, obj);
				if (IS_CLASS(obj, CObservationRange))
				{
					CObservationRangePtr obs = CObservationRangePtr(obj);

					// Represent sonars in 3D.
					if (m_gl_sonars[idx]->size()!=obs->sensedData.size())
						m_gl_sonars[idx]->clear();

					for (size_t i=0;i<obs->sensedData.size();i++)
					{
						std::string obj = format("S%d",(int)obs->sensedData[i].sensorID);
						mrpt::opengl::CCylinderPtr sonarRange;
						mrpt::opengl::CRenderizablePtr objPtr = m_gl_sonars[idx]->getByName( obj );
						if( !objPtr )
						{
							sonarRange = mrpt::opengl::CCylinder::Create(0.0f,0.0f,1.0f,30,10);
							sonarRange->setName( obj );
							m_gl_sonars[idx]->insert( sonarRange );
						}
						else
							sonarRange = CCylinderPtr( objPtr );

						sonarRange->setRadii( 0, tan( obs->sensorConeApperture )*obs->sensedData[i].sensedDistance );
						sonarRange->setPose( CPose3D(obs->sensedData[i].sensorPose)+CPose3D(0,0,0,0,DEG2RAD(90.0),0) );
						sonarRange->setHeight( obs->sensedData[i].sensedDistance );
						sonarRange->enableShowName();
						sonarRange->setColor( 0.2, 0, 0.8, 0.15 );
					}

					m_refresh_3d = true;
				}
			} // end if is visible
		}
	}
	else if (var=="NAVIGATE_TARGET")
	{
		CMatrixDouble M;
		if (M.fromMatlabStringFormat(value) && size(M,1)==1 && size(M,2)>=2)
		{
			m_gl_current_nav_target->setVisibility(true);
			m_gl_current_nav_target->setLocation(M(0,0),M(0,1),0.10);

			m_refresh_3d = true;
		}
	}
	else if (var.find("_STATUS")!=string::npos)
	{
		/* Example:
		PMOBILEROBOT_SIMUL_STATUS:
		AppErrorFlag=false,Uptime=8.76741,MOOSName=pMobileRobot_Simul,Publishing="LASER1,ODOMETRY,PMOBILEROBOT_SIMUL_STATUS,",Subscribing="LOCALIZATION_PARTICLES,MOTION_CMD_V,MOTION_CMD_W,PMOBILEROBOT_SIMUL_CMD,"
		*/
		size_t nam1,nam2;
		if (string::npos!=(nam1=value.find("MOOSName=")))
		{
			if (string::npos!=(nam2=value.find(",",nam1)))
			{
				const string modName = value.substr(nam1+9,nam2-nam1-9);
				size_t uptim1;
				if (string::npos!=(uptim1=value.find("Uptime=")))
				{
					const double uptim_secs = atof(&value[uptim1+7]);
					const bool app_err = value.find("AppErrorFlag=true")!=string::npos;

					// save all this info:
					TInfoPerRunningModule &inf = m_modules_ping_info[modName];
					inf.appError = app_err;
					inf.upTime   = uptim_secs;
				}
			}
		}
	}
	else if(var=="MORA_GLOBAL_LOG")
	{
		this->edSystemLog->AppendText( _U(( value+string("\n") ).c_str()) );
	}
	else if (var.substr(0,12)=="GUI_VISIBLE_")
	{
		const string varNam = mrpt::system::upperCase(var.substr(12));
		const bool val = ::atoi(value.c_str())!=0;

		for (size_t i=0;i<cblistViewEnables->GetCount();i++)
		{
			const string nam = mrpt::system::upperCase(string( cblistViewEnables->GetString(i).mbc_str() ));
			if (nam==varNam)
				cblistViewEnables->Check(i,val);
		}
		cblistViewEnables->Refresh();
		{
			m_ignoreClicksVisibleCheckBox=true;
			wxCommandEvent ev;
			OncblistViewEnablesToggled(ev);  // Grab the visibility states to the actual m_gl_* objects
			m_ignoreClicksVisibleCheckBox=false;
		}
	}
	else if (var=="NAVIGATION_VIRTUAL_OBS")
	{
		CMatrixDouble M;
		if (M.fromMatlabStringFormat(value) && (size(M,1)==2 || size(M,1)==3) && size(M,2)>=1)
		{
			m_gl_virtual_obstacles->clear();

			mrpt::opengl::CPointCloudPtr cl = mrpt::opengl::CPointCloud::Create();
			const bool M_has_Z = size(M,1)==3;

			const size_t nPts = size(M,2);
			for (size_t i=0;i<nPts;i++)
				cl->insertPoint(M(0,i),M(1,i),M_has_Z ? M(2,i) : 0.0 );

			cl->setPointSize(6.0);
			cl->setColor(0,0.9,0);

			m_gl_virtual_obstacles->insert(cl);
			m_refresh_3d = true;
		}
		else { std::cerr << format("Error: In format of NAVIGATION_VIRTUAL_OBS:\n%s\n",value.c_str()) << "M: "<< M << endl; }
	}
	else if (var=="VIRTUAL_WORLD")
	{

		this->edSystemLog->AppendText(_("Loading virtual world..."));

		CVirtualWorld vw;
		CSetOfObjectsPtr world=CSetOfObjects::Create();
		vw.LoadVirtualWorld("vw.xml");//this file by now. It should be passed as a parameter in the MOOS vble.

		vw.GetWorld(world);

		//it is assummed that the virtual_map only contains a CSetOfObject object

		CRenderizablePtr i = m_gl_virtual_map->getByClass<mrpt::opengl::CSetOfObjects>();
		if (i) m_gl_virtual_map->removeObject(i);

		m_gl_virtual_map->insert(world);

		m_refresh_3d = true;

	}
	else if (var=="ALL_NODES")  // All nodes in a topological map, represented as spheres in "m_gl_topological_map"
	{
		paint_nodes(value);
	}
	else if (var=="ALL_ARCS")  // All arcs in a topological map, represented as lines in "m_gl_topological_map"
	{
		paint_arcs(value);

	}
	else if (var=="GRAPH")
	{
		vector_string toks;
		mrpt::system::tokenize(value,"&",toks);

		if (toks.size()==2)
		{
			paint_nodes(toks[0]);
			paint_arcs(toks[1]);
		}
	}
	else if (var=="IN_EXECUTION")
	{
		lstCurPlan->Clear();

		vector_string toks;
		mrpt::system::tokenize(value,"#",toks);
		lstCurPlan->Append( _U(format("In execution:").c_str()));
		for (size_t i=0;i<toks.size();i++)
		{
			if (i==1) lstCurPlan->Append( _U(format("Global ID [%s]", toks[i].c_str()).c_str() ));
			else if (i==2) lstCurPlan->Append( _U(format("Owner ID [%s]", toks[i].c_str()).c_str() ));
			else lstCurPlan->Append( _U(format("%s\n", toks[i].c_str()).c_str() ));

		}



	}
	else if (var =="BATTERY_V")		//Battery voltage of the robot base
	{
		CSerializablePtr batt_obj;		
		mrpt::utils::RawStringToObject(value,batt_obj);
		double voltios;
		voltios=((mrpt::obs::CObservationBatteryStatePtr)batt_obj)->voltageMainRobotBattery;

		//Update indicator on main viewport
		StaticTextBattery->SetLabel(wxString::Format(_("Battery base: %.2f"),voltios));

		//Update graph (Battery Manager tab, first plot)
		static mrpt::system::TTimeStamp starttime=mrpt::system::getCurrentLocalTime();
		mrpt::system::TTimeStamp tt=mrpt::system::getCurrentLocalTime();
		double elapse=mrpt::system::timeDifference(starttime,tt);

		data_x.push_back(elapse);
		data_y.push_back(voltios);
		bat_data->SetData(data_x,data_y);
		bat_data->SetContinuity(true);
		wxPen vectorpen(*wxBLUE, 0.5, wxSOLID);
		bat_data->SetPen(vectorpen);
		bat_data->SetDrawOutsideMargins(false);
		bat_plot1->Refresh();
		if (data_x.size()<100)
			bat_plot1->Fit();
		else 
			bat_plot1->SetPosX(elapse-20);

		if (voltios<11)
			this->edSystemLog->AppendText( _U(format("Critical base battery level\n").c_str()));

		//Set value on LCD display
		lcdtext1->SetValue( _U(format("%.2f",voltios).c_str()));
		lcdtext1->Refresh();
	}

	else if (var=="BATTERY_IS_CHARGING")	//Battery of the robot base
	{
		if (atof(value.c_str())>0.0)
		{
			btnBattery->SetBitmapLabel(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("BATTERY_CHARGING"))));
			charging=true;
		}
		else
		{
			btnBattery->SetBitmapLabel(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("BATTERY_FULL"))));
			charging=false;			
		}
	}

	else if (var=="BATTERY_MANAGER_V")		//External Battery (monitored by the BatteryManager module)
	{
		CSerializablePtr batt_obj;		
		mrpt::utils::RawStringToObject(value,batt_obj);
		double voltios;
		voltios=((mrpt::obs::CObservationBatteryStatePtr)batt_obj)->voltageMainRobotBattery;

		//Update indicator on main viewport
		StaticTextBattery2->SetLabel(wxString::Format(_("Battery ext: %.2f"),voltios));

		//Update graph (Battery Manager tab, second plot)
		static mrpt::system::TTimeStamp starttime=mrpt::system::getCurrentLocalTime();
		mrpt::system::TTimeStamp tt=mrpt::system::getCurrentLocalTime();
		double elapse=mrpt::system::timeDifference(starttime,tt);

		data2_x.push_back(elapse);
		data2_y.push_back(voltios);
		bat_data2->SetData(data2_x,data2_y);
		bat_data2->SetContinuity(true);
		wxPen vectorpen(*wxBLUE, 0.5, wxSOLID);
		bat_data2->SetPen(vectorpen);
		bat_data2->SetDrawOutsideMargins(false);
		bat_plot2->Refresh();
		if (data2_x.size()<100)
			bat_plot2->Fit();
		else
			bat_plot2->SetPosX(elapse-20);		

		if (voltios<11)
			this->edSystemLog->AppendText( _U(format("Critical external battery  level\n").c_str()));

		//Set value on LCD display
		lcdtext2->SetValue( _U(format("%.2f",voltios).c_str()));
		lcdtext2->Refresh();
	}

	else if (var=="BATTERY_MANAGER_IS_CHARGING")	//External Battery
	{
		if (atof(value.c_str())>0.0)
		{
			btnBattery2->SetBitmapLabel(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("BATTERY_CHARGING"))));
			charging=true;
		}
		else
		{
			btnBattery2->SetBitmapLabel(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("BATTERY_FULL"))));
			charging=false;			
		}
	}

	else if (var=="BATTERY_MANAGER_LEVEL" && !charging)
	{
		if (atof(value.c_str()) >80)
		{
			btnBattery2->SetBitmapLabel(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("BATTERY_FULL"))));
		}
		else if (atof(value.c_str()) >50)
		{
			btnBattery2->SetBitmapLabel(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("BATTERY_L2"))));
		}
		else if (atof(value.c_str()) >30)
		{
			btnBattery2->SetBitmapLabel(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("BATTERY_L1"))));
		}
		else if (atof(value.c_str()) >10)
		{
			btnBattery2->SetBitmapLabel(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("BATTERY_DEAD"))));
		}
	}

	else if (var=="GUI_SET_CAMERA")
	{
		// Format:  AZIM=12.34,ELEV=45.0,DIST=20.0,X=0,Y=0,Z=0,ANIM=1.0

		// Save current camera position and new one, so we can interpolate:
		m_camanim_old->setAzimuthDegrees( m_glMap->cameraAzimuthDeg );
		m_camanim_old->setElevationDegrees( m_glMap->cameraElevationDeg );
		m_camanim_old->setZoomDistance( m_glMap->cameraZoomDistance);
		m_camanim_old->setPointingAt( m_glMap->cameraPointingX,m_glMap->cameraPointingY,m_glMap->cameraPointingZ );

		// Modify current view as requested by the user:
		*m_camanim_new = *m_camanim_old;

		double x;
		if (MOOSValFromString(x,value, "AZIM"))	m_camanim_new->setAzimuthDegrees(x);
		if (MOOSValFromString(x,value, "ELEV"))	m_camanim_new->setElevationDegrees(x);
		if (MOOSValFromString(x,value, "DIST"))	m_camanim_new->setZoomDistance(x);

		x = m_glMap->cameraPointingX;
		double y = m_glMap->cameraPointingY;
		double z = m_glMap->cameraPointingZ;
		MOOSValFromString(x,value, "X");
		MOOSValFromString(y,value, "Y");
		MOOSValFromString(z,value, "Z");
		m_camanim_new->setPointingAt(x,y,z);

		m_camanim_animLength_secs = 0;
		MOOSValFromString(m_camanim_animLength_secs,value, "ANIM");

		m_camanim_timStart = mrpt::system::now();

		// start animation:
		timDoCameraAnim.Start(100, true);

	}
	else if (var=="PLAN_FINISHED")
	{
		// don't know which mission is being executed, so just turn off all of them
		mission1->SetBackgroundColour(wxNullColour);
		mission2->SetBackgroundColour(wxNullColour);
		mission3->SetBackgroundColour(wxNullColour);
		mission4->SetBackgroundColour(wxNullColour);
		mission5->SetBackgroundColour(wxNullColour);
		mission6->SetBackgroundColour(wxNullColour);
	}
	/*else if (var=="AVAILABLE_ACTIONS")
	{
	    // contains a list of available actions separated by a space
        char *actBuff;
        char actChar[1024];
        strcpy(actChar,value.c_str());
        actBuff = strtok(actChar," ");
        while (actBuff)
        {
            actionList.push_back(*(new string(actBuff)));
            actBuff = strtok(NULL," ");
        }
        // set the action list (first, convert it!)
        std::vector<std::string>::iterator action = actionList.begin();
        std::vector<wxString> actions;
        wxString action2;
        while (action!=actionList.end()){
            action2 = wxString::FromAscii((*action).c_str());
            actions.push_back(action2);
            action++;
        }
        boardSCE->setActions(actions);

	}*/
	else
	{ }
}
void RobotGUI2009_guidesignFrame::paint_nodes(const std::string value)
{
// 1st: Remove old nodes, if any:

		for (;;)
		{
			CRenderizablePtr i = m_gl_topological_map->getByClass<mrpt::opengl::CSphere>();
			if (!i) break; // No more objects
			m_gl_topological_map->removeObject(i);
		}

		// 2nd: Parse string & add 3D objects:
		vector_string toks;
		mrpt::system::tokenize(value,"#",toks);

		for (size_t i=0;i<toks.size();i++)
		{

			// format: node_name node_posx node_posy
			vector_string vals;
			mrpt::system::tokenize(toks[i]," ",vals);
			if (vals.size()==3 && toks[i]!="Robot")
			{
				CSpherePtr sp = CSphere::Create();
				sp->setLocation(atof(vals[1].c_str()),atof(vals[2].c_str()), TOPO_NODES_HEIGHT );
				sp->setColor(1,0,0);
				sp->setRadius(0.1);
				sp->enableShowName(true);
				sp->setName(vals[0]);
				m_gl_topological_map->insert(sp);
			}
			else if (toks[i]!="Robot")
				this->edSystemLog->AppendText( _U(format("Error parsing ALL_NODES string: %s\n", toks[i].c_str()).c_str() ) );
		}

		m_refresh_3d = true;
}

void RobotGUI2009_guidesignFrame::paint_arcs(const std::string value)
{
	// 1st: Remove old arcs, if any:
		for (;;)
		{
			CRenderizablePtr i = m_gl_topological_map->getByClass<mrpt::opengl::CSimpleLine>();
			if (!i) break; // No more objects
			m_gl_topological_map->removeObject(i);
		}

		// 2nd: Parse string & add 3D objects:
		vector_string toks;
		mrpt::system::tokenize(value,"#",toks);

		for (size_t i=0;i<toks.size();i++)
		{
			// format: node_origin_name node_destination_name arc_type
			vector_string vals;
			mrpt::system::tokenize(toks[i]," ",vals);
			if (vals.size()==3)
			{
				const string nam1 = vals[0];
				const string nam2 = vals[1];

				CRenderizablePtr o1 = m_gl_topological_map->getByName(nam1);
				CRenderizablePtr o2 = m_gl_topological_map->getByName(nam2);

				if (o1 && o2 && vals[0]!="Robot")
				{
					CSimpleLinePtr sl = CSimpleLine::Create();
					sl->setLineCoords(
						o1->getPoseX(),o1->getPoseY(),o1->getPoseZ(),
						o2->getPoseX(),o2->getPoseY(),o2->getPoseZ() );
					if (vals[2] == "Location")
					{
						sl->setColor(1,0,1);
						sl->setLineWidth(1.0);
					}
					else
					{
						sl->setColor(0,1,1);
						sl->setLineWidth(2.0);
					}
					m_gl_topological_map->insert(sl);
				}
				else if (vals[0]=="Robot" && o2)
				{
					o2->setColor(0,0,1);
				}
				else
					this->edSystemLog->AppendText( _U(format("Error parsing ALL_ARCS string: Node names not found as nodes: %s\n", toks[i].c_str()).c_str() ) );
			}
			else
				this->edSystemLog->AppendText( _U(format("Error parsing ALL_ARCS string: %s\n", toks[i].c_str()).c_str() ) );
		}
		m_refresh_3d = true;
}
