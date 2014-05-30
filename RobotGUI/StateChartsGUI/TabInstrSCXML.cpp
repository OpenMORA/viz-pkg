#include "TabInstrSCXML.h"

//(*InternalHeaders(TabInstrSCXML)
#include <wx/intl.h>
#include <wx/button.h>
#include <wx/msgdlg.h>
#include <wx/string.h>
#include <wx/stattext.h>
//*)

#include <cmath>
#include "Main.h"
#include <cstdio>
#include <utility>
#include "Acciones.h"

//(*IdInit(TabInstrSCXML)
//*)

using namespace std;
using namespace mrpt::math;
using namespace InterfazSancho;

//CArray<string,7> TabInstrSCXML::acciones=TabInstrSCXML::generarAcciones();
wxString TabInstrSCXML::acciones[7]={_("If"),_("Elseif"),_("Else"),_("Endif"),_("Assign"),_("Send"),_("Event")};

const long TabInstrSCXML::ID_COMBOPRINCIPAL=wxNewId();
const long TabInstrSCXML::ID_IF_CONDICION=wxNewId();
const long TabInstrSCXML::ID_ELSEIF_CONDICION=wxNewId();
const long TabInstrSCXML::ID_ASSIGN_VARIABLE=wxNewId();
const long TabInstrSCXML::ID_ASSIGN_EXPRESION=wxNewId();
const long TabInstrSCXML::ID_SEND_EVENTO=wxNewId();
const long TabInstrSCXML::ID_SEND_DELAY=wxNewId();
const long TabInstrSCXML::ID_EVENT_EVENTO=wxNewId();
const long CDTabInstrSCXML::ID_OK=wxNewId();
const long CDTabInstrSCXML::ID_CANCEL=wxNewId();

BEGIN_EVENT_TABLE(TabInstrSCXML,wxPanel)
	//(*EventTable(TabInstrSCXML)
	//*)
END_EVENT_TABLE()

