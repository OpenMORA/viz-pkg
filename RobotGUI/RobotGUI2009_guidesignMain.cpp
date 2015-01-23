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
#include <wx/colour.h>
#include "RobotGUI2009_guidesignApp.h"
#include "RobotGUI2009_guidesignMain.h"
#include "CMapirArtProvider.h"

#include <mrpt/math/interp_fit.h>
#include <mrpt/opengl/CSetOfLines.h>
#include <mrpt/opengl/CGridPlaneXY.h>
#include <mrpt/opengl/CSphere.h>
#include <mrpt/opengl/stock_objects.h>

//(*InternalHeaders(RobotGUI2009_guidesignFrame)
#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/tglbtn.h>
#include <wx/settings.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/image.h>
#include <wx/string.h>
//*)
//#include "StateChartsGUI/Main.h"


using namespace std;
using namespace mrpt;
using namespace mrpt::synch;
using namespace mrpt::math;
using namespace mrpt::utils;
using namespace mrpt::poses;
using namespace mrpt::opengl;


//(*IdInit(RobotGUI2009_guidesignFrame)
const long RobotGUI2009_guidesignFrame::ID_BUTTON4 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_BUTTON3 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_STATICLINE2 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_BTN_JOYSTICK = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_BUTTON1 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_BUTTON2 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_STATICLINE1 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_BUTTON5 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_BUTTON6 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_STATICLINE3 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_BUTTON11 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_BUTTON10 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_BUTTON8 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_BUTTON9 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_BUTTON12 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_STATICLINE4 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_BUTTON7 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_PANEL1 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_STATICTEXT3 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_LISTBOX1 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_GRID1 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_PANEL4 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_CHECKLISTBOX1 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_CHECKBOX1 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_PANEL8 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_PANEL7 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_NOTEBOOK1 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_PANEL2 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_STATICTEXT2 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_GLOBAL_GLCANVAS = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_PANEL5 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_STATICTEXT1 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_STATICTEXT5 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_STATICTEXT4 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_TEXTCTRL1 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_PANEL6 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_SPLITTERWINDOW1 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_PANEL3 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_SPLITTERWINDOW2 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_PANEL9 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_PANEL10 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_PANEL11 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_PANEL12 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_NOTEBOOK2 = wxNewId();
const long RobotGUI2009_guidesignFrame::idMenuQuit = wxNewId();
const long RobotGUI2009_guidesignFrame::idMenuAbout = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_STATUSBAR1 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_TIMER1 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_TIMER2 = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_BUTTONBATTERY = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_BUTTONBATTERY2 = wxNewId();


// NEW STATE CHARTS EDITOR
const long RobotGUI2009_guidesignFrame::BTN_STATE = wxNewId();
const long RobotGUI2009_guidesignFrame::BTN_LINK = wxNewId();
const long RobotGUI2009_guidesignFrame::pBTN_SAVE = wxNewId();
const long RobotGUI2009_guidesignFrame::BTN_PUB = wxNewId();
const long RobotGUI2009_guidesignFrame::pBTN_OPEN = wxNewId();
const long RobotGUI2009_guidesignFrame::BTN_SAVEAS = wxNewId();
//*)

// SIMPLE GUI PANEL
const long RobotGUI2009_guidesignFrame::BTN_M1 = wxNewId();
const long RobotGUI2009_guidesignFrame::BTN_M2 = wxNewId();
const long RobotGUI2009_guidesignFrame::BTN_M3 = wxNewId();
const long RobotGUI2009_guidesignFrame::BTN_M4 = wxNewId();
const long RobotGUI2009_guidesignFrame::BTN_M5 = wxNewId();
const long RobotGUI2009_guidesignFrame::BTN_M6 = wxNewId();
const long RobotGUI2009_guidesignFrame::BTN_SIMPLE_STOP = wxNewId();
const long RobotGUI2009_guidesignFrame::BTN_SIMPLE_EXPERT = wxNewId();
// END SIMPLE GUI PANEL

const long RobotGUI2009_guidesignFrame::ID_TIMER_LIST_MODULES = wxNewId();
const long RobotGUI2009_guidesignFrame::ID_TIMER_DO_ANIM_CAMERA = wxNewId();

BEGIN_EVENT_TABLE(RobotGUI2009_guidesignFrame,wxFrame)
    //(*EventTable(RobotGUI2009_guidesignFrame)
	EVT_SIZE    (RobotGUI2009_guidesignFrame::OnResize)
	EVT_MOVE    (RobotGUI2009_guidesignFrame::OnMove)
    //*)
END_EVENT_TABLE()

