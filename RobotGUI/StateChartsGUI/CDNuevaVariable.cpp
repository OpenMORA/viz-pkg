#include "CDNuevaVariable.h"

//(*InternalHeaders(CDNuevaVariable)
#include <wx/intl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/msgdlg.h>
#include <wx/string.h>
#include <wx/stattext.h>
//*)

using namespace std;
using namespace InterfazSancho;

//(*IdInit(CDNuevaVariable)
const long CDNuevaVariable::ID_NOMBRE=wxNewId();
const long CDNuevaVariable::ID_VALOR=wxNewId();
const long CDNuevaVariable::ID_OK=wxNewId();
const long CDNuevaVariable::ID_CANCEL=wxNewId();
//*)

BEGIN_EVENT_TABLE(CDNuevaVariable,wxDialog)
	//(*EventTable(CDNuevaVariable)
	//*)
END_EVENT_TABLE()

CDNuevaVariable::CDNuevaVariable(Main* parent):panel(parent)
{
	//(*Initialize(CDNuevaVariable)
	Create(parent,wxID_ANY,_("New variable"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	//*)
	wxFlexGridSizer *vb=new wxFlexGridSizer(2,1,5,5);
	vb->AddGrowableRow(0);
	vb->AddGrowableRow(1);
	wxFlexGridSizer *sizer=new wxFlexGridSizer(2,2,5,5);
	sizer->AddGrowableCol(0);
	sizer->AddGrowableRow(0);
	sizer->AddGrowableRow(1);
	wxStaticText *label1=new wxStaticText(this,wxID_ANY,_("Variable name: "),wxDefaultPosition,wxDefaultSize,wxALIGN_RIGHT);
	sizer->Add(label1,1,wxALIGN_CENTRE_VERTICAL);
	nombre=new wxTextCtrl(this,ID_NOMBRE,_(""),wxDefaultPosition,wxDefaultSize,wxTE_LEFT|wxEXPAND);
	sizer->Add(nombre,3);
	wxStaticText *label2=new wxStaticText(this,wxID_ANY,_("Initial value: "),wxDefaultPosition,wxDefaultSize,wxALIGN_RIGHT);
	sizer->Add(label2,1,wxALIGN_CENTRE_VERTICAL);
	valor=new wxTextCtrl(this,ID_VALOR,_("0"),wxDefaultPosition,wxDefaultSize,wxTE_LEFT|wxEXPAND);
	sizer->Add(valor,3,wxEXPAND);
	vb->Add(sizer,0,wxEXPAND);
	wxSizer *hb=new wxBoxSizer(wxHORIZONTAL);
	hb->AddSpacer(10);
	wxButton *ac=new wxButton(this,ID_OK,_("OK"));
	hb->Add(ac,1);
	hb->AddSpacer(10);
	wxButton *ca=new wxButton(this,ID_CANCEL,_("Cancel"));
	hb->Add(ca,1);
	hb->AddSpacer(10);
	vb->Add(hb,0,wxEXPAND);
	Connect(ID_OK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CDNuevaVariable::OnOK);
	Connect(ID_CANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CDNuevaVariable::OnCancel);
	SetSizer(vb);
	vb->Fit(this);
	vb->SetSizeHints(this);
	ShowModal();
}

void CDNuevaVariable::OnOK(wxEvent &)	{
	string n(nombre->GetValue().mbc_str()),v(valor->GetValue().mbc_str());
	if (n.length()==0)	{
		wxMessageBox(_("Variable name missing."),_("Error"),wxICON_ERROR);
		nombre->SetFocus();
		return;
	}	else if (v.length()==0)	{
		wxMessageBox(_("Variable value missing."),_("Error"),wxICON_ERROR);
		valor->SetFocus();
		return;
	}
	panel->nuevaVariable(n,v);
	EndModal(wxID_OK);
}

void CDNuevaVariable::OnCancel(wxEvent &)	{
	EndModal(wxID_CANCEL);
}
