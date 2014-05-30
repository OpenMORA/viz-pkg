#include "CDPedirEstadoDestino.h"

//(*InternalHeaders(CDPedirEstadoDestino)
#include <wx/intl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/msgdlg.h>
#include <wx/string.h>
#include <wx/stattext.h>

#include "PanelGraf.h"
//*)

using namespace std;
using namespace InterfazSancho;

//(*IdInit(CDPedirEstadoDestino)
const long CDPedirEstadoDestino::ID_COMBO=wxNewId();
const long CDPedirEstadoDestino::ID_OK=wxNewId();
const long CDPedirEstadoDestino::ID_CANCEL=wxNewId();
//*)

BEGIN_EVENT_TABLE(CDPedirEstadoDestino,wxDialog)
	//(*EventTable(CDPedirEstadoDestino)
	//*)
END_EVENT_TABLE()

//VERY IMPORTANT NOTE. THE CALLING PROCESS MUST GET THE STRING AND ACT ACCORDINGLY
CDPedirEstadoDestino::CDPedirEstadoDestino(PanelGraf* parent,const wxString &t,bool conCancel):panel(parent),selection("")	{
	//(*Initialize(CDPedirEstadoDestino)
	Create(parent,wxID_ANY,t, wxDefaultPosition, wxDefaultSize, wxCAPTION, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	//*)
	wxSizer *vsb=new wxBoxSizer(wxVERTICAL);
	vector<wxString> stateNames;
	panel->getTodosLosEstados(stateNames);
	stateNames.insert(stateNames.begin(),_(""));
	combo=new wxComboBox(this,ID_COMBO,_(""),wxDefaultPosition,wxDefaultSize,stateNames.size(),&stateNames[0],wxCB_READONLY|wxEXPAND);
	vsb->Add(combo,0,wxEXPAND);
	wxSizer *hsb=new wxBoxSizer(wxHORIZONTAL);
	hsb->AddStretchSpacer(1);
	wxButton *ac=new wxButton(this,ID_OK,_("OK"));
	hsb->Add(ac);
	hsb->AddStretchSpacer(1);
	if (conCancel)	{
		wxButton *ca=new wxButton(this,ID_CANCEL,_("Cancel"));
		hsb->Add(ca);
		hsb->AddStretchSpacer(1);
		Connect(ID_CANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CDPedirEstadoDestino::OnCancel);
	}
	Connect(ID_OK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CDPedirEstadoDestino::OnOK);
	vsb->Add(hsb);
	SetSizer(vsb);
	vsb->Fit(this);
	vsb->SetSizeHints(this);
	ShowModal();
}

void CDPedirEstadoDestino::OnOK(wxEvent &)	{
	selection=string(combo->GetValue().mbc_str());
	EndModal(wxID_OK);
}

void CDPedirEstadoDestino::OnCancel(wxEvent &)	{
	selection="";
	EndModal(wxID_CANCEL);
}