RobotGUI2009_guidesignFrame::RobotGUI2009_guidesignFrame(RobotGUI2009_guidesignApp* theApp,wxWindow* parent,wxWindowID id) :
	m_theWxApp(theApp),
	m_ignoreClicksVisibleCheckBox(false),
	m_curCursorPos(0,0),
	m_cursorPickState(cpsNone),
	RELOCALIZ_AREA_WIDTH( 4.0 ),
	m_camanim_old( mrpt::opengl::CCamera::Create() ),
	m_camanim_new( mrpt::opengl::CCamera::Create() ),
	m_glMap(NULL)
{
	wxArtProvider::Push(new CMapirArtProvider);

    //(*Initialize(RobotGUI2009_guidesignFrame)
    wxFlexGridSizer* FlexGridSizer4;
    wxMenuItem* MenuItem2;
    wxFlexGridSizer* FlexGridSizer10;
    wxFlexGridSizer* FlexGridSizer3;
    wxMenuItem* MenuItem1;
    wxFlexGridSizer* FlexGridSizer5;
    wxFlexGridSizer* FlexGridSizer9;
    wxFlexGridSizer* FlexGridSizer2;
    wxMenu* Menu1;
    wxFlexGridSizer* FlexGridSizer7;
    wxFlexGridSizer* FlexGridSizer8;
//    wxFlexGridSizer* FlexGridSizer12;
    wxMenuBar* MenuBar1;
    wxFlexGridSizer* FlexGridSizer6;
    wxFlexGridSizer* FlexGridSizer1;
    wxFlexGridSizer* FlexGridSizer11;
	wxFlexGridSizer* FlexGridSizer13;
    wxMenu* Menu2;

    Create(parent, wxID_ANY, _("Robot GUI 2009 - MAPIR Lab"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(1007,461));
    FlexGridSizer1 = new wxFlexGridSizer(1, 1, 0, 0);
    FlexGridSizer1->AddGrowableCol(0);
    FlexGridSizer1->AddGrowableRow(0);
    Notebook2 = new wxNotebook(this, ID_NOTEBOOK2, wxDefaultPosition, wxSize(1007,520), 0, _T("ID_NOTEBOOK2"));
    Panel9 = new wxPanel(Notebook2, ID_PANEL9, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL9"));
    FlexGridSizer11 = new wxFlexGridSizer(2, 1, 0, 0);
    FlexGridSizer11->AddGrowableCol(0);
    FlexGridSizer11->AddGrowableRow(1);
    Panel1 = new wxPanel(Panel9, ID_PANEL1, wxDefaultPosition, wxSize(409,20), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    FlexGridSizer10 = new wxFlexGridSizer(1, 21, 0, 0);
    btnResetSimul = new wxCustomButton(Panel1,ID_BUTTON4,_("Reset simul..."),wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("ICON_RESET")),wxART_MAKE_CLIENT_ID_FROM_STR(wxString(wxEmptyString))),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_BUTTON|wxCUSTBUT_BOTTOM|wxCUSTBUT_FLAT,wxDefaultValidator,_T("ID_BUTTON4"));
    btnResetSimul->SetBitmapDisabled(btnResetSimul->CreateBitmapDisabled(btnResetSimul->GetBitmapLabel()));
    btnResetSimul->SetLabelMargin(wxSize(10,6));
    btnResetSimul->SetBitmapMargin(wxSize(6,6));
    FlexGridSizer10->Add(btnResetSimul, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
    btnRelocalize = new wxCustomButton(Panel1,ID_BUTTON3,_("Relocalize..."),wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("ICON_RELOCALIZE")),wxART_MAKE_CLIENT_ID_FROM_STR(wxString(wxEmptyString))),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_BUTTON|wxCUSTBUT_BOTTOM|wxCUSTBUT_FLAT,wxDefaultValidator,_T("ID_BUTTON3"));
    btnRelocalize->SetBitmapDisabled(btnRelocalize->CreateBitmapDisabled(btnRelocalize->GetBitmapLabel()));
    btnRelocalize->SetLabelMargin(wxSize(10,-1));
    btnRelocalize->SetBitmapMargin(wxSize(20,6));
    FlexGridSizer10->Add(btnRelocalize, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
    StaticLine2 = new wxStaticLine(Panel1, ID_STATICLINE2, wxDefaultPosition, wxSize(2,-1), wxLI_VERTICAL, _T("ID_STATICLINE2"));
    FlexGridSizer10->Add(StaticLine2, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
    btnJoystickMode = new wxCustomButton(Panel1,ID_BTN_JOYSTICK,_("Joystick mode"),wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("ICON_JOY")),wxART_MAKE_CLIENT_ID_FROM_STR(wxString(wxEmptyString))),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_BUTTON|wxCUSTBUT_BOTTOM|wxCUSTBUT_FLAT,wxDefaultValidator,_T("ID_BTN_JOYSTICK"));
    btnJoystickMode->SetBitmapDisabled(btnJoystickMode->CreateBitmapDisabled(btnJoystickMode->GetBitmapLabel()));
    btnJoystickMode->SetLabelMargin(wxSize(10,-1));
    btnJoystickMode->SetBitmapMargin(wxSize(20,6));
    FlexGridSizer10->Add(btnJoystickMode, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
    btnNavCmd = new wxCustomButton(Panel1,ID_BUTTON1,_("Navigate to..."),wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("ICON_NAVIGATE")),wxART_MAKE_CLIENT_ID_FROM_STR(wxString(wxEmptyString))),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_BUTTON|wxCUSTBUT_BOTTOM|wxCUSTBUT_FLAT,wxDefaultValidator,_T("ID_BUTTON1"));
    btnNavCmd->SetBitmapDisabled(btnNavCmd->CreateBitmapDisabled(btnNavCmd->GetBitmapLabel()));
    btnNavCmd->SetLabelMargin(wxSize(10,-1));
    btnNavCmd->SetBitmapMargin(wxSize(20,6));
    FlexGridSizer10->Add(btnNavCmd, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
    btnNavStop = new wxCustomButton(Panel1,ID_BUTTON2,_("Stop"),wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("ICON_STOP")),wxART_MAKE_CLIENT_ID_FROM_STR(wxString(wxEmptyString))),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_BUTTON|wxCUSTBUT_BOTTOM|wxCUSTBUT_FLAT,wxDefaultValidator,_T("ID_BUTTON2"));
    btnNavStop->SetBitmapDisabled(btnNavStop->CreateBitmapDisabled(btnNavStop->GetBitmapLabel()));
    btnNavStop->SetLabelMargin(wxSize(10,6));
    btnNavStop->SetBitmapMargin(wxSize(20,6));
    FlexGridSizer10->Add(btnNavStop, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
    StaticLine1 = new wxStaticLine(Panel1, ID_STATICLINE1, wxDefaultPosition, wxSize(2,-1), wxLI_VERTICAL, _T("ID_STATICLINE1"));
    FlexGridSizer10->Add(StaticLine1, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
    btnZoomIn = new wxCustomButton(Panel1,ID_BUTTON5,_("Zoom in"),wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("ICON_ZOOM_IN")),wxART_MAKE_CLIENT_ID_FROM_STR(wxString(wxEmptyString))),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_BUTTON|wxCUSTBUT_BOTTOM|wxCUSTBUT_FLAT,wxDefaultValidator,_T("ID_BUTTON5"));
    btnZoomIn->SetBitmapDisabled(btnZoomIn->CreateBitmapDisabled(btnZoomIn->GetBitmapLabel()));
    btnZoomIn->SetLabelMargin(wxSize(10,-1));
    btnZoomIn->SetBitmapMargin(wxSize(20,6));
    FlexGridSizer10->Add(btnZoomIn, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
    btnZoomOut = new wxCustomButton(Panel1,ID_BUTTON6,_("Zoom out"),wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("ICON_ZOOM_OUT")),wxART_MAKE_CLIENT_ID_FROM_STR(wxString(wxEmptyString))),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_BUTTON|wxCUSTBUT_BOTTOM|wxCUSTBUT_FLAT,wxDefaultValidator,_T("ID_BUTTON6"));
    btnZoomOut->SetBitmapDisabled(btnZoomOut->CreateBitmapDisabled(btnZoomOut->GetBitmapLabel()));
    btnZoomOut->SetLabelMargin(wxSize(10,-1));
    btnZoomOut->SetBitmapMargin(wxSize(20,6));
    FlexGridSizer10->Add(btnZoomOut, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
    StaticLine3 = new wxStaticLine(Panel1, ID_STATICLINE3, wxDefaultPosition, wxSize(2,-1), wxLI_VERTICAL, _T("ID_STATICLINE3"));
    FlexGridSizer10->Add(StaticLine3, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
    btnLoadTopo = new wxCustomButton(Panel1,ID_BUTTON11,_("Load Topology"),wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FILE_OPEN")),wxART_MAKE_CLIENT_ID_FROM_STR(wxString(wxEmptyString))),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_BUTTON|wxCUSTBUT_BOTTOM|wxCUSTBUT_FLAT,wxDefaultValidator,_T("ID_BUTTON11"));
    btnLoadTopo->SetBitmapDisabled(btnLoadTopo->CreateBitmapDisabled(btnLoadTopo->GetBitmapLabel()));
    FlexGridSizer10->Add(btnLoadTopo, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    btnSaveTopo = new wxCustomButton(Panel1,ID_BUTTON10,_("Save Topology"),wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FILE_SAVE")),wxART_MAKE_CLIENT_ID_FROM_STR(wxString(wxEmptyString))),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_BUTTON|wxCUSTBUT_BOTTOM|wxCUSTBUT_FLAT,wxDefaultValidator,_T("ID_BUTTON10"));
    btnSaveTopo->SetBitmapDisabled(btnSaveTopo->CreateBitmapDisabled(btnSaveTopo->GetBitmapLabel()));
    btnSaveTopo->SetBitmapMargin(wxSize(20,6));
    FlexGridSizer10->Add(btnSaveTopo, 1, wxALL|wxEXPAND|wxFIXED_MINSIZE|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    btnNewNode = new wxCustomButton(Panel1,ID_BUTTON8,_("New Node"),wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_ADD_BOOKMARK")),wxART_MAKE_CLIENT_ID_FROM_STR(wxString(wxEmptyString))),wxDefaultPosition,wxSize(67,38),wxCUSTBUT_BUTTON|wxCUSTBUT_BOTTOM|wxCUSTBUT_FLAT,wxDefaultValidator,_T("ID_BUTTON8"));
    btnNewNode->SetBitmapDisabled(btnNewNode->CreateBitmapDisabled(btnNewNode->GetBitmapLabel()));
    btnNewNode->SetLabelMargin(wxSize(10,-1));
    btnNewNode->SetBitmapMargin(wxSize(20,6));
    FlexGridSizer10->Add(btnNewNode, 1, wxALL|wxEXPAND|wxFIXED_MINSIZE|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    btnEditNode = new wxCustomButton(Panel1,ID_BUTTON9,_("Edit Node"),wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FIND_AND_REPLACE")),wxART_MAKE_CLIENT_ID_FROM_STR(wxString(wxEmptyString))),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_BUTTON|wxCUSTBUT_BOTTOM|wxCUSTBUT_FLAT,wxDefaultValidator,_T("ID_BUTTON9"));
    btnEditNode->SetBitmapDisabled(btnEditNode->CreateBitmapDisabled(btnEditNode->GetBitmapLabel()));
    btnEditNode->SetLabelMargin(wxSize(10,-1));
    btnEditNode->SetBitmapMargin(wxSize(20,6));
    FlexGridSizer10->Add(btnEditNode, 1, wxALL|wxEXPAND|wxFIXED_MINSIZE|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    btnPlaceRobot = new wxCustomButton(Panel1,ID_BUTTON12,_("Robot position"),wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("PLACE_ROBOT")),wxART_MAKE_CLIENT_ID_FROM_STR(wxString(wxEmptyString))),wxDefaultPosition,wxSize(76,59),wxCUSTBUT_BUTTON|wxCUSTBUT_BOTTOM|wxCUSTBUT_FLAT,wxDefaultValidator,_T("ID_BUTTON12"));
    btnPlaceRobot->SetBitmapDisabled(btnPlaceRobot->CreateBitmapDisabled(btnPlaceRobot->GetBitmapLabel()));
    btnPlaceRobot->SetLabelMargin(wxSize(10,6));
    btnPlaceRobot->SetBitmapMargin(wxSize(20,6));
    FlexGridSizer10->Add(btnPlaceRobot, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticLine4 = new wxStaticLine(Panel1, ID_STATICLINE4, wxDefaultPosition, wxSize(2,-1), wxLI_VERTICAL, _T("ID_STATICLINE4"));
    FlexGridSizer10->Add(StaticLine4, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    btnQuit = new wxCustomButton(Panel1,ID_BUTTON7,_("Quit"),wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("ICON_QUIT")),wxART_MAKE_CLIENT_ID_FROM_STR(wxString(wxEmptyString))),wxDefaultPosition,wxDefaultSize,wxCUSTBUT_BUTTON|wxCUSTBUT_BOTTOM|wxCUSTBUT_FLAT,wxDefaultValidator,_T("ID_BUTTON7"));
    btnQuit->SetBitmapDisabled(btnQuit->CreateBitmapDisabled(btnQuit->GetBitmapLabel()));
    btnQuit->SetLabelMargin(wxSize(10,6));
    btnQuit->SetBitmapMargin(wxSize(20,6));
    FlexGridSizer10->Add(btnQuit, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
    Panel1->SetSizer(FlexGridSizer10);
    FlexGridSizer10->SetSizeHints(Panel1);
    FlexGridSizer11->Add(Panel1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SplitterWindow2 = new wxSplitterWindow(Panel9, ID_SPLITTERWINDOW2, wxDefaultPosition, wxDefaultSize, wxSP_3D, _T("ID_SPLITTERWINDOW2"));
    SplitterWindow2->SetMinSize(wxSize(100,100));
    SplitterWindow2->SetMaxSize(wxSize(-1,-1));
    SplitterWindow2->SetMinimumPaneSize(100);
    Panel2 = new wxPanel(SplitterWindow2, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    FlexGridSizer4 = new wxFlexGridSizer(2, 1, 0, 0);
    FlexGridSizer4->AddGrowableCol(0);
    FlexGridSizer4->AddGrowableRow(0);
    FlexGridSizer4->AddGrowableRow(1);
    FlexGridSizer6 = new wxFlexGridSizer(2, 1, 0, 0);
    FlexGridSizer6->AddGrowableCol(0);
    FlexGridSizer6->AddGrowableRow(1);
    StaticText3 = new wxStaticText(Panel2, ID_STATICTEXT3, _("Current plan:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    FlexGridSizer6->Add(StaticText3, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
    lstCurPlan = new wxListBox(Panel2, ID_LISTBOX1, wxDefaultPosition, wxSize(288,164), 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX1"));
    FlexGridSizer6->Add(lstCurPlan, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
    FlexGridSizer4->Add(FlexGridSizer6, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
    Notebook1 = new wxNotebook(Panel2, ID_NOTEBOOK1, wxDefaultPosition, wxSize(-1,-1), 0, _T("ID_NOTEBOOK1"));
    Panel4 = new wxPanel(Notebook1, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
    FlexGridSizer5 = new wxFlexGridSizer(2, 1, 0, 0);
    FlexGridSizer5->AddGrowableCol(0);
    FlexGridSizer5->AddGrowableRow(0);
    gridRunningMods = new wxGrid(Panel4, ID_GRID1, wxDefaultPosition, wxDefaultSize, 0, _T("ID_GRID1"));
    gridRunningMods->CreateGrid(1,2);
    wxFont gridRunningModsFont = wxSystemSettings::GetFont(wxSYS_OEM_FIXED_FONT);
    if ( !gridRunningModsFont.Ok() ) gridRunningModsFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    gridRunningModsFont.SetPointSize(7);
    gridRunningMods->SetFont(gridRunningModsFont);
    FlexGridSizer5->Add(gridRunningMods, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
    Panel4->SetSizer(FlexGridSizer5);
    FlexGridSizer5->Fit(Panel4);
    FlexGridSizer5->SetSizeHints(Panel4);
    Panel7 = new wxPanel(Notebook1, ID_PANEL7, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL7"));
    FlexGridSizer9 = new wxFlexGridSizer(2, 1, 0, 0);
    FlexGridSizer9->AddGrowableCol(0);
    FlexGridSizer9->AddGrowableRow(0);
    cblistViewEnables = new wxCheckListBox(Panel7, ID_CHECKLISTBOX1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHECKLISTBOX1"));
    cblistViewEnables->Check(cblistViewEnables->Append(_("LASER1")));
    cblistViewEnables->Check(cblistViewEnables->Append(_("LASER2")));
    cblistViewEnables->Append(_("LASER3"));
    cblistViewEnables->Check(cblistViewEnables->Append(_("SONAR1")));
    cblistViewEnables->Append(_("VIRTUALWORLD"));
	cblistViewEnables->Append(_("DOCK_STATION"));
    FlexGridSizer9->Add(cblistViewEnables, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 1);
    Panel8 = new wxPanel(Panel7, ID_PANEL8, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL8"));
    FlexGridSizer3 = new wxFlexGridSizer(2, 1, 0, 0);
    cbCameraFollowsRobot = new wxCheckBox(Panel8, ID_CHECKBOX1, _("Camera follows robot"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    cbCameraFollowsRobot->SetValue(false);
    FlexGridSizer3->Add(cbCameraFollowsRobot, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
    Panel8->SetSizer(FlexGridSizer3);
    FlexGridSizer3->Fit(Panel8);
    FlexGridSizer3->SetSizeHints(Panel8);
    FlexGridSizer9->Add(Panel8, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
    Panel7->SetSizer(FlexGridSizer9);
    FlexGridSizer9->Fit(Panel7);
    FlexGridSizer9->SetSizeHints(Panel7);
    Notebook1->AddPage(Panel4, _("Modules"), true);
    Notebook1->AddPage(Panel7, _("View"), false);
    FlexGridSizer4->Add(Notebook1, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
    Panel2->SetSizer(FlexGridSizer4);
    FlexGridSizer4->Fit(Panel2);
    FlexGridSizer4->SetSizeHints(Panel2);
    Panel3 = new wxPanel(SplitterWindow2, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
    FlexGridSizer2 = new wxFlexGridSizer(1, 1, 0, 0);
    FlexGridSizer2->AddGrowableCol(0);
    FlexGridSizer2->AddGrowableRow(0);
    SplitterWindow1 = new wxSplitterWindow(Panel3, ID_SPLITTERWINDOW1, wxDefaultPosition, wxDefaultSize, wxSP_3D, _T("ID_SPLITTERWINDOW1"));
    SplitterWindow1->SetMinSize(wxSize(50,50));
    SplitterWindow1->SetMinimumPaneSize(50);
	SplitterWindow1->SetSashGravity(1);
    Panel5 = new wxPanel(SplitterWindow1, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL5"));
    FlexGridSizer7 = new wxFlexGridSizer(2, 1, 0, 0);
    FlexGridSizer7->AddGrowableCol(0);
    FlexGridSizer7->AddGrowableRow(1);
    StaticText2 = new wxStaticText(Panel5, ID_STATICTEXT2, _("3D robot view"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    FlexGridSizer7->Add(StaticText2, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
    m_glMap = new CMyGLCanvas(Panel5,ID_GLOBAL_GLCANVAS,wxDefaultPosition,wxSize(290,124),wxTAB_TRAVERSAL,_T("ID_GLOBAL_GLCANVAS"));
    FlexGridSizer7->Add(m_glMap, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 1);
    Panel5->SetSizer(FlexGridSizer7);
    FlexGridSizer7->Fit(Panel5);
    FlexGridSizer7->SetSizeHints(Panel5);
    Panel6 = new wxPanel(SplitterWindow1, ID_PANEL6, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL6"));
    FlexGridSizer8 = new wxFlexGridSizer(3, 2, 0, 0);
    FlexGridSizer8->AddGrowableCol(1);
    FlexGridSizer8->AddGrowableRow(1);

    StaticTextBattery = new wxStaticText(Panel6, ID_STATICTEXT1, _("Battery base: N/A"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));	
    FlexGridSizer8->Add(StaticTextBattery, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	btnBattery=new wxCustomButton(Panel6,ID_BUTTONBATTERY,_(""),wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("BATTERY_FULL")),wxART_MAKE_CLIENT_ID_FROM_STR(wxString(wxEmptyString))),wxDefaultPosition,wxDefaultSize,wxBORDER_NONE,wxDefaultValidator,_T("ID_BUTTONBATTERY"));
	FlexGridSizer8->Add(btnBattery,1,0,0);
	StaticTextBattery2 = new wxStaticText(Panel6, ID_STATICTEXT5, _("Battery ext: N/A"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer8->Add(StaticTextBattery2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	btnBattery2=new wxCustomButton(Panel6,ID_BUTTONBATTERY2,_(""),wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("BATTERY_FULL")),wxART_MAKE_CLIENT_ID_FROM_STR(wxString(wxEmptyString))),wxDefaultPosition,wxDefaultSize,wxBORDER_NONE,wxDefaultValidator,_T("ID_BUTTONBATTERY2"));
	FlexGridSizer8->Add(btnBattery2,1,0,0);

    StaticText4 = new wxStaticText(Panel6, ID_STATICTEXT4, _("System messages:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    FlexGridSizer8->Add(StaticText4, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
    edSystemLog = new wxTextCtrl(Panel6, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxSize(252,250), wxTE_MULTILINE|wxTE_READONLY|wxTE_DONTWRAP|wxALWAYS_SHOW_SB, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    edSystemLog->SetMinSize(wxSize(-1,-1));
    wxFont edSystemLogFont = wxSystemSettings::GetFont(wxSYS_OEM_FIXED_FONT);
    if ( !edSystemLogFont.Ok() ) edSystemLogFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    edSystemLogFont.SetPointSize(8);
    edSystemLog->SetFont(edSystemLogFont);
    FlexGridSizer8->Add(edSystemLog, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 2);
    Panel6->SetSizer(FlexGridSizer8);
    FlexGridSizer8->Fit(Panel6);
    FlexGridSizer8->SetSizeHints(Panel6);
    SplitterWindow1->SplitHorizontally(Panel5, Panel6);
    FlexGridSizer2->Add(SplitterWindow1, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 0);
    Panel3->SetSizer(FlexGridSizer2);
    FlexGridSizer2->Fit(Panel3);
    FlexGridSizer2->SetSizeHints(Panel3);
    SplitterWindow2->SplitVertically(Panel2, Panel3);
    FlexGridSizer11->Add(SplitterWindow2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel9->SetSizer(FlexGridSizer11);
    FlexGridSizer11->Fit(Panel9);
    FlexGridSizer11->SetSizeHints(Panel9);
//    Panel10 = new wxPanel(Notebook2, ID_PANEL10, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL10"));
//    FlexGridSizer12 = new wxFlexGridSizer(0, 3, 0, 0); COMMENTED OUT FOR NEW STATE CHARTS EDITOR
//    Panel10->SetSizer(FlexGridSizer12);   COMMENTED OUT FOR NEW STATE CHARTS EDITOR
//    FlexGridSizer12->Fit(Panel10); COMMENTED OUT FOR NEW STATE CHARTS EDITOR
//    FlexGridSizer12->SetSizeHints(Panel10); COMMENTED OUT FOR NEW STATE CHARTS EDITOR
    Notebook2->AddPage(Panel9, _("Navigation && Maps"), false);


	// BATTERY MANAGEMENT SECTION

	battery_management=new wxPanel(Notebook2,ID_PANEL12, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL12"));

	FlexGridSizer13 = new wxFlexGridSizer(2, 2, 0, 0);
    FlexGridSizer13->AddGrowableCol(0);
	FlexGridSizer13->AddGrowableRow(0);

	battery_management->SetSizer(FlexGridSizer13);

	bat_plot1 = new mpWindow( battery_management, -1, wxPoint(10,5), wxSize(400,300), wxSUNKEN_BORDER );

	wxFont graphFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	mpScaleX* xaxis = new mpScaleX(wxT("t"), mpALIGN_BOTTOM, true);
	mpScaleY* yaxis = new mpScaleY(wxT("V"), mpALIGN_LEFT, true);
	bat_plot1->SetLabel(wxT("Laptop Voltage"));
	xaxis->SetTicks(false);
	yaxis->SetTicks(false);
	xaxis->SetFont(graphFont);
	yaxis->SetFont(graphFont);
    xaxis->SetDrawOutsideMargins(false);
    yaxis->SetDrawOutsideMargins(false);
	bat_plot1->SetMargins(30, 30, 30, 30);
	bat_plot1->AddLayer(     xaxis );
    bat_plot1->AddLayer(     yaxis );


	bat_plot2 = new mpWindow( battery_management, -1, wxPoint(450,5), wxSize(400,300), wxSUNKEN_BORDER );

	wxFont graphFont2(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	mpScaleX* xaxis2 = new mpScaleX(wxT("t"), mpALIGN_BOTTOM, true);
	mpScaleY* yaxis2 = new mpScaleY(wxT("V"), mpALIGN_LEFT, true);
	xaxis2->SetTicks(false);
	yaxis2->SetTicks(false);
	xaxis2->SetFont(graphFont2);
	yaxis2->SetFont(graphFont2);
    xaxis2->SetDrawOutsideMargins(false);
    yaxis2->SetDrawOutsideMargins(false);
	bat_plot2->SetMargins(30, 30, 30, 30);
	bat_plot2->AddLayer(     xaxis2 );
    bat_plot2->AddLayer(     yaxis2 );

	// Create a mpFXYVector layer
	bat_data = new mpFXYVector();
	bat_plot1->AddLayer(     bat_data );

	bat_data2 = new mpFXYVector();
	bat_plot2->AddLayer(     bat_data2 );

	bat_plot1->EnableDoubleBuffer(true);
    bat_plot1->SetMPScrollbars(false);
	bat_plot1->Fit();
	bat_plot1->Refresh();

	bat_plot2->EnableDoubleBuffer(true);
    bat_plot2->SetMPScrollbars(false);
	bat_plot2->Fit();
	bat_plot2->Refresh();

	lcdtext1=new kwxLCDDisplay(battery_management, wxPoint(100,400),wxSize(200,100));
	lcdtext2=new kwxLCDDisplay(battery_management, wxPoint(600,400),wxSize(200,100));

    // NEW STATE CHARTS EDIT SECTION

    panel_SCE = new wxPanel(Notebook2, ID_PANEL10, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL10"));
    stateButtonSCE = new wxButton(panel_SCE,BTN_STATE,_("State"));
    linkButtonSCE = new wxButton(panel_SCE,BTN_LINK,_("Link"));
    saveButtonSCE = new wxButton(panel_SCE,pBTN_SAVE,_("Save"));
    saveasButtonSCE = new wxButton(panel_SCE,BTN_SAVEAS,_("Save as"));
    pubButtonSCE = new wxButton(panel_SCE,BTN_PUB,_("Load"));
    openButtonSCE = new wxButton(panel_SCE,pBTN_OPEN,_("Open"));

    // convert all actions from string to wxString
        /*// temporary solution
        std::vector<std::string> actionList;
        std::string str1 = *(new string("ACTION1"));
        std::string str2 = *(new string("ACTION2"));
        actionList.push_back(str1);
        actionList.push_back(str2);
        // end of the temporary solution*/




/*
    std::vector<std::string>::iterator action = actionList.begin();
    std::vector<wxString> actions;
    wxString action2;
    while (action!=actionList.end()){
        action2 = wxString::FromAscii((*action).c_str());
        actions.push_back(action2);
        action++;
    }*/

    boardSCE = new PaintBoard(panel_SCE);
 //   boardSCE->setActions(actions);

    wxBoxSizer *buttonsSCE = new wxBoxSizer(wxVERTICAL);
    buttonsSCE->Add(stateButtonSCE,0);
    buttonsSCE->Add(linkButtonSCE,0);
    buttonsSCE->Add(saveButtonSCE,0);
    buttonsSCE->Add(saveasButtonSCE,0);
    buttonsSCE->Add(pubButtonSCE,0);
    buttonsSCE->Add(openButtonSCE,0);

    wxBoxSizer *mainSizerSCE = new wxBoxSizer(wxHORIZONTAL);
    mainSizerSCE->Add(buttonsSCE,0,wxLEFT);
    mainSizerSCE->Add(boardSCE,1,wxRIGHT|wxEXPAND);

    panel_SCE->SetSizer(mainSizerSCE);

    Connect(BTN_STATE,wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RobotGUI2009_guidesignFrame::OnStateButton));
    Connect(BTN_LINK,wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RobotGUI2009_guidesignFrame::OnLinkButton));
    Connect(pBTN_SAVE,wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RobotGUI2009_guidesignFrame::OnSaveButton));
    Connect(BTN_PUB,wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RobotGUI2009_guidesignFrame::OnPublishButton));
    Connect(BTN_SAVEAS,wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RobotGUI2009_guidesignFrame::OnSaveAsButton));
    Connect(pBTN_OPEN,wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RobotGUI2009_guidesignFrame::OnOpen));
    // END OF GUI DEFINITION OF NEW STATE CHARTS EDIT


	// GUI DEFINITION OF THE NEW SIMPLE PANEL
	simplePanel = new wxPanel(Notebook2, ID_PANEL11, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL11"));
	mission1 = new wxButton(simplePanel,BTN_M1,_("Mission 1"),wxDefaultPosition,wxSize(200,100));
	mission1->SetFont(wxFont(15,wxFONTFAMILY_ROMAN,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD));
	mission2 = new wxButton(simplePanel,BTN_M2,_("Mission 2"),wxDefaultPosition,wxSize(200,100));
	mission2->SetFont(wxFont(15,wxFONTFAMILY_ROMAN,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD));
	mission3 = new wxButton(simplePanel,BTN_M3,_("Mission 3"),wxDefaultPosition,wxSize(200,100));
	mission3->SetFont(wxFont(15,wxFONTFAMILY_ROMAN,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD));
	mission4 = new wxButton(simplePanel,BTN_M4,_("Mission 4"),wxDefaultPosition,wxSize(200,100));
	mission4->SetFont(wxFont(15,wxFONTFAMILY_ROMAN,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD));
	mission5 = new wxButton(simplePanel,BTN_M5,_("Mission 5"),wxDefaultPosition,wxSize(200,100));
	mission5->SetFont(wxFont(15,wxFONTFAMILY_ROMAN,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD));
	mission6 = new wxButton(simplePanel,BTN_M6,_("Mission 6"),wxDefaultPosition,wxSize(200,100));
	mission6->SetFont(wxFont(15,wxFONTFAMILY_ROMAN,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD));
	simple_stop = new wxButton(simplePanel,BTN_SIMPLE_STOP,_("STOP"),wxDefaultPosition,wxSize(200,200));
	simple_stop->SetFont(wxFont(20,wxFONTFAMILY_ROMAN,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD));
	simple_stop->SetBackgroundColour(wxColour(255,0,0));
	simple_expert = new wxButton(simplePanel,BTN_SIMPLE_EXPERT,_("Expert mode"),wxDefaultPosition,wxSize(200,100));
	simple_expert->SetFont(wxFont(15,wxFONTFAMILY_ROMAN,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD));

	wxBoxSizer *simple_row1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *simple_row2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *simple_row3 = new wxBoxSizer(wxHORIZONTAL);
//	wxBoxSizer *simple_row4 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *missionRack = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *controlRack = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *simple_panel_sizer = new wxBoxSizer(wxHORIZONTAL);
	//wxBoxSizer *simple_panel_sizer2 = new wxBoxSizer(wxVERTICAL|wxEXPAND);

	simple_row1->Add(mission1,0);
	simple_row1->Add(mission2,0);

	simple_row2->Add(mission3,0);
	simple_row2->Add(mission4,0);

	simple_row3->Add(mission5,0);
	simple_row3->Add(mission6,0);
/*
	simple_row4->Add(simple_stop,0);
	simple_row4->Add(simple_expert,0);
*/

	missionRack->Add(simple_row1,0);
	missionRack->Add(simple_row2,0);
	missionRack->Add(simple_row3,0);

	controlRack->Add(simple_stop,0,wxALIGN_RIGHT);
	controlRack->Add(simple_expert,0,wxALIGN_RIGHT);

	simple_panel_sizer->Add(controlRack,1,wxALIGN_CENTER_VERTICAL);
	simple_panel_sizer->Add(missionRack,1,wxALIGN_CENTER_VERTICAL);
	//simple_panel_sizer2->Add(simple_panel_sizer,wxALIGN_CENTER);
	simplePanel->SetSizer(simple_panel_sizer);

	Connect(BTN_M1,wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RobotGUI2009_guidesignFrame::OnMission1Button));
	Connect(BTN_M2,wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RobotGUI2009_guidesignFrame::OnMission2Button));
	Connect(BTN_M3,wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RobotGUI2009_guidesignFrame::OnMission3Button));
	Connect(BTN_M4,wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RobotGUI2009_guidesignFrame::OnMission4Button));
	Connect(BTN_M5,wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RobotGUI2009_guidesignFrame::OnMission5Button));
	Connect(BTN_M6,wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RobotGUI2009_guidesignFrame::OnMission6Button));

	Connect(BTN_SIMPLE_STOP,wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RobotGUI2009_guidesignFrame::OnSimpleStopButton));
	Connect(BTN_SIMPLE_EXPERT,wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RobotGUI2009_guidesignFrame::OnSimpleExpertButton));
	// END OF GUI DEFINITION OF THE NEW SIMPLE PANEL

    Notebook2->AddPage(panel_SCE, _("Robot Missions"), false);
    Notebook2->AddPage(simplePanel, _("Simple GUI"), false);
	Notebook2->AddPage(battery_management,_("Battery Management"),false);
	Notebook2->SetSelection(2);
    FlexGridSizer1->Add(Notebook2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    SetSizer(FlexGridSizer1);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);
    timUpdateFromMOOS.SetOwner(this, ID_TIMER1);
    timUpdateFromMOOS.Start(100, false);
    timInit.SetOwner(this, ID_TIMER2);
    timInit.Start(20, true);
    FlexGridSizer1->SetSizeHints(this);
    Center();

    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RobotGUI2009_guidesignFrame::OnSendResetSimulator);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RobotGUI2009_guidesignFrame::OnBtnRelocalizeWithPF);
    Connect(ID_BTN_JOYSTICK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RobotGUI2009_guidesignFrame::OnbtnEnableJoyClick);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RobotGUI2009_guidesignFrame::OnReactiveNavCmd);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RobotGUI2009_guidesignFrame::OnStopNavigator);
    Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RobotGUI2009_guidesignFrame::OnbtnZoomInClick);
    Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RobotGUI2009_guidesignFrame::OnbtnZoomOutClick);
    Connect(ID_BUTTON10,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RobotGUI2009_guidesignFrame::OnbtnSaveTopoClick);
	Connect(ID_BUTTON11,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RobotGUI2009_guidesignFrame::OnbtnLoadTopoClick);
    Connect(ID_BUTTON8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RobotGUI2009_guidesignFrame::OnbtnNewNodeClick);
    Connect(ID_BUTTON9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RobotGUI2009_guidesignFrame::OnbtnEditNodeClick);
    Connect(ID_BUTTON12,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RobotGUI2009_guidesignFrame::OnbtnPlaceRobotClick);
    Connect(ID_BUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RobotGUI2009_guidesignFrame::OnQuit);
    Connect(ID_CHECKLISTBOX1,wxEVT_COMMAND_CHECKLISTBOX_TOGGLED,(wxObjectEventFunction)&RobotGUI2009_guidesignFrame::OncblistViewEnablesToggled);
    Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&RobotGUI2009_guidesignFrame::OncbCameraFollowsRobotClick);
    Connect(ID_NOTEBOOK2,wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,(wxObjectEventFunction)&RobotGUI2009_guidesignFrame::OnNotebook2PageChanged1);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&RobotGUI2009_guidesignFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&RobotGUI2009_guidesignFrame::OnAbout);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&RobotGUI2009_guidesignFrame::OntimUpdateFromMOOSTrigger);
    Connect(ID_TIMER2,wxEVT_TIMER,(wxObjectEventFunction)&RobotGUI2009_guidesignFrame::OntimInitTrigger);
    //*)

	// Setup 3D view ================================
	m_glMap->m_openGLScene->insert( mrpt::opengl::CGridPlaneXY::Create(-60,60,-60,60,0,5) );

	m_gl_metricmap = opengl::CSetOfObjects::Create();
	m_glMap->m_openGLScene->insert(m_gl_metricmap);

	m_gl_gasmap_simul = opengl::CSetOfObjects::Create();
	m_glMap->m_openGLScene->insert(m_gl_gasmap_simul);


	m_gl_gas_mean_online_map = opengl::CSetOfObjects::Create();
	m_glMap->m_openGLScene->insert(m_gl_gas_mean_online_map);

	m_gl_gas_var_online_map = opengl::CSetOfObjects::Create();
	m_glMap->m_openGLScene->insert(m_gl_gas_var_online_map);

	m_gl_wind_online_map = opengl::CSetOfObjects::Create();
	m_glMap->m_openGLScene->insert(m_gl_wind_online_map);



	//As viewPort (jgMonroy)
	////ONLINE_MAP
	//// Create a ViewPort in the Bottom-left corner to show MEAN_MAP
	//COpenGLViewportPtr mean_map = m_glMap->m_openGLScene->createViewport("miniMap1");
	//mean_map->setBorderSize(0);
 //   mean_map->setViewportPosition(0.01,0.01,0.35,0.35);
 //   mean_map->setTransparent(true);
	//{
	//	mrpt::opengl::CCamera &cam = mean_map->getCamera();
	//	cam.setAzimuthDegrees(-90);
	//	cam.setElevationDegrees(90);
	//	cam.setZoomDistance(20);
	//	cam.setOrthogonal();
	//}
	////MAINmean submap
	//m_gl_mean_online_map = opengl::CSetOfObjects::Create();
	//mean_map->insert(m_gl_metricmap);
	//mean_map->insert(m_gl_mean_online_map);

	//// Create a ViewPort in the Bottom-left corner to show VAR_MAP
	//COpenGLViewportPtr var_map = m_glMap->m_openGLScene->createViewport("miniMap2");
	//var_map->setBorderSize(0);
 //   var_map->setViewportPosition(0.19,0.01,0.35,0.35);
 //   var_map->setTransparent(true);
	//{
	//	mrpt::opengl::CCamera &cam = var_map->getCamera();
	//	cam.setAzimuthDegrees(-90);
	//	cam.setElevationDegrees(90);
	//	cam.setZoomDistance(20);
	//	cam.setOrthogonal();
	//}
	////MAINvar submap
	//m_gl_var_online_map = opengl::CSetOfObjects::Create();
	//var_map->insert(m_gl_metricmap);
	//var_map->insert(m_gl_var_online_map);


	m_gl_virtual_obstacles = opengl::CSetOfObjects::Create();
	m_glMap->m_openGLScene->insert(m_gl_virtual_obstacles);

	{	// Sign of "current navigation target ":
		m_gl_current_nav_target = opengl::CSetOfObjects::Create();
		opengl::CDiskPtr o = opengl::CDisk::Create(0.5,0.4, 50,4);
		o->setColor(0.1,0.1,0.9);
		m_gl_current_nav_target->insert( o );
		m_gl_current_nav_target->setName("NAV_TRG_DISK");
		m_gl_current_nav_target->setVisibility(false); // Start invisible.
		m_glMap->m_openGLScene->insert(m_gl_current_nav_target);
	}

	{	// Sign of "picking a navigation target":
		m_gl_placing_nav_target = opengl::CSetOfObjects::Create();
		opengl::CDiskPtr o = opengl::CDisk::Create(0.5,0.4, 50,4);
		o->setColor(0.7,0.7,0.7);
		m_gl_placing_nav_target->insert( o );
		m_gl_placing_nav_target->setName("PLACING_NAV_TRG_DISK");
		m_gl_placing_nav_target->setVisibility(false); // Start invisible.
		m_glMap->m_openGLScene->insert(m_gl_placing_nav_target);
	}

	{	// Sign of "picking a relocalizing area":
		m_gl_relocalizing_area = opengl::CSetOfObjects::Create();
		opengl::CSetOfLinesPtr o = opengl::CSetOfLines::Create();
		o->setColor(0.1,0.8,0.8);
		o->setLineWidth(4);
		const double a=RELOCALIZ_AREA_WIDTH*0.5;
		o->appendLine(-a,-a,0,-a,a,0);
		o->appendLine(-a,a,0,a,a,0);
		o->appendLine(a,a,0,a,-a,0);
		o->appendLine(a,-a,0,-a,-a,0);
		m_gl_relocalizing_area->insert( o );
		m_gl_relocalizing_area->setName("PLACING_RELOCALIZ_AREA");
		m_gl_relocalizing_area->setVisibility(false); // Start invisible.
		m_glMap->m_openGLScene->insert(m_gl_relocalizing_area);
	}


	m_gl_robot = mrpt::opengl::CSetOfObjects::Create();
	m_gl_robot->setName("robot_group");

	//m_gl_robot_model=mrpt::opengl::stock_objects::RobotPioneer();
	//m_gl_robot_model=mrpt::opengl::stock_objects::RobotRhodon();
	m_gl_robot_model=mrpt::opengl::stock_objects::RobotGiraff();
	m_gl_robot->insert( m_gl_robot_model );

	//Lasers
	m_gl_robot_lasers.resize(3);
	m_scan.resize(3);
	for (size_t i=0;i<m_gl_robot_lasers.size();i++)
	{
		m_gl_robot_lasers[i] = opengl::CPlanarLaserScan::Create();
		m_gl_robot->insert(m_gl_robot_lasers[i]);   // Lasers go onboard the robot
	}

	//Range camera
	//m_gl_robot_rangecam = opengl::CPointCloudColoured::Create();
	m_gl_robot_rangecam = opengl::CPointCloud::Create();
	m_gl_robot_rangecam->setColor(0,0.7,0);
	m_gl_robot_rangecam->setPointSize( 3.0 );
	m_gl_robot->insert( m_gl_robot_rangecam );
	m_gl_robot_rangecam_2D_scan = opengl::CPlanarLaserScan::Create();
	m_gl_robot->insert( m_gl_robot_rangecam_2D_scan );

	//GridPoints
	m_gl_gridpoints = opengl::CPointCloud::Create();
	m_gl_gridpoints->setColor(0,0,0.8);
	m_gl_gridpoints->setPointSize( 5.0 );
	m_gl_robot->insert( m_gl_gridpoints );

	m_gl_sonars.resize(3);
	for (size_t i=0;i<m_gl_sonars.size();i++)
	{
		m_gl_sonars[i] = mrpt::opengl::CSetOfObjects::Create();
		m_gl_sonars[i]->setName(format("sonars%u",(unsigned)i));
		m_gl_robot->insert(m_gl_sonars[i]);   // Lasers go onboard the robot
	}

	m_glMap->m_openGLScene->insert(m_gl_robot);

	m_gl_parts = opengl::CPointCloud::Create();
	m_gl_parts->setName("particles");
	m_glMap->m_openGLScene->insert(m_gl_parts);

	m_gl_topological_map = opengl::CSetOfObjects::Create();
	m_glMap->m_openGLScene->insert(m_gl_topological_map);
	node_selected=false;

	//virtual map
	m_gl_virtual_map = opengl::CSetOfObjects::Create();
	m_glMap->m_openGLScene->insert(m_gl_virtual_map);

	// wifi
	m_gl_wifi=opengl::CSetOfObjects::Create();
	m_glMap->m_openGLScene->insert(m_gl_wifi);

	// dock station
	m_gl_dock_station=opengl::CSetOfObjects::Create();
	m_glMap->m_openGLScene->insert(m_gl_dock_station);


	// local_map_limits
	m_gl_local_map_limits = opengl::CSetOfObjects::Create();
	m_glMap->m_openGLScene->insert(m_gl_local_map_limits);

	// bumpers
	m_gl_bumpers=opengl::CSetOfObjects::Create();
	m_glMap->m_openGLScene->insert(m_gl_bumpers);

	// Events on the 3D view:
	m_glMap->Connect(wxEVT_MOTION,(wxObjectEventFunction)&RobotGUI2009_guidesignFrame::OnplotMouseMove,0,this);
	m_glMap->Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&RobotGUI2009_guidesignFrame::OnplotMouseClick,0,this);

	// Prepare timers:
    timUpdateFromMOOS.Stop();
    timUpdateFromMOOS.Start(ROBOTGUI_MAIN_SCREEN_REFRESH_PERIOD*1000, false);

	timUpdateListOfConnectedModules.SetOwner(this, ID_TIMER_LIST_MODULES);
    timUpdateListOfConnectedModules.Start(4000, false);
    Connect(ID_TIMER_LIST_MODULES,wxEVT_TIMER,(wxObjectEventFunction)&RobotGUI2009_guidesignFrame::OnTimerUpdateListModules);

	timDoCameraAnim.SetOwner(this, ID_TIMER_DO_ANIM_CAMERA);
    Connect(ID_TIMER_DO_ANIM_CAMERA,wxEVT_TIMER,(wxObjectEventFunction)&RobotGUI2009_guidesignFrame::OnTimerDoCameraAnimation);

	// Check list box of 3D objects:
	// *NOTE*: Don't forget to also add these variables to "subscribe_vars.cpp"
	cblistViewEnables->Clear();
	this->addObjectToListOfVisibleCheckboxs("ROBOT",m_gl_robot_model, true);
	this->addObjectToListOfVisibleCheckboxs("PARTICLES",m_gl_parts, true);
	this->addObjectToListOfVisibleCheckboxs("METRIC_MAP",m_gl_metricmap, true);
	this->addObjectToListOfVisibleCheckboxs("GASMAP_SIMUL",m_gl_gasmap_simul, true);
	this->addObjectToListOfVisibleCheckboxs("GAS_MEAN_ONLINE_MAP",m_gl_gas_mean_online_map, true);
	this->addObjectToListOfVisibleCheckboxs("GAS_VAR_ONLINE_MAP",m_gl_gas_var_online_map, false);
	this->addObjectToListOfVisibleCheckboxs("WIND_ONLINE_MAP",m_gl_wind_online_map, true);
	this->addObjectToListOfVisibleCheckboxs("TOPOLOGICAL_MAP",m_gl_topological_map, false);
	this->addObjectToListOfVisibleCheckboxs("LASER1",m_gl_robot_lasers[0], true);
	this->addObjectToListOfVisibleCheckboxs("LASER2",m_gl_robot_lasers[1], true);
	this->addObjectToListOfVisibleCheckboxs("LASER3",m_gl_robot_lasers[2], false);
	this->addObjectToListOfVisibleCheckboxs("RANGECAM1",m_gl_robot_rangecam, true);
	this->addObjectToListOfVisibleCheckboxs("SONAR1",m_gl_sonars[0], false);
	this->addObjectToListOfVisibleCheckboxs("SONAR2",m_gl_sonars[1], false);
	this->addObjectToListOfVisibleCheckboxs("INFRARED1",m_gl_sonars[2], false);
	this->addObjectToListOfVisibleCheckboxs("VIRTUAL_OBS",m_gl_virtual_obstacles, true);
	this->addObjectToListOfVisibleCheckboxs("VIRTUAL_WORLD",m_gl_virtual_map, false);
	this->addObjectToListOfVisibleCheckboxs("DOCK_STATION",m_gl_dock_station, false);
	this->addObjectToListOfVisibleCheckboxs("LOCAL_MAP_LIMITS",m_gl_local_map_limits, false);


	this->addObjectToListOfVisibleCheckboxs("BUMPERS",m_gl_bumpers, false);
	this->addObjectToListOfVisibleCheckboxs("WIFI",m_gl_wifi, false);


	{
		wxCommandEvent ev;
		OncblistViewEnablesToggled(ev);  // Grab the visibility states to the actual m_gl_* objects
	}

	// Load extra parameters, as needed:
	CMissionReader2ConfigFile_adaptor   ini(&m_theWxApp->m_pcr);

	// Start in "simple" or "expert" GUI?
	// --------------------------------------
	//! @moos_param start_in_simple_gui Start in 'simple' mode (vs. 'expert' mode) (default: false).
	const bool start_in_simple_gui = ini.read_bool("","start_in_simple_gui",false);

	if (!start_in_simple_gui)
		Notebook2->ChangeSelection(0);

    Maximize();

}

RobotGUI2009_guidesignFrame::~RobotGUI2009_guidesignFrame()
{
    //(*Destroy(RobotGUI2009_guidesignFrame)
    //*)
}

void RobotGUI2009_guidesignFrame::OnQuit(wxCommandEvent& event)
{
	//Send Command to disconnect all OpenMORA modules
	CCriticalSectionLocker lock(& m_theWxApp->m_lstToSendToMOOS_cs );
	m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("SHUTDOWN"),string("true")) );
	delete boardSCE;
	mrpt::system::sleep(500);
    Close();
}

void RobotGUI2009_guidesignFrame::OnAbout(wxCommandEvent& event)
{

}

void RobotGUI2009_guidesignFrame::OnbtnEnableJoyClick(wxCommandEvent& event)
{
	static bool enableJoy = false;
	enableJoy = !enableJoy;
	btnJoystickMode->SetValue( enableJoy );

	CCriticalSectionLocker lock(& m_theWxApp->m_lstToSendToMOOS_cs );
	m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("JOYSTICK_MODE"),string( enableJoy ? "1":"0" )));
}

void RobotGUI2009_guidesignFrame::OnReactiveNavCmd(wxCommandEvent& event)
{
	if (m_cursorPickState!=cpsReactivePickTarget)
	{
		m_cursorPickState = cpsReactivePickTarget;
		m_glMap->SetCursor( *wxCROSS_CURSOR );
	}
	else
	{	// Cancel:
		m_cursorPickState = cpsNone;
		m_glMap->SetCursor( *wxSTANDARD_CURSOR );
		m_gl_placing_nav_target->setVisibility(false);
	}
	this->btnNavCmd->SetValue( m_cursorPickState == cpsReactivePickTarget );
	this->btnNavCmd->Refresh();
}

void RobotGUI2009_guidesignFrame::OnStopNavigator(wxCommandEvent& event)
{
	CCriticalSectionLocker lock(& m_theWxApp->m_lstToSendToMOOS_cs );
	m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("PNAVIGATORREACTIVEPTG3D_CMD"),string("CANCEL")));
	m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("PNAVIGATORREACTIVEPTG_CMD"),string("CANCEL")));
	//JGMonroy
	m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("CANCEL_NAVIGATION"),string("GUI - Stop button")));	
}

void RobotGUI2009_guidesignFrame::OntimUpdateFromMOOSTrigger(wxTimerEvent& event)
{
	static int nFails = 0;

	if (!m_theWxApp->theMOOSApp.IsConnectedToMOOSDB())
	{
		nFails++;
		if (nFails>25)
		{
			this->StatusBar1->SetStatusText(_("It seems that there is no connection to MOOSDB") );
			nFails = 0;
		}
	}
	else nFails=0;

	// Process data from MOOSDB:
	m_refresh_3d = false;
	{
		CCriticalSectionLocker lock(& m_theWxApp->m_lstFromMOOS_cs);

		for (map<string,string>::const_iterator it=m_theWxApp->m_lstFromMOOS.begin();it!=m_theWxApp->m_lstFromMOOS.end();it++)
		{
			try
			{
				processNewMOOSVar(it->first,it->second);	// Process it:
			}
			catch(std::exception &e)
			{
				cerr << e.what() << endl;
				this->edSystemLog->AppendText( _U(( e.what() ) ));
			}
		}
		m_theWxApp->m_lstFromMOOS.clear();
	}

	//if (m_refresh_2d) m_plotMap->Refresh();
	if (m_refresh_3d) m_glMap->Refresh();
}

void RobotGUI2009_guidesignFrame::OnBtnRelocalizeWithPF(wxCommandEvent& event)
{
//	CCriticalSectionLocker lock(& m_theWxApp->m_lstToSendToMOOS_cs );
//	m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("PLOCALIZATIONPF_CMD"),string("RESTART")));
	if (m_cursorPickState!=cpsRelocalizePickArea)
	{
		m_cursorPickState = cpsRelocalizePickArea;
		m_glMap->SetCursor( *wxCROSS_CURSOR );
	}
	else
	{	// Cancel:
		m_cursorPickState = cpsNone;
		m_glMap->SetCursor( *wxSTANDARD_CURSOR );
		m_gl_relocalizing_area->setVisibility(false);
	}
	btnRelocalize->SetValue(m_cursorPickState == cpsRelocalizePickArea );
	btnRelocalize->Refresh();

}

void RobotGUI2009_guidesignFrame::OnSendResetSimulator(wxCommandEvent& event)
{
	CCriticalSectionLocker lock(& m_theWxApp->m_lstToSendToMOOS_cs );
	m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("PMOBILEROBOT_SIMUL_CMD"),string("RESTART")));
}

void RobotGUI2009_guidesignFrame::OnplotMouseMove(wxMouseEvent& event)
{
	int X, Y;
	event.GetPosition(&X,&Y);

	// Intersection of 3D ray with ground plane ====================
	TLine3D ray;
	m_glMap->m_openGLScene->getViewport("main")->get3DRayForPixelCoord( X,Y,ray);
	// Create a 3D plane, e.g. Z=0
	const TPlane ground_plane(TPoint3D(0,0,0),TPoint3D(1,0,0),TPoint3D(0,1,0));
	// Intersection of the line with the plane:
	TObject3D inters;
	intersect(ray,ground_plane, inters);
	// Interpret the intersection as a point, if there is an intersection:
	TPoint3D inters_pt;
	if (inters.getPoint(inters_pt))
	{
		m_curCursorPos.x = inters_pt.x;
		m_curCursorPos.y = inters_pt.y;

		if (m_cursorPickState==cpsReactivePickTarget)
		{
			m_gl_placing_nav_target->setVisibility(true);
			m_gl_placing_nav_target->setLocation(m_curCursorPos.x,m_curCursorPos.y,0.10);
		}
		else if (m_cursorPickState==cpsRelocalizePickArea)
		{
			m_gl_relocalizing_area->setVisibility(true);
			m_gl_relocalizing_area->setLocation(m_curCursorPos.x,m_curCursorPos.y,0.10);
		}

		StatusBar1->SetStatusText(wxString::Format(wxT("X=%.03f Y=%.04f Z=0"),m_curCursorPos.x,m_curCursorPos.y), 0);
	}

	// Do normal process in that class:
	m_glMap->OnMouseMove(event);
}

void RobotGUI2009_guidesignFrame::OnplotMouseClick(wxMouseEvent& event)
{
	std::string pos;
	m_curCursorPos.asString(pos);

	if (node_selected && btnEditNode->GetValue())
	{

				sendTopologyEdditNodeToMOOS(m_curCursorPos);
				this->edSystemLog->AppendText(_U(format("Move node at: %s\n",pos.c_str()).c_str() )  );
				node_selected=false;
				btnEditNode->SetValue(false);
				btnEditNode->Refresh();
	}

	switch (m_cursorPickState)
	{
	case cpsReactivePickTarget:
		{
			btnNavCmd->SetValue(false);
			btnNavCmd->Refresh();
			m_gl_placing_nav_target->setVisibility(false);

			// By now "m_curCursorPos" should have the target pos:
			sendReactiveNavTargetToMOOS(m_curCursorPos);
			break;
		}
	case cpsRelocalizePickArea:
		{
			btnRelocalize->SetValue(false);
			btnRelocalize->Refresh();
			m_gl_relocalizing_area->setVisibility(false);

			// By now "m_curCursorPos" should have the target pos:
			sendRelocalizeCommand(m_curCursorPos);

			break;
		}
	case cpsNewNode:
		{
			btnNewNode->SetValue(false);
			btnNewNode->Refresh();
			std::string pos;
			m_curCursorPos.asString(pos);
			this->edSystemLog->AppendText(_U(format("Creating node at: %s\n",pos.c_str()).c_str()));
			sendTopologyAddNodeToMOOS(m_curCursorPos);
			break;
		}
		case cpsEditNode:
		{
			btnEditNode->SetValue(true);
			btnEditNode->Refresh();
			this->edSystemLog->AppendText(_U(format("Select node at: %s\n",pos.c_str()).c_str() )  );
			selectNode(m_curCursorPos);
			m_gl_selected_topo_node->setColor(0,1,0);
			node_selected=true;
			break;
		}
		case cpsPlaceRobot:
		{
			this->edSystemLog->AppendText(_U(format("Changing robot position to: \n").c_str() )  );
			btnPlaceRobot->SetValue(true);
			btnPlaceRobot->Refresh();

			CCriticalSectionLocker lock(& m_theWxApp->m_lstToSendToMOOS_cs );
			{
				selectNode(m_curCursorPos);
				std::string s=m_gl_selected_topo_node->getName();
				m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("ROBOT_TOPOLOGICAL_PLACE"),s));
				this->edSystemLog->AppendText(_U(format("Changed robot position to: %s\n",s.c_str()).c_str() )  );
			}

			btnPlaceRobot->SetValue(false);
			btnPlaceRobot->Refresh();

			break;
		}

	default:
			// Do normal process in that class:
			m_glMap->OnMouseDown(event);			
	}

	m_glMap->SetCursor( *wxSTANDARD_CURSOR ); // End of cross cursor
	m_cursorPickState = cpsNone; // end of mode

	// Do normal process in that class:
	m_glMap->OnMouseDown(event);
}
void RobotGUI2009_guidesignFrame::sendTopologyEdditNodeToMOOS(const mrpt::math::TPoint2D &trg)
{
	CCriticalSectionLocker lock(& m_theWxApp->m_lstToSendToMOOS_cs );

	const string s = format("%s %.03f %.03f", m_gl_selected_topo_node->getName().c_str(), trg.x, trg.y);
	m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("MOVE_NODE"),s));
	this->edSystemLog->AppendText(_U(format("Sent %s\n",s.c_str()).c_str()  ));

}
void RobotGUI2009_guidesignFrame::selectNode(const mrpt::math::TPoint2D &trg)
{
	CSetOfObjects::iterator it, itaux = m_gl_topological_map->end();

	double min=1000;

	for (it=m_gl_topological_map->begin();it!=m_gl_topological_map->end();it++)
	{
		if (  IS_CLASS(*it, CSphere) )
		{

			double x=(*it)->getPoseX();
			double y=(*it)->getPoseY();
			TPoint2D loc;
			loc.x=x;loc.y=y;

			if (math::distance(loc,trg) < min)
			{
				itaux=it;
				min=math::distance(loc,trg);
			}
			}
	}

	if (itaux != m_gl_topological_map->end())
		m_gl_selected_topo_node=(*itaux);

	m_glMap->Refresh();

}
void RobotGUI2009_guidesignFrame::sendTopologyAddNodeToMOOS(const mrpt::math::TPoint2D &trg)
{
	CCriticalSectionLocker lock(& m_theWxApp->m_lstToSendToMOOS_cs );

	wxString nodename = wxGetTextFromUser(_("Enter node's label"),_("Add Node"),_(""),this );

	const string s = format("%s Place %.03f %.03f", std::string( nodename.mbc_str()).c_str(), trg.x, trg.y);
	m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("ADD_NODE"),s));
	this->edSystemLog->AppendText(_U(format("Sent %s\n",s.c_str()).c_str()  ));

}
void RobotGUI2009_guidesignFrame::sendReactiveNavTargetToMOOS(const mrpt::math::TPoint2D &trg)
{
	CCriticalSectionLocker lock(& m_theWxApp->m_lstToSendToMOOS_cs );
	const string s = format("[%.03f %.03f]", trg.x, trg.y);
	m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("NAVIGATE_TARGET"),s));
}