TabInstrSCXML::TabInstrSCXML(wxWindow* parent,const Main *m,const AccionPtr &instr,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(TabInstrSCXML)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	comboPrincipal=new wxChoicebook(this,ID_COMBOPRINCIPAL,wxDefaultPosition,wxDefaultSize,wxCHB_TOP|wxEXPAND,_("ID_COMBOPRINCIPAL"));
	wxColour rojo(255,0,0);
	size_t N;
	wxFont f;
	vector<wxString> choices;
	/* "If" instruction */	{
		wxPanel *panel=new wxPanel(this);
		wxBoxSizer *v=new wxBoxSizer(wxVERTICAL);
		wxBoxSizer *h=new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *labelIfCondition=new wxStaticText(panel,wxID_ANY,_("Condition: "),wxDefaultPosition,wxDefaultSize,wxEXPAND|wxALIGN_RIGHT);
		h->Add(labelIfCondition,1,wxALIGN_CENTRE_VERTICAL);
		ifCondicion=new wxTextCtrl(panel,ID_IF_CONDICION,_(""),wxDefaultPosition,wxDefaultSize,wxEXPAND|wxTE_LEFT);
		h->Add(ifCondicion,5);
		v->Add(h);
		wxStaticText *labelIfWarning=new wxStaticText(panel,wxID_ANY,_("Every If block must be finished with and Endif instruction."),wxDefaultPosition,wxDefaultSize,wxEXPAND|wxTE_CENTRE);
		labelIfWarning->SetForegroundColour(rojo);
		f=labelIfWarning->GetFont();
		f.SetWeight(wxBOLD);
		labelIfWarning->SetFont(f);
		v->Add(labelIfWarning,0,wxALIGN_CENTRE_VERTICAL);
		panel->SetSizer(v);
		v->Fit(panel);
		v->SetSizeHints(panel);
		comboPrincipal->AddPage(panel,acciones[0]);
	}
	/* "Elseif" instruction */	{
		wxPanel *panel=new wxPanel(this);
		wxBoxSizer *v=new wxBoxSizer(wxVERTICAL);
		wxBoxSizer *h=new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *labelElseifCondition=new wxStaticText(panel,wxID_ANY,_("Condition: "),wxDefaultPosition,wxDefaultSize,wxEXPAND|wxALIGN_CENTRE_VERTICAL|wxALIGN_RIGHT);
		h->Add(labelElseifCondition,1,wxALIGN_CENTRE_VERTICAL);
		elseifCondicion=new wxTextCtrl(panel,ID_ELSEIF_CONDICION,_(""),wxDefaultPosition,wxDefaultSize,wxEXPAND|wxTE_LEFT);
		h->Add(elseifCondicion,5);
		v->Add(h);
		wxStaticText *labelElseifWarning=new wxStaticText(panel,wxID_ANY,_("Every Elseif block must appear between an If/Elseif block and an Else or Endif instruction."),wxDefaultPosition,wxDefaultSize,wxEXPAND|wxALIGN_CENTRE);
		labelElseifWarning->SetForegroundColour(rojo);
		labelElseifWarning->SetFont(f);
		v->Add(labelElseifWarning,wxALIGN_CENTRE_VERTICAL);
		panel->SetSizer(v);
		v->Fit(panel);
		v->SetSizeHints(panel);
		comboPrincipal->AddPage(panel,acciones[1]);
	}
	/* "Else" instruction */	{
		wxPanel *panel=new wxPanel(this);
		wxBoxSizer *v=new wxBoxSizer(wxVERTICAL);
		wxStaticText *labelElseWarning=new wxStaticText(panel,wxID_ANY,_("When an Else block appears, it must always be the last of the If/Elseif blocks."),wxDefaultPosition,wxDefaultSize,wxEXPAND|wxALIGN_CENTRE);
		labelElseWarning->SetForegroundColour(rojo);
		labelElseWarning->SetFont(f);
		v->Add(labelElseWarning,0,wxALIGN_CENTRE_VERTICAL);
		panel->SetSizer(v);
		v->Fit(panel);
		v->SetSizeHints(panel);
		comboPrincipal->AddPage(panel,acciones[2]);
	}
	/* "Endif" instruction */	{
		wxPanel *panel=new wxPanel(this);
		wxBoxSizer *v=new wxBoxSizer(wxVERTICAL);
		wxStaticText *labelEndifWarning=new wxStaticText(panel,wxID_ANY,_("Endif must finish an If/Elseif/Else block."),wxDefaultPosition,wxDefaultSize,wxEXPAND|wxALIGN_CENTRE);
		labelEndifWarning->SetForegroundColour(rojo);
		labelEndifWarning->SetFont(f);
		v->Add(labelEndifWarning,0,wxALIGN_CENTRE_VERTICAL);
		panel->SetSizer(v);
		v->Fit(panel);
		v->SetSizeHints(panel);
		comboPrincipal->AddPage(panel,acciones[3]);
	}
	/* "Assign" instruction */	{
		wxPanel *panel=new wxPanel(this);
		wxBoxSizer *v=new wxBoxSizer(wxVERTICAL);
		wxBoxSizer *h=new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *labelAssignDestiny=new wxStaticText(panel,wxID_ANY,_("Destiny variable: "),wxDefaultPosition,wxDefaultSize,wxEXPAND|wxALIGN_RIGHT);
		h->Add(labelAssignDestiny,1,wxALIGN_CENTRE_VERTICAL);
		if (m)	{
			m->getVariables(choices);
			N=choices.size();
		}	else N=0;
		assignVariable=new wxComboBox(panel,ID_ASSIGN_VARIABLE,_(""),wxDefaultPosition,wxDefaultSize,N,(N==0)?NULL:&choices[0],wxEXPAND);
		assignVariable->SetEditable(false);
		h->Add(assignVariable,5);
		v->Add(h);
		h=new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *labelAssignExpression=new wxStaticText(panel,wxID_ANY,_("Expression: "),wxDefaultPosition,wxDefaultSize,wxEXPAND|wxALIGN_RIGHT);
		h->Add(labelAssignExpression,1,wxALIGN_CENTRE_VERTICAL);
		assignExpresion=new wxTextCtrl(panel,ID_ASSIGN_EXPRESION,_(""),wxDefaultPosition,wxDefaultSize,wxEXPAND|wxTE_LEFT);
		h->Add(assignExpresion,5);
		v->Add(h);
		panel->SetSizer(v);
		v->Fit(panel);
		v->SetSizeHints(panel);
		comboPrincipal->AddPage(panel,acciones[4]);
	}
	/* "Send" instruction */	{
		wxPanel *panel=new wxPanel(this);
		wxBoxSizer *v=new wxBoxSizer(wxVERTICAL);
		wxBoxSizer *h=new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *labelSendEvent=new wxStaticText(panel,wxID_ANY,_("Event name: "),wxDefaultPosition,wxDefaultSize,wxEXPAND|wxALIGN_RIGHT);
		h->Add(labelSendEvent,1,wxALIGN_CENTRE_VERTICAL);
		if (m)	{
			m->getEventos(choices);
			N=choices.size();
		}	else N=0;
		sendEvento=new wxComboBox(panel,ID_SEND_EVENTO,_(""),wxDefaultPosition,wxDefaultSize,N,(N==0)?NULL:&choices[0],wxEXPAND);
		sendEvento->SetEditable(false);
		h->Add(sendEvento,5);
		v->Add(h);
		h=new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *labelSendDelay=new wxStaticText(panel,wxID_ANY,_("Delay: "),wxDefaultPosition,wxDefaultSize,wxEXPAND|wxALIGN_RIGHT);
		h->Add(labelSendDelay,1,wxALIGN_CENTRE_VERTICAL);
		sendDelay=new wxTextCtrl(panel,ID_SEND_DELAY,_("0"),wxDefaultPosition,wxDefaultSize,wxEXPAND|wxTE_LEFT);
		h->Add(sendDelay,5);
		v->Add(h);
		panel->SetSizer(v);
		v->Fit(panel);
		v->SetSizeHints(panel);
		comboPrincipal->AddPage(panel,acciones[5]);
	}
	/* "Event" instruction */	{
		wxPanel *panel=new wxPanel(this);
		wxBoxSizer *v=new wxBoxSizer(wxVERTICAL);
		wxBoxSizer *h=new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *labelEventEvent=new wxStaticText(panel,wxID_ANY,_("Event name: "),wxDefaultPosition,wxDefaultSize,wxEXPAND|wxALIGN_RIGHT);
		h->Add(labelEventEvent,1,wxALIGN_CENTRE_VERTICAL);
		eventEvento=new wxComboBox(panel,ID_EVENT_EVENTO,_(""),wxDefaultPosition,wxDefaultSize,N,(N==0)?NULL:&choices[0],wxEXPAND|wxTE_LEFT);
		eventEvento->SetEditable(false);
		h->Add(eventEvento,5);
		v->Add(h);
		panel->SetSizer(v);
		v->Fit(panel);
		v->SetSizeHints(panel);
		comboPrincipal->AddPage(panel,acciones[6]);
	}
	wxSizer *vboxMain=new wxBoxSizer(wxVERTICAL);
	vboxMain->Add(comboPrincipal);
	SetSizer(vboxMain);
	vboxMain->Fit(this);
	vboxMain->SetSizeHints(this);
	
	if (instr)	{
		string par1,par2;
		if (IS_DERIVED(instr,AccionIf))	{
			comboPrincipal->SetSelection(0);
			instr->getParametro(par1);
			ifCondicion->SetValue(_U(par1.c_str()));
			elseifCondicion->SetValue(_U(par1.c_str()));
		}	else if (IS_DERIVED(instr,AccionElseif))	{
			comboPrincipal->SetSelection(1);
			instr->getParametro(par1);
			elseifCondicion->SetValue(_U(par1.c_str()));
			ifCondicion->SetValue(_U(par1.c_str()));
		}	else if (IS_DERIVED(instr,AccionElse)) comboPrincipal->SetSelection(2);
		else if (IS_DERIVED(instr,AccionEndif)) comboPrincipal->SetSelection(3);
		else if (IS_DERIVED(instr,AccionAssign))	{
			comboPrincipal->SetSelection(4);
			instr->getParametro(par1);
			instr->getParametro(par2);
			assignVariable->SetValue(_U(par1.c_str()));
			assignExpresion->SetValue(_U(par2.c_str()));
		}	else if (IS_DERIVED(instr,AccionSend))	{
			comboPrincipal->SetSelection(5);
			instr->getParametro(par1);
			instr->getParametro2(par2);
			sendEvento->SetValue(_U(par1.c_str()));
			eventEvento->SetValue(_U(par1.c_str()));
			sendDelay->SetValue(_U(par2.c_str()));
		}	else if (IS_DERIVED(instr,AccionEvent))	{
			comboPrincipal->SetSelection(6);
			instr->getParametro(par1);
			eventEvento->SetValue(_U(par1.c_str()));
			sendEvento->SetValue(_U(par1.c_str()));
		}
	}

	Connect(ID_IF_CONDICION,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&TabInstrSCXML::OnIfCondicion);
	Connect(ID_ELSEIF_CONDICION,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&TabInstrSCXML::OnElseifCondicion);
	Connect(ID_SEND_EVENTO,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&TabInstrSCXML::OnSendEvento);
	Connect(ID_EVENT_EVENTO,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&TabInstrSCXML::OnEventEvento);
}

