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


#ifndef ROBOTGUI2009_GUIDESIGNMAIN_H
#define ROBOTGUI2009_GUIDESIGNMAIN_H

#include "StateChartsEdit/PaintBoard.h"
#include <MOOS/libMOOS/App/MOOSApp.h>
#include <COpenMORAMOOSApp.h>

#include <mrpt/opengl/CPointCloud.h>
#include <mrpt/opengl/CPlanarLaserScan.h>
#include <mrpt/opengl/CText.h>
#include <mrpt/opengl/CDisk.h>
#include <mrpt/slam/COccupancyGridMap2D.h>
#include <mrpt/slam/CSimplePointsMap.h>


//(*Headers(RobotGUI2009_guidesignFrame)
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/checklst.h>
#include <wx/checkbox.h>
#include <wx/things/toggle.h>
#include <wx/splitter.h>
#include <wx/listbox.h>
#include <wx/statline.h>
#include "MyGLCanvas.h"
#include <wx/panel.h>
#include <wx/grid.h>
#include <wx/frame.h>
#include <wx/timer.h>
#include <wx/statusbr.h>
#include <wx/things/mathplot.h>
#include <wx/things/LCDWindow.h>



//*)



class RobotGUI2009_guidesignApp;
class GUIMOOSApp;

//JL: Funny conflict with X.h defines:
#ifdef Button1
#undef Button1
#endif
#ifdef Button2
#undef Button2
#endif
#ifdef Button3
#undef Button3
#endif


class RobotGUI2009_guidesignFrame: public wxFrame
{
	friend class GUIMOOSApp;

    public:

        RobotGUI2009_guidesignFrame(RobotGUI2009_guidesignApp* theApp, wxWindow* parent,wxWindowID id = -1);
        virtual ~RobotGUI2009_guidesignFrame();


        /**  The state of the cursor onto the 3D view:
          */
        enum TCursorPickState
        {
        	cpsNone = 0,
        	cpsReactivePickTarget,
        	cpsRelocalizePickArea,
			cpsNewNode,
			cpsEditNode,
			cpsPlaceRobot
        };
		wxPanel *panel_SCE;
        PaintBoard *boardSCE;
		wxPanel *simplePanel;
		wxPanel *battery_management;
		mpWindow        *bat_plot1;
		mpWindow        *bat_plot2;

		mpFXYVector     *bat_data;
		std::vector<double> data_x,data_y;

		mpFXYVector     *bat_data2;
		std::vector<double> data2_x,data2_y;

		kwxLCDDisplay *lcdtext1;

   private:


   // NEW STATE CHART EDITOR
        // controls
        wxButton *stateButtonSCE, *linkButtonSCE, *saveButtonSCE, *saveasButtonSCE, *pubButtonSCE, *openButtonSCE;


        static const long BTN_STATE;
        static const long BTN_LINK;
        static const long pBTN_SAVE;
        static const long BTN_SAVEAS;
        static const long BTN_PUB;
        static const long pBTN_OPEN;

        // button handlers
        void OnStateButton(wxCommandEvent & event);
        void OnLinkButton(wxCommandEvent & event);
        void OnSaveButton(wxCommandEvent & event);
        void OnSaveAsButton(wxCommandEvent & event);
        void OnQuitButton(wxCommandEvent & event);
        void OnPublishButton(wxCommandEvent & event);
        void OnOpen(wxCommandEvent & event);

        std::vector<std::string> actionList;
   // END OF STATE CHART EDITOR

  // SIMPLE GUI PANEL
		// controls
		wxButton *mission1, *mission2, *mission3, *mission4, *mission5, *mission6, *simple_stop, *simple_expert;


		static const long BTN_M1;
		static const long BTN_M2;
		static const long BTN_M3;
		static const long BTN_M4;
		static const long BTN_M5;
		static const long BTN_M6;
		static const long BTN_SIMPLE_STOP;
		static const long BTN_SIMPLE_EXPERT;