// Update list of modules from data in "m_modules_ping_info"
void RobotGUI2009_guidesignFrame::OnTimerUpdateListModules(wxTimerEvent& event)
{
	const size_t nMods =  m_modules_ping_info.size() ;
	if (!nMods) return;

	gridRunningMods->BeginBatch();

	// Number of cols: fixed
	if (gridRunningMods->GetNumberCols()!=2)
	{
		gridRunningMods->DeleteCols(0,gridRunningMods->GetNumberCols());
		gridRunningMods->AppendCols(2);
		gridRunningMods->SetColSize(0,120);
		gridRunningMods->SetColSize(1,50);
		gridRunningMods->SetColLabelValue(0,wxT("Module"));
		gridRunningMods->SetColLabelValue(1,wxT("Up-time"));
		//	gridRunningMods->SetColSize(2,50);
		//	gridRunningMods->SetColLabelValue(2,wxT("Error?"));
	}

	// Number of rows:
	if (size_t(gridRunningMods->GetNumberRows())>nMods)
		gridRunningMods->DeleteRows(nMods-1,gridRunningMods->GetNumberRows()-nMods);
	else if (size_t(gridRunningMods->GetNumberRows())<nMods)
		gridRunningMods->AppendRows( nMods-gridRunningMods->GetNumberRows() );

	unsigned int i=0;
	std::map<std::string,TInfoPerRunningModule>::const_iterator it;
	for (it=m_modules_ping_info.begin(),i=0;i<nMods;++i,++it)
	{
		const string &modName = it->first;
		const TInfoPerRunningModule &inf = it->second;

		gridRunningMods->SetCellValue(i,0, _U(modName.c_str()) );

		gridRunningMods->SetCellValue(i,1, _U( mrpt::system::formatTimeInterval(inf.upTime).c_str() ) );
		gridRunningMods->SetCellAlignment(i,1,wxALIGN_RIGHT);

//		gridRunningMods->SetCellValue(i,2, inf.appError ? wxT("YES"):wxT("NO") );
//		gridRunningMods->SetCellBackgroundColour(i,2, inf.appError ? (*wxRED) : (*wxWHITE) );
	}

	gridRunningMods->EndBatch();
	gridRunningMods->Refresh();


}

