#include "VentanaP.h"

//(*InternalHeaders(VentanaD)
#include <wx/intl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/string.h>
#include <wx/stattext.h>
//*)

#include "PanelGraf.h"

using namespace std;
using namespace InterfazSancho;

//(*IdInit(VentanaD)
const long VentanaP::ID_ADJ=wxNewId();
const long VentanaP::ID_CANCEL=wxNewId();
const long VentanaP::ID_OK=wxNewId();
//*)

BEGIN_EVENT_TABLE(VentanaP,wxDialog)
	//(*EventTable(VentanaD)
	//*)
END_EVENT_TABLE()

VentanaP::VentanaP(PanelGraf *p,int px,int py,const std::vector<DibujablePtr> &ep):jer(ep),vx(px),vy(py),padre(p)	{
	//(*Initialize(VentanaD)
	Create(p,wxID_ANY,_("Parallel state creation"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	//*)
	wxSizer *c=new wxBoxSizer(wxVERTICAL);
	wxSizer *h=new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *label=new wxStaticText(this,wxID_ANY,_("Enter number of branches: "),wxDefaultPosition,wxDefaultSize,wxALIGN_RIGHT);
	h->Add(label,0,wxALIGN_CENTRE_VERTICAL);
	s=new wxSpinCtrl(this,ID_ADJ,_(""),wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,2,20,2);
	h->Add(s);
	c->Add(h);
	h=new wxBoxSizer(wxHORIZONTAL);
	h->AddStretchSpacer(1);
	wxButton *ac=new wxButton(this,ID_OK,_("OK"));
	h->Add(ac,1);
	h->AddStretchSpacer(1);
	wxButton *ca=new wxButton(this,ID_CANCEL,_("Cancel"));
	h->Add(ca,1);
	h->AddStretchSpacer(1);
	c->Add(h);
	Connect(ID_OK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&VentanaP::OnOK);
	Connect(ID_CANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&VentanaP::OnCancel);
	SetSizer(c);
	c->Fit(this);
	c->SetSizeHints(this);
	ShowModal();
}

void VentanaP::OnOK(wxEvent &)	{
	padre->eventoCreacionParalelo(s->GetValue(),vx,vy,jer);
	EndModal(wxID_OK);
}

void VentanaP::OnCancel(wxEvent &)	{
	EndModal(wxID_CANCEL);
}