void TabInstrSCXML::OnIfCondicion(wxEvent &event)	{
	elseifCondicion->ChangeValue(ifCondicion->GetValue());
}

void TabInstrSCXML::OnElseifCondicion(wxEvent &event)	{
	ifCondicion->ChangeValue(ifCondicion->GetValue());
}

void TabInstrSCXML::OnSendEvento(wxEvent &event)	{
	wxString sel=sendEvento->GetValue();
	if (eventEvento->GetValue()!=sel) eventEvento->SetValue(sel);
}

void TabInstrSCXML::OnEventEvento(wxEvent &event)	{
	wxString sel=eventEvento->GetValue();
	if (sendEvento->GetValue()!=sel) sendEvento->SetValue(sel);
}

AccionPtr TabInstrSCXML::getAccion()	{
	wxString str1,str2;
	int number;
	switch (currentSelection=comboPrincipal->GetSelection())	{
		case 0:return AccionPtr(((str1=ifCondicion->GetValue()).size()==0)?NULL:new AccionIf(string(str1.mbc_str())));
		case 1:return AccionPtr(((str1=elseifCondicion->GetValue()).size()==0)?NULL:new AccionElseif(string(str1.mbc_str())));
		case 2:return AccionPtr(new AccionElse());
		case 3:return AccionPtr(new AccionEndif());
		case 4:return AccionPtr(((str1=assignVariable->GetValue()).size()==0)?NULL:(((str2=assignExpresion->GetValue()).size()==0)?NULL:new AccionAssign(string(str1.mbc_str()),string(str2.mbc_str()))));
		case 5:return AccionPtr((((str1=sendEvento->GetValue()).size()==0)||((str2=sendDelay->GetValue()).size()==0))?NULL:((sscanf(str2.mbc_str(),"%d",&number)<=0)?NULL:new AccionSend(string(str1.mbc_str()),number)));
		case 6:return AccionPtr(((str1=eventEvento->GetValue()).size()==0)?NULL:new AccionEvent(string(str1.mbc_str())));
		default:return AccionPtr(NULL);
	}
}