void RobotGUI2009_guidesignFrame::sendRelocalizeCommand(const mrpt::math::TPoint2D &trg)
{
	CCriticalSectionLocker lock(& m_theWxApp->m_lstToSendToMOOS_cs );
	const unsigned int nParts = 20000;
	const string s = format("[%.03f %.03f %.03f %.03f  %u]",
		trg.x - RELOCALIZ_AREA_WIDTH *0.5,
		trg.x + RELOCALIZ_AREA_WIDTH *0.5,
		trg.y - RELOCALIZ_AREA_WIDTH *0.5,
		trg.y + RELOCALIZ_AREA_WIDTH *0.5,
		nParts );

	//! @moos_publish RELOCALIZE_IN_AREA Relocalize the robot in the given area. Var value is: "[x_min x_max y_min y_max]"
	m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("RELOCALIZE_IN_AREA"),s));
}

// The user changed the enable ticks for the 3D visualization.
// Update the "visible" field in m_gl_* objects.
void RobotGUI2009_guidesignFrame::OncblistViewEnablesToggled(wxCommandEvent& event)
{
	for (size_t i=0;i<cblistViewEnables->GetCount();i++)
	{
		const string nam = string( cblistViewEnables->GetString(i).mbc_str() );
		const bool   visible = cblistViewEnables->IsChecked(i);
		mrpt::opengl::CRenderizablePtr  p;

		if (m_mapNames2GlObjects.find(nam)!=m_mapNames2GlObjects.end())
			m_mapNames2GlObjects[nam]->setVisibility(visible);

		// also, update the variables in the MOOS DB:
		if (!m_ignoreClicksVisibleCheckBox)
			m_theWxApp->m_lstToSendToMOOS.push( make_pair(format("GUI_VISIBLE_%s",nam.c_str()),string(visible ? "1":"0")));
	}
	m_glMap->Refresh();
}

