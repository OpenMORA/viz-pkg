#include "CDNuevoEvento.h"

//(*InternalHeaders(CDNuevoEvento)
#include <wx/intl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/msgdlg.h>
#include <wx/string.h>
#include <wx/stattext.h>
//*)

#include <string>

using namespace std;
using namespace InterfazSancho;

//(*IdInit(CDNuevoEvento)
const long CDNuevoEvento::ID_NOMBRE=wxNewId();
const long CDNuevoEvento::ID_OK=wxNewId();
const long CDNuevoEvento::ID_CANCEL=wxNewId();
//*)

BEGIN_EVENT_TABLE(CDNuevoEvento,wxDialog)
	//(*EventTable(CDNuevoEvento)
	//*)
END_EVENT_TABLE()

CDNuevoEvento::CDNuevoEvento(Main* parent):panel(parent)	{
	//(*Initialize(CDNuevoEvento)
	Create(parent,wxID_ANY,_("New event"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	//*)
	wxSizer *vb=new wxBoxSizer(wxVERTICAL);
	wxSizer *hb=new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *label=new wxStaticText(this,wxID_ANY,_("Event name: "),wxDefaultPosition,wxDefaultSize,wxALIGN_RIGHT);
	hb->Add(label,1,wxALIGN_CENTRE_VERTICAL);
	nombre=new wxTextCtrl(this,ID_NOMBRE,_(""),wxDefaultPosition,wxDefaultSize,wxTE_LEFT|wxEXPAND);
	hb->Add(nombre,2);
	vb->Add(hb,0,wxALIGN_CENTRE_HORIZONTAL);
	hb=new wxBoxSizer(wxHORIZONTAL);
	hb->AddSpacer(10);
	wxButton *ac=new wxButton(this,ID_OK,_("OK"));
	hb->Add(ac,1);
	hb->AddSpacer(10);
	wxButton *ca=new wxButton(this,ID_CANCEL,_("Cancel"));
	hb->Add(ca,1);
	hb->AddSpacer(10);
	vb->Add(hb);
	SetSizer(vb);
	vb->Fit(this);
	vb->SetSizeHints(this);
	Connect(ID_OK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CDNuevoEvento::OnOK);
	Connect(ID_CANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CDNuevoEvento::OnCancel);
	ShowModal();
}

void CDNuevoEvento::OnOK(wxEvent &)	{
	string s=string(nombre->GetValue().mbc_str());
	if (s.length()==0)	{
		wxMessageBox(_("Event name missing"),_("Error"),wxICON_ERROR);
		nombre->SetFocus();
		return;
	}
	panel->nuevoEvento(s);
	EndModal(wxID_OK);
}

void CDNuevoEvento::OnCancel(wxEvent &)	{
	EndModal(wxID_CANCEL);
}