CDTabInstrSCXML::CDTabInstrSCXML(wxWindow *parent,Main *main,const AccionPtr &instr,const wxString &tit)	{
	Create(parent,wxID_ANY,tit);
	wxSizer *sz=new wxBoxSizer(wxVERTICAL);
	base=new TabInstrSCXML(this,main,instr);
	sz->Add(base);
	wxSizer *h=new wxBoxSizer(wxHORIZONTAL);
	h->AddStretchSpacer(1);
	wxButton *ac=new wxButton(this,ID_OK,_("OK"));
	h->Add(ac);
	h->AddStretchSpacer(1);
	wxButton *ca=new wxButton(this,ID_CANCEL,_("Cancel"));
	h->Add(ca);
	h->AddStretchSpacer(1);
	sz->Add(h,0,wxALIGN_CENTRE_HORIZONTAL);
	SetSizer(sz);
	sz->Fit(this);
	sz->SetSizeHints(this);
	Connect(ID_OK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CDTabInstrSCXML::OnOK);
	Connect(ID_CANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CDTabInstrSCXML::OnCancel);
}

void CDTabInstrSCXML::OnOK(wxEvent &)	{
	EndModal(wxID_OK);
}

void CDTabInstrSCXML::OnCancel(wxEvent &)	{
	EndModal(wxID_CANCEL);
}