void RobotGUI2009_guidesignFrame::addObjectToListOfVisibleCheckboxs(const std::string &display_name, const mrpt::opengl::CRenderizablePtr &gl_obj, bool default_visible)
{
	m_mapNames2GlObjects[display_name] = gl_obj;
	if (default_visible)
			cblistViewEnables->Check(cblistViewEnables->Append(_U(display_name.c_str())));
	else 	cblistViewEnables->Append(_U(display_name.c_str()));
}

void RobotGUI2009_guidesignFrame::OnbtnZoomInClick(wxCommandEvent& event)
{
	m_glMap->cameraZoomDistance *= 0.95;
	m_glMap->Refresh();
}

void RobotGUI2009_guidesignFrame::OnbtnZoomOutClick(wxCommandEvent& event)
{
	m_glMap->cameraZoomDistance *= 1.05;
	m_glMap->Refresh();
}

void RobotGUI2009_guidesignFrame::OntimInitTrigger(wxTimerEvent& event)
{
	wxSizeEvent dum;
	OnResize(dum);
}

void RobotGUI2009_guidesignFrame::OncbCameraFollowsRobotClick(wxCommandEvent& event)
{
}

void RobotGUI2009_guidesignFrame::OnMove(wxMoveEvent& event)
{
	wxSizeEvent dum;
	OnResize(dum);
	event.Skip(true);
}

