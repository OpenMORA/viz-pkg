#include "CDTransicion.h"

//(*InternalHeaders(CDTransicion)
#include <wx/intl.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/string.h>
#include <wx/notebook.h>
#include <wx/stattext.h>
//*)

#include <string>
#include <sstream>

#include "PanelGraf.h"

using namespace std;
using namespace InterfazSancho;

//(*IdInit(CDTransicion)
const long CDTransicion::ID_ENTRANTE=wxNewId();
const long CDTransicion::ID_EVENTO=wxNewId();
const long CDTransicion::ID_CONDICION=wxNewId();
const long CDTransicion::ID_OK=wxNewId();
const long CDTransicion::ID_CANCEL=wxNewId();
//*)

BEGIN_EVENT_TABLE(CDTransicion,wxDialog)
	//(*EventTable(CDTransicion)
	//*)
END_EVENT_TABLE()

CDTransicion::CDTransicion(wxWindow* parent,const TransicionPtr &t):trans(t)	{
	ostringstream oss;
	EstadoPtr e;
	t->getSaliente(e);
	oss<<"Transition from "<<e->getNombre();
	//(*Initialize(CDTransicion)
	Create(parent,wxID_ANY,_U(oss.str().c_str()), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	wxSizer *cont=new wxBoxSizer(wxVERTICAL);
	wxNotebook *ac=new wxNotebook(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxNB_TOP);
	/* First tab */	{
		wxPanel *panel=new wxPanel(ac);
		wxGridSizer *container=new wxGridSizer(4,2,5,5);
		wxStaticText *label1=new wxStaticText(panel,wxID_ANY,_("Departure state: "),wxDefaultPosition,wxDefaultSize,wxALIGN_RIGHT);
		container->Add(label1,0,wxALIGN_CENTRE_VERTICAL);
		wxTextCtrl *tf=new wxTextCtrl(panel,wxID_ANY,_U(e->getNombre().c_str()),wxDefaultPosition,wxDefaultSize,wxTE_LEFT|wxEXPAND|wxTE_READONLY);
		tf->Disable();
		container->Add(tf);
		wxStaticText *label2=new wxStaticText(panel,wxID_ANY,_("Destination state: "),wxDefaultPosition,wxDefaultSize,wxALIGN_RIGHT);
		container->Add(label2,0,wxALIGN_CENTRE_VERTICAL);
		vector<wxString> strs;
		e->getPanel()->getTodosLosEstados(strs);
		strs.insert(strs.begin(),_(""));
		entrante=new wxComboBox(panel,ID_ENTRANTE,_U(trans->getEntranteString().c_str()),wxDefaultPosition,wxDefaultSize,strs.size(),&strs[0],wxCB_READONLY|wxEXPAND);
		container->Add(entrante);
		wxStaticText *label3=new wxStaticText(panel,wxID_ANY,_("Event: "),wxDefaultPosition,wxDefaultSize,wxALIGN_RIGHT);
		container->Add(label3,0,wxALIGN_CENTRE_VERTICAL);
		e->getPanel()->getTodosLosEventos(strs);
		strs.insert(strs.begin(),_(""));
		string tmp;
		trans->getEvento(tmp);
		evento=new wxComboBox(panel,ID_EVENTO,_U(tmp.c_str()),wxDefaultPosition,wxDefaultSize,strs.size(),&strs[0],wxCB_READONLY|wxEXPAND);
		container->Add(evento);
		wxStaticText *label4=new wxStaticText(panel,wxID_ANY,_("Condition: "),wxDefaultPosition,wxDefaultSize,wxALIGN_RIGHT);
		container->Add(label4,0,wxALIGN_CENTRE_VERTICAL);
		condicion=new wxTextCtrl(panel,ID_CONDICION,_(""),wxDefaultPosition,wxDefaultSize,wxTE_LEFT|wxEXPAND);
		trans->getCondicion(tmp);
		condicion->ChangeValue(_U(tmp.c_str()));
		container->Add(condicion);
		panel->SetSizer(container);
		container->Fit(panel);
		container->SetSizeHints(panel);
		ac->AddPage(panel,_("General configuration"));
	}
	ac->AddPage(tabSentencias=new TabSentenciasSimple(e,_(""),ac),_("Actions to take"));
	vector<AccionPtr> snts;
	trans->getListaSentencias(snts);
	tabSentencias->setSentencias(snts);
	cont->Add(ac);
	wxSizer *hb=new wxBoxSizer(wxHORIZONTAL);
	hb->AddStretchSpacer(1);
	wxButton *bAc=new wxButton(this,ID_OK,_("OK"));
	hb->Add(bAc,1);
	hb->AddStretchSpacer(1);
	wxButton *bCa=new wxButton(this,ID_CANCEL,_("Cancel"));
	hb->Add(bCa);
	hb->AddStretchSpacer(1);
	cont->Add(hb);
	Connect(ID_OK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CDTransicion::OnOK);
	Connect(ID_CANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CDTransicion::OnCancel);
	SetSizer(cont);
	cont->Fit(this);
	cont->SetSizeHints(this);
	ShowModal();
}

void CDTransicion::OnOK(wxEvent &)	{
	EstadoPtr tmp;
	trans->getSaliente(tmp);
	string s=string(entrante->GetValue().mbc_str());
	trans->setEntrante((s.length()==0)?EstadoPtr(NULL):tmp->getPanel()->getEstadoPorNombre(s));
	trans->setEvento(string(evento->GetValue().mbc_str()));
	trans->setCondicion(string(condicion->GetValue().mbc_str()));
	vector<AccionPtr> snts;
	tabSentencias->getSentencias(snts);
	trans->setListaSentencias(snts);
	tmp->getPanel()->Refresh();
	EndModal(wxID_OK);
}

void CDTransicion::OnCancel(wxEvent &)	{
	EndModal(wxID_CANCEL);
}
