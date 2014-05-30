#include "CDEstadoParalelo.h"

//(*InternalHeaders(CDEstadoParalelo)
#include <wx/intl.h>
#include <wx/sizer.h>
#include <wx/msgdlg.h>
#include <wx/string.h>
#include <wx/stattext.h>

#include <string>
#include <sstream>
//*)

#include "PanelGraf.h"

using namespace std;
using namespace InterfazSancho;

//(*IdInit(CDEstadoParalelo)
const long CDEstadoParalelo::ID_ADJ=wxNewId();
const long CDEstadoParalelo::ID_NOMBREESTADO=wxNewId();
const long CDEstadoParalelo::ID_OK=wxNewId();
const long CDEstadoParalelo::ID_CANCEL=wxNewId();
//*)

BEGIN_EVENT_TABLE(CDEstadoParalelo,wxDialog)
	//(*EventTable(CDEstadoParalelo)
	//*)
END_EVENT_TABLE()

CDEstadoParalelo::CDEstadoParalelo(const EstadoParaleloPtr &ep,wxWindow* parent):estado(ep)	{
	ostringstream oss;
	oss<<"Configuration: "<<estado->getNombre();
	//(*Initialize(CDEstadoParalelo)
	Create(parent,wxID_ANY,_U(oss.str().c_str()), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	//The main panel has two tabs. One of them contains the parallel state-specific configuration, while the other is intended to configure the actions of the state.
	wxNotebook *ac=new wxNotebook(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxNB_TOP);
	/* Base tab*/	{
		wxPanel *panel=new wxPanel(ac,wxID_ANY);
		wxSizer *container=new wxBoxSizer(wxVERTICAL);
		wxSizer *hb=new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *label=new wxStaticText(panel,wxID_ANY,_("Name: "),wxDefaultPosition,wxDefaultSize,wxALIGN_RIGHT);
		hb->Add(label,1,wxALIGN_CENTRE_VERTICAL);
		nombreEstado=new wxTextCtrl(panel,ID_NOMBREESTADO,_U(estado->getNombre().c_str()),wxDefaultPosition,wxDefaultSize,wxEXPAND|wxTE_LEFT);
		hb->Add(nombreEstado,5);
		container->Add(hb);
		hb=new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *label2=new wxStaticText(panel,wxID_ANY,_("Amount of branches: "),wxDefaultPosition,wxDefaultSize,wxALIGN_RIGHT);
		hb->Add(label2,1,wxALIGN_CENTRE_VERTICAL);
		vector<EstadoPtr> subs;
		ep->getSubestados(subs);
		adj=new wxSpinCtrl(panel,ID_ADJ,_(""),wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,2,20,subs.size());
		hb->Add(adj,3);
		container->Add(hb);
		panel->SetSizer(container);
		container->Fit(panel);
		container->SetSizeHints(panel);
		ac->AddPage(panel,_("General configuration"));
	}
	ac->AddPage(tabCodigo=new TabSentencias(estado,ac),_("Actions on entry and/or exit"));
	wxSizer *v=new wxBoxSizer(wxVERTICAL);
	v->Add(ac,0,wxEXPAND|wxALIGN_CENTRE_HORIZONTAL);
	wxSizer *h=new wxBoxSizer(wxHORIZONTAL);
	h->AddStretchSpacer(1);
	wxButton *bac=new wxButton(this,ID_OK,_("OK"));
	h->Add(bac,1);
	h->AddStretchSpacer(1);
	wxButton *bca=new wxButton(this,ID_CANCEL,_("Cancel"));
	h->Add(bca);
	h->AddStretchSpacer(1);
	v->Add(h,0,wxEXPAND|wxALIGN_CENTRE_HORIZONTAL);
	SetSizer(v);
	v->Fit(this);
	v->SetSizeHints(this);
	//*)
	Connect(ID_OK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CDEstadoParalelo::OnOK);
	Connect(ID_CANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CDEstadoParalelo::OnCancel);
	ShowModal();
}

void CDEstadoParalelo::OnOK(wxEvent &event)	{
	string s(nombreEstado->GetValue().mbc_str());
	if (s.size()==0)	{
		wxMessageBox(_("The state must have a name"),_("Error"),wxICON_ERROR);
		nombreEstado->SetFocus();
		return;
	}
	EstadoPtr ne=estado->getPanel()->getEstadoPorNombre(s);
	if (ne&&(ne.pointer()!=estado.pointer()))	{
		wxMessageBox(_("There already exists a state with that name."),_("Error"),wxICON_ERROR);
		nombreEstado->SetFocus();
		return;
	}
	estado->setNombre(s);
	vector<AccionPtr> snts;
	tabCodigo->getOnentry(snts);
	estado->setOnentry(snts);
	tabCodigo->getOnexit(snts);
	estado->setOnexit(snts);
	estado->redimensionar(adj->GetValue());
	EndModal(ID_OK);
}

void CDEstadoParalelo::OnCancel(wxEvent &event)	{
	EndModal(ID_CANCEL);
}