//Publish the glMap screen coordinates to the GUI_POS_SIZE variable.
void RobotGUI2009_guidesignFrame::OnResize(wxSizeEvent& event)
{
	wxRect rect;
	std::ostringstream s;

	if(m_glMap)
	{
		rect = m_glMap->GetScreenRect();
		s << rect.GetX() <<','<< rect.GetY() <<','<< rect.GetWidth() <<','<< rect.GetHeight() << endl;
		m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("GUI_POS_SIZE"),s.str().c_str()));
	}

	event.Skip(true); // Keep processing this event in base classes
}

// Do camera animation on MOOS var "GUI_SET_CAMERA"
void RobotGUI2009_guidesignFrame::OnTimerDoCameraAnimation(wxTimerEvent& event)
{
	const double animLen = mrpt::system::timeDifference(m_camanim_timStart, mrpt::system::now() );

	bool continueAnim = true;

	if (animLen>=m_camanim_animLength_secs)
	{
		// Go straight to the final camera pose:
		m_glMap->cameraAzimuthDeg   = m_camanim_new->getAzimuthDegrees();
		m_glMap->cameraElevationDeg = m_camanim_new->getElevationDegrees();
		m_glMap->cameraZoomDistance = m_camanim_new->getZoomDistance();
		m_glMap->cameraPointingX    = m_camanim_new->getPointingAtX();
		m_glMap->cameraPointingY    = m_camanim_new->getPointingAtY();
		m_glMap->cameraPointingZ    = m_camanim_new->getPointingAtZ();

		continueAnim=false;
	}
	else
	{
		// Interpolate camera pose:
		m_glMap->cameraAzimuthDeg   = RAD2DEG(mrpt::math::interpolate2points(animLen, 0,DEG2RAD(m_camanim_old->getAzimuthDegrees()),m_camanim_animLength_secs, DEG2RAD(m_camanim_new->getAzimuthDegrees()), true /* wrap 2PI */ ));
		m_glMap->cameraElevationDeg = RAD2DEG(mrpt::math::interpolate2points(animLen, 0,DEG2RAD(m_camanim_old->getElevationDegrees()),m_camanim_animLength_secs, DEG2RAD(m_camanim_new->getElevationDegrees()), true /* wrap 2PI */ ));
		m_glMap->cameraZoomDistance = mrpt::math::interpolate2points(animLen, 0,m_camanim_old->getZoomDistance(),m_camanim_animLength_secs,m_camanim_new->getZoomDistance() );
		m_glMap->cameraPointingX    = mrpt::math::interpolate2points(animLen, 0,m_camanim_old->getPointingAtX(),m_camanim_animLength_secs,m_camanim_new->getPointingAtX() );
		m_glMap->cameraPointingY    = mrpt::math::interpolate2points(animLen, 0,m_camanim_old->getPointingAtY(),m_camanim_animLength_secs,m_camanim_new->getPointingAtY() );
		m_glMap->cameraPointingZ    = mrpt::math::interpolate2points(animLen, 0,m_camanim_old->getPointingAtZ(),m_camanim_animLength_secs,m_camanim_new->getPointingAtZ() );

	}

	// Refresh 3D view:
	m_glMap->Refresh();

	// One more shot:
	if (continueAnim)
		timDoCameraAnim.Start(100, true);
}