		// button handlers
		void OnMission1Button(wxCommandEvent & event);
		void OnMission2Button(wxCommandEvent & event);
		void OnMission3Button(wxCommandEvent & event);
		void OnMission4Button(wxCommandEvent & event);
		void OnMission5Button(wxCommandEvent & event);
		void OnMission6Button(wxCommandEvent & event);
		void OnSimpleStopButton(wxCommandEvent & event);
		void OnSimpleExpertButton(wxCommandEvent & event);
		void turnAllSimpleButtonsOff();
		std::vector<std::string> missionList;

  // END OF SIMPLE GUI PANEL


        //(*Handlers(RobotGUI2009_guidesignFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnbtnEnableJoyClick(wxCommandEvent& event);
        void OnReactiveNavCmd(wxCommandEvent& event);
        void OnStopNavigator(wxCommandEvent& event);
        void OnSendResetSimulator(wxCommandEvent& event);
        void OntimUpdateFromMOOSTrigger(wxTimerEvent& event);
        void OnBtnRelocalizeWithPF(wxCommandEvent& event);
        void OncblistViewEnablesToggled(wxCommandEvent& event);
        void OnbtnZoomInClick(wxCommandEvent& event);
        void OnbtnZoomOutClick(wxCommandEvent& event);
        void OntimInitTrigger(wxTimerEvent& event);
        void OncbCameraFollowsRobotClick(wxCommandEvent& event);
        void OnResize(wxSizeEvent& event);
		void OnMove(wxMoveEvent& event);
        void OnPanel1Paint(wxPaintEvent& event);
        void OnPanel2Paint(wxPaintEvent& event);
        void OnNotebook2PageChanged(wxNotebookEvent& event);
        void OnNotebook1PageChanged(wxNotebookEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnNotebook2PageChanged1(wxNotebookEvent& event);
        void OnbtnNewNodeClick(wxCommandEvent& event);
        void OnbtnEditNodeClick(wxCommandEvent& event);
        void OnbtnSaveTopoClick(wxCommandEvent& event);

        void OnbtnLoadTopoClick(wxCommandEvent& event);
        void OnbtnPlaceRobotClick(wxCommandEvent& event);
        //*)

		void OnplotMouseMove(wxMouseEvent& event);
		void OnplotMouseClick(wxMouseEvent& event);
        void OnTimerUpdateListModules(wxTimerEvent& event);
        void OnTimerDoCameraAnimation(wxTimerEvent& event);

        //(*Identifiers(RobotGUI2009_guidesignFrame)
        static const long ID_BUTTON4;
        static const long ID_BUTTON3;
        static const long ID_STATICLINE2;
        static const long ID_BTN_JOYSTICK;
        static const long ID_BUTTON1;
        static const long ID_BUTTON2;
        static const long ID_STATICLINE1;
        static const long ID_BUTTON5;
        static const long ID_BUTTON6;
        static const long ID_STATICLINE3;
        static const long ID_BUTTON11;
        static const long ID_BUTTON10;
        static const long ID_BUTTON8;
        static const long ID_BUTTON9;
        static const long ID_BUTTON12;
        static const long ID_STATICLINE4;
        static const long ID_BUTTON7;
        static const long ID_PANEL1;
        static const long ID_STATICTEXT3;
        static const long ID_LISTBOX1;
        static const long ID_GRID1;
        static const long ID_PANEL4;
        static const long ID_CHECKLISTBOX1;
        static const long ID_CHECKBOX1;
        static const long ID_PANEL8;
        static const long ID_PANEL7;
        static const long ID_NOTEBOOK1;
        static const long ID_PANEL2;
        static const long ID_STATICTEXT2;
        static const long ID_GLOBAL_GLCANVAS;
        static const long ID_PANEL5;
        static const long ID_STATICTEXT1;
        static const long ID_STATICTEXT4;
        static const long ID_TEXTCTRL1;
        static const long ID_PANEL6;
        static const long ID_SPLITTERWINDOW1;
        static const long ID_PANEL3;
        static const long ID_SPLITTERWINDOW2;
        static const long ID_PANEL9;
        static const long ID_PANEL10;
		static const long ID_PANEL11;
		static const long ID_PANEL12;
        static const long ID_NOTEBOOK2;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        static const long ID_TIMER1;
        static const long ID_TIMER2;
		static const long ID_BUTTONBATTERY;

        //*)

		static const long ID_TIMER_LIST_MODULES;
		static const long ID_TIMER_DO_ANIM_CAMERA;

        //(*Declarations(RobotGUI2009_guidesignFrame)
        wxTimer timInit;
        wxPanel* Panel5;
        wxNotebook* Notebook1;
        wxCustomButton* btnNewNode;
        wxTimer timUpdateFromMOOS;
        wxStaticText* StaticText2;
        wxPanel* Panel4;
        wxCustomButton* btnJoystickMode;
        wxCustomButton* btnResetSimul;
        wxSplitterWindow* SplitterWindow2;
        wxStaticText* StaticTextBattery;
        wxPanel* Panel9;
        wxListBox* lstCurPlan;
        wxPanel* Panel8;
        wxCustomButton* btnQuit;
        wxPanel* Panel1;
        wxStaticText* StaticText3;
        wxCustomButton* btnSaveTopo;
		wxCustomButton* btnBattery;
        wxPanel* Panel6;
        wxPanel* Panel3;
        wxStaticLine* StaticLine4;
        wxStaticLine* StaticLine2;
        wxCustomButton* btnRelocalize;
        wxCustomButton* btnZoomOut;
        wxCheckListBox* cblistViewEnables;
        wxPanel* Panel7;
        wxStatusBar* StatusBar1;
        wxStaticLine* StaticLine3;
        wxStaticLine* StaticLine1;
        wxCustomButton* btnLoadTopo;
        wxCustomButton* btnNavStop;
        wxCustomButton* btnPlaceRobot;
        wxCustomButton* btnZoomIn;
        wxPanel* Panel2;
        wxTextCtrl* edSystemLog;
        wxPanel* Panel10;
        CMyGLCanvas* m_glMap;
        wxNotebook* Notebook2;
        wxSplitterWindow* SplitterWindow1;
        wxCheckBox* cbCameraFollowsRobot;
        wxStaticText* StaticText4;
        wxCustomButton* btnNavCmd;
        wxGrid* gridRunningMods;
        wxCustomButton* btnEditNode;
        //*)

        wxTimer timUpdateListOfConnectedModules;
        wxTimer timDoCameraAnim;

        RobotGUI2009_guidesignApp *m_theWxApp;

        /**  The composite 3D object that represents the robot + its sensors.
          *  It can be acccessed directly thru a smart pointer (not thru its parent scene) safely as long
          *  as it's modified from the main wx thread.
          */
        mrpt::opengl::CSetOfObjectsPtr 						m_gl_robot;  //!< The group of robot 3D objects
		CPose2D												newP;		//pose of the robot
        mrpt::opengl::CSetOfObjectsPtr 						m_gl_robot_model;  //!< The 3D robot itself, which goes into m_gl_robot
		std::vector<mrpt::slam::CObservation2DRangeScanPtr>	m_scan;		//To reduce the delay effect
		std::vector<mrpt::opengl::CPlanarLaserScanPtr> 		m_gl_robot_lasers;
		//mrpt::opengl::CPointCloudColouredPtr 				m_gl_robot_kinect;
		mrpt::slam::CSimplePointsMap						m_theMap;	//To reduce the delay effect (kinect points)
		mrpt::opengl::CPointCloudPtr 						m_gl_robot_kinect;
		mrpt::opengl::CPlanarLaserScanPtr					m_gl_robot_kinect_2D_scan;
        mrpt::opengl::CPointCloudPtr 						m_gl_parts;
		std::vector<mrpt::opengl::CSetOfObjectsPtr> 		m_gl_sonars;  //!< Sonars and IRs
		mrpt::opengl::CPointCloudPtr						m_gl_gridpoints;

        mrpt::opengl::CSetOfObjectsPtr 		m_gl_metricmap;
        mrpt::opengl::CSetOfObjectsPtr 		m_gl_gasmap_simul;
		mrpt::opengl::CSetOfObjectsPtr 		m_gl_gas_mean_online_map;
		mrpt::opengl::CSetOfObjectsPtr 		m_gl_gas_var_online_map;
		mrpt::opengl::CSetOfObjectsPtr 		m_gl_wind_online_map;
        mrpt::opengl::CSetOfObjectsPtr 		m_gl_virtual_obstacles;
		mrpt::opengl::CSetOfObjectsPtr		m_gl_virtual_map;
        mrpt::opengl::CSetOfObjectsPtr 		m_gl_topological_map;
		mrpt::opengl::CRenderizablePtr		m_gl_selected_topo_node;
		mrpt::opengl::CSetOfObjectsPtr 		m_gl_bumpers;
		mrpt::opengl::CSetOfObjectsPtr 		m_gl_wifi;
		mrpt::opengl::CSetOfObjectsPtr 		m_gl_dock_station;
		mrpt::opengl::CSetOfObjectsPtr		m_gl_local_map_limits;


        mrpt::opengl::CSetOfObjectsPtr 		m_gl_current_nav_target;
        mrpt::opengl::CSetOfObjectsPtr 		m_gl_placing_nav_target;
        mrpt::opengl::CSetOfObjectsPtr 		m_gl_relocalizing_area;

        mrpt::slam::COccupancyGridMap2D		m_curmap_grid; //!< The last grid map received from wherever.

		mrpt::opengl::CTextPtr				m_gl_battery_level_text;


        void processNewMOOSVar(const std::string &var, const std::string &value);

        void sendReactiveNavTargetToMOOS(const mrpt::math::TPoint2D &trg);
		void sendRelocalizeCommand(const mrpt::math::TPoint2D &trg);


		//battery management
		bool charging;
		///topology
		void paint_nodes(std::string value);
		void paint_arcs(std::string value);

		void sendTopologyAddNodeToMOOS(const mrpt::math::TPoint2D &trg);
		bool node_selected;
		void selectNode(const mrpt::math::TPoint2D &trg);
		void sendTopologyEdditNodeToMOOS(const mrpt::math::TPoint2D &trg);

		void addObjectToListOfVisibleCheckboxs(const std::string &display_name, const mrpt::opengl::CRenderizablePtr &gl_obj, bool default_visible);

        bool m_refresh_3d;  //!< set, if needed, by processNewMOOSVar
		bool m_ignoreClicksVisibleCheckBox;


		mrpt::math::TPoint2D	m_curCursorPos; //!< The last point the user passes the cursor on.

        TCursorPickState  m_cursorPickState;   //!< The state of the cursor onto the 3D view:

		double RELOCALIZ_AREA_WIDTH;

        struct TInfoPerRunningModule
        {
        	double upTime; //!< Up-time, in seconds.
        	bool   appError; //!< If there has been an error running the module.
        };
        std::map<std::string,TInfoPerRunningModule>  m_modules_ping_info;	//!< The info that each module sends while it's alive. Collected in "process_new_vars.cpp"

        std::map<std::string, mrpt::opengl::CRenderizablePtr> m_mapNames2GlObjects;  //!< For the list of checkboxes visible/invisible.

        // Vars for 3D  camera animation on MOOS var "GUI_SET_CAMERA"
        mrpt::opengl::CCameraPtr 	m_camanim_old, m_camanim_new;
        mrpt::system::TTimeStamp	m_camanim_timStart;
        double						m_camanim_animLength_secs;

		DECLARE_EVENT_TABLE()
};

#endif // ROBOTGUI2009_GUIDESIGNMAIN_H
