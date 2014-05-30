#include "CDEstadoCompuesto.h"

//(*InternalHeaders(CDEstadoCompuesto)
#include <wx/intl.h>
#include <wx/sizer.h>
#include <wx/msgdlg.h>
#include <wx/string.h>
#include <wx/notebook.h>
#include <wx/stattext.h>
//*)

#include <sstream>

#include "PanelGraf.h"

//(*IdInit(CDEstadoCompuesto)
const long CDEstadoCompuesto::ID_COMBO=wxNewId();
const long CDEstadoCompuesto::ID_NOMBREESTADO=wxNewId();
const long CDEstadoCompuesto::ID_OK=wxNewId();
const long CDEstadoCompuesto::ID_CANCEL=wxNewId();
//*)

using namespace std;
using namespace InterfazSancho;

BEGIN_EVENT_TABLE(CDEstadoCompuesto,wxDialog)
	//(*EventTable(CDEstadoCompuesto)
	//*)
END_EVENT_TABLE()

CDEstadoCompuesto::CDEstadoCompuesto(const EstadoCompuestoPtr &e,wxWindow* parent):estado(e)
{
	//(*Initialize(CDEstadoCompuesto)
	ostringstream oss;
	oss<<"Configuration: "<<estado->getNombre();

	Create(parent, wxID_ANY, _U(oss.str().c_str()), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	//The main panel consists of a tabbed panel with two panels. One of them contains the specific data of a compound state, while the other manages the code on entry/exit.
	wxNotebook *ac=new wxNotebook(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxNB_TOP);
	/* Base tab */	{
		wxPanel *panel=new wxPanel(ac,wxID_ANY);
		wxSizer *v=new wxBoxSizer(wxVERTICAL);
		wxSizer *h=new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *label1=new wxStaticText(panel,wxID_ANY,_("Name: "),wxDefaultPosition,wxDefaultSize,wxEXPAND|wxALIGN_RIGHT);
		h->Add(label1,1,wxALIGN_CENTRE_VERTICAL);
		nombreEstado=new wxTextCtrl(panel,ID_NOMBREESTADO,_U(e->getNombre().c_str()),wxDefaultPosition,wxDefaultSize,wxEXPAND|wxTE_LEFT);
		h->Add(nombreEstado,5);
		v->Add(h);
		h=new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *label2=new wxStaticText(panel,wxID_ANY,_("Initial State: "),wxDefaultPosition,wxDefaultSize,wxEXPAND|wxALIGN_RIGHT);
		h->Add(label2,1,wxALIGN_CENTRE_VERTICAL);
		vector<EstadoPtr> subs;
		estado->getSubestados(subs);
		size_t N=subs.size();
		vector<wxString> subNames(N);
		for (size_t i=0;i<N;++i) subNames[i]=_U(subs[i]->getNombre().c_str());
		combo=new wxComboBox(panel,ID_COMBO,_(""),wxDefaultPosition,wxDefaultSize,N,(N==0)?NULL:&subNames[0],wxEXPAND|wxCB_READONLY);
		EstadoPtr ei;
		estado->getInicial(ei);
		if (ei) combo->SetValue(_U(ei->getNombre().c_str()));
		h->Add(combo,3);
		v->Add(h);
		panel->SetSizer(v);
		v->Fit(panel);
		v->SetSizeHints(panel);
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
	h->Add(bca,1);
	h->AddStretchSpacer(1);
	v->Add(h,0,wxEXPAND|wxALIGN_CENTRE_HORIZONTAL);
	SetSizer(v);
	v->Fit(this);
	v->SetSizeHints(this);
	//*)
	Connect(ID_OK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CDEstadoCompuesto::OnOK);
	Connect(ID_CANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CDEstadoCompuesto::OnCancel);
	ShowModal();
}

void CDEstadoCompuesto::OnOK(wxEvent &event)	{
	string s(nombreEstado->GetValue().mbc_str());
	if (s.length()==0)	{
		wxMessageBox(_("The state must have a name."),_("Error"),wxICON_ERROR);
		nombreEstado->SetFocus();
		return;
	}
	EstadoPtr p=estado->getPanel()->getEstadoPorNombre(s);
	if (p&&(p.pointer()!=estado.pointer()))	{
		wxMessageBox(_("There already exists a state with that name."),_("Error"),wxICON_ERROR);
		nombreEstado->SetFocus();
		return;
	}
	estado->setNombre(s);
	estado->setInicialPorNombre(string(combo->GetValue().mbc_str()));	//Every compound state MUST have a initial state. However, this is not ensured by this call.
	vector<AccionPtr> snts;
	tabCodigo->getOnentry(snts);
	estado->setOnentry(snts);
	tabCodigo->getOnexit(snts);
	estado->setOnexit(snts);
	EndModal(wxID_OK);
}

void CDEstadoCompuesto::OnCancel(wxEvent &event)	{
	EndModal(wxID_CANCEL);
}