void RobotGUI2009_guidesignFrame::OnPanel2Paint(wxPaintEvent& event)
{
}

void RobotGUI2009_guidesignFrame::OnNotebook2PageChanged(wxNotebookEvent& event)
{
}

void RobotGUI2009_guidesignFrame::OnNotebook2PageChanged1(wxNotebookEvent& event)
{
	// Repaint the paint board
	boardSCE->repaint();
}

void RobotGUI2009_guidesignFrame::OnbtnNewNodeClick(wxCommandEvent& event)
{
	if (m_cursorPickState!=cpsNewNode)
	{
		m_cursorPickState = cpsNewNode;
		m_glMap->SetCursor( wxCURSOR_PAINT_BRUSH );
	}
	else
	{	// Cancel:
		m_cursorPickState = cpsNone;
		m_glMap->SetCursor( *wxSTANDARD_CURSOR );

	}
	btnNewNode->Refresh();

}

void RobotGUI2009_guidesignFrame::OnbtnEditNodeClick(wxCommandEvent& event)
{
	if (m_cursorPickState!=cpsEditNode)
	{
		m_cursorPickState = cpsEditNode;
		m_glMap->SetCursor( wxCURSOR_PAINT_BRUSH );
	}
	else
	{	// Cancel:
		m_cursorPickState = cpsNone;
		m_glMap->SetCursor( *wxSTANDARD_CURSOR );

	}
	btnEditNode->SetValue(true);
	btnEditNode->Refresh();

}

void RobotGUI2009_guidesignFrame::OnbtnSaveTopoClick(wxCommandEvent& event)
{
	CCriticalSectionLocker lock(& m_theWxApp->m_lstToSendToMOOS_cs );


	wxFileDialog openFileDialog( this, _("Save file"), _(""), _(""), _(""),wxFD_SAVE);
	if ( openFileDialog.ShowModal() == wxID_OK )
	{
		std::string filename=std::string(openFileDialog.GetFilename().mbc_str());

		this->edSystemLog->AppendText(_U(format("Save graph to %s\n",filename.c_str()).c_str()));

		m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("SAVE_GRAPH"),filename));

	}

}

void RobotGUI2009_guidesignFrame::OnbtnLoadTopoClick(wxCommandEvent& event)
{
    CCriticalSectionLocker lock(& m_theWxApp->m_lstToSendToMOOS_cs );


	wxFileDialog openFileDialog( this, _("Load file"), _(""), _(""), _(""));
	if ( openFileDialog.ShowModal() == wxID_OK )
	{
		std::string filename=std::string(openFileDialog.GetFilename().mbc_str());

		this->edSystemLog->AppendText(_U(format("Load graph from %s\n",filename.c_str()).c_str()));

		m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("LOAD_GRAPH"),filename));

	}
}

void RobotGUI2009_guidesignFrame::OnbtnPlaceRobotClick(wxCommandEvent& event)
{

	m_cursorPickState = cpsPlaceRobot;
	btnPlaceRobot->SetValue(true);
	btnPlaceRobot->Refresh();
}


// NEW STATE CHART EDITOR

void RobotGUI2009_guidesignFrame::OnStateButton(wxCommandEvent & event)
{
    boardSCE->setMState(STATE);
}

void RobotGUI2009_guidesignFrame::OnLinkButton(wxCommandEvent & event)
{
    boardSCE->setMState(LINK);
}

void RobotGUI2009_guidesignFrame::OnSaveButton(wxCommandEvent & event)
{

    //boardSCE->SaveXML()
    boardSCE->SaveXML();
}

void RobotGUI2009_guidesignFrame::OnPublishButton(wxCommandEvent & event)
{
    std::string filename;
    filename = boardSCE->SaveXML();
    m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("OBJECTIVE_FILE"),filename));
}

void RobotGUI2009_guidesignFrame::OnOpen(wxCommandEvent & event)
{
    boardSCE->openStateChart();
}

void RobotGUI2009_guidesignFrame::OnQuitButton(wxCommandEvent & event)
{

}
void RobotGUI2009_guidesignFrame::OnSaveAsButton(wxCommandEvent & event)
{
    wxString filename(_("/home/emil/mision.scxml"));
    boardSCE->setSaved(false);
    boardSCE->SaveXML();
}

void RobotGUI2009_guidesignFrame::OnMission1Button(wxCommandEvent & event)
{
	turnAllSimpleButtonsOff();
	mission1->SetBackgroundColour(wxColour(0,255,0));
	m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("OBJECTIVE_FILE"),missionList[0]));
}
void RobotGUI2009_guidesignFrame::OnMission2Button(wxCommandEvent & event)
{
	turnAllSimpleButtonsOff();
	mission2->SetBackgroundColour(wxColour(0,255,0));
	m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("OBJECTIVE_FILE"),missionList[1]));
}
void RobotGUI2009_guidesignFrame::OnMission3Button(wxCommandEvent & event)
{
	turnAllSimpleButtonsOff();
	mission3->SetBackgroundColour(wxColour(0,255,0));
	m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("OBJECTIVE_FILE"),missionList[2]));

}
void RobotGUI2009_guidesignFrame::OnMission4Button(wxCommandEvent & event)
{
	turnAllSimpleButtonsOff();
	mission4->SetBackgroundColour(wxColour(0,255,0));
	m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("OBJECTIVE_FILE"),missionList[3]));

}
void RobotGUI2009_guidesignFrame::OnMission5Button(wxCommandEvent & event)
{
	turnAllSimpleButtonsOff();
	mission5->SetBackgroundColour(wxColour(0,255,0));
	m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("OBJECTIVE_FILE"),missionList[4]));

}
void RobotGUI2009_guidesignFrame::OnMission6Button(wxCommandEvent & event)
{
	turnAllSimpleButtonsOff();
	mission6->SetBackgroundColour(wxColour(0,255,0));
	m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("OBJECTIVE_FILE"),missionList[5]));

}
void RobotGUI2009_guidesignFrame::OnSimpleStopButton(wxCommandEvent & event)
{
	turnAllSimpleButtonsOff();
	m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("PNAVIGATORREACTIVEPTG3D_CMD"),string("CANCEL")));
	m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("PNAVIGATORREACTIVEPTG_CMD"),string("CANCEL")));
//	m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("MOTION_CMD_V"),string("0")));
//	m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("MOTION_CMD_W"),string("0")));
	m_theWxApp->m_lstToSendToMOOS.push( make_pair(string("OBJECTIVE_FILE"),missionList[6]));
}
void RobotGUI2009_guidesignFrame::OnSimpleExpertButton(wxCommandEvent & event)
{
	Notebook2->ChangeSelection(0);
}

void RobotGUI2009_guidesignFrame::turnAllSimpleButtonsOff()
{
	mission1->SetBackgroundColour(wxNullColour);
	mission2->SetBackgroundColour(wxNullColour);
	mission3->SetBackgroundColour(wxNullColour);
	mission4->SetBackgroundColour(wxNullColour);
	mission5->SetBackgroundColour(wxNullColour);
	mission6->SetBackgroundColour(wxNullColour);
}
