#include "TabSentenciasSimple.h"

//(*InternalHeaders(TabSentenciasSimple)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <algorithm>

#include "Acciones.h"
#include "PanelGraf.h"
#include "TabInstrRobot.h"
#include "TabInstrSCXML.h"

using namespace std;
//(*IdInit(TabSentenciasSimple)
const long TabSentenciasSimple::ID_BUTTON1 = wxNewId();
const long TabSentenciasSimple::ID_BUTTON2 = wxNewId();
const long TabSentenciasSimple::ID_LISTBOX1 = wxNewId();
const long TabSentenciasSimple::ID_BUTTON3 = wxNewId();
const long TabSentenciasSimple::ID_BUTTON4 = wxNewId();
const long TabSentenciasSimple::ID_BUTTON5 = wxNewId();
const long TabSentenciasSimple::ID_BUTTON6 = wxNewId();
const long TabSentenciasSimple::ID_BUTTON7 = wxNewId();
//*)

BEGIN_EVENT_TABLE(TabSentenciasSimple,wxPanel)
	//(*EventTable(TabSentenciasSimple)
	//*)
END_EVENT_TABLE()

TabSentenciasSimple::TabSentenciasSimple(const EstadoPtr &e,const wxString &labelText,wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size):indiceEnEspera(-1),estado(e),instrs()
{
	//(*Initialize(TabSentenciasSimple)
	wxGridSizer* GridSizer1;
	wxSizer* BoxSizer1;
	wxGridSizer* GridSizer2;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	SetMaxSize(wxSize(-1,-1));
	BoxSizer1=new wxStaticBoxSizer(wxVERTICAL,this,labelText);
	anadirRobot = new wxButton(this, ID_BUTTON1, _("Add Robot Action"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	BoxSizer1->Add(anadirRobot, 0, wxBOTTOM|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	anadirControl = new wxButton(this, ID_BUTTON2, _("Add Control Action"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	BoxSizer1->Add(anadirControl, 0, wxBOTTOM|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	sentencias = new wxListBox(this, ID_LISTBOX1, wxDefaultPosition, wxDefaultSize, 0, 0, wxLB_SINGLE|wxLB_NEEDED_SB|wxVSCROLL, wxDefaultValidator, _T("ID_LISTBOX1"));
	BoxSizer1->Add(sentencias, 4, wxBOTTOM|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	GridSizer1 = new wxGridSizer(1, 2, 0, 0);
	quitar = new wxButton(this, ID_BUTTON3, _("Remove"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
	quitar->Disable();
	GridSizer1->Add(quitar, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	limpiar = new wxButton(this, ID_BUTTON4, _("Clean"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
	GridSizer1->Add(limpiar, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	BoxSizer1->Add(GridSizer1, 0, wxBOTTOM|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	GridSizer2 = new wxGridSizer(1, 3, 0, 0);
	arriba = new wxButton(this, ID_BUTTON5, _("Move up"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON5"));
	arriba->Disable();
	GridSizer2->Add(arriba, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	editar = new wxButton(this, ID_BUTTON6, _("Edit"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON6"));
	editar->Disable();
	GridSizer2->Add(editar, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	abajo = new wxButton(this, ID_BUTTON7, _("Move down"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON7"));
	abajo->Disable();
	GridSizer2->Add(abajo, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer1->Add(GridSizer2, 0, wxBOTTOM|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TabSentenciasSimple::OnanadirRobotClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TabSentenciasSimple::OnanadirControlClick);
	Connect(ID_LISTBOX1,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&TabSentenciasSimple::OnsentenciasSelect);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TabSentenciasSimple::OnquitarClick);
	Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TabSentenciasSimple::OnlimpiarClick);
	Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TabSentenciasSimple::OnarribaClick);
	Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TabSentenciasSimple::OneditarClick);
	Connect(ID_BUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TabSentenciasSimple::OnabajoClick);
	//*)
}

void TabSentenciasSimple::OnanadirRobotClick(wxCommandEvent& event)	{
	CDTabInstrRobot *tir=new CDTabInstrRobot(this,AccionRobotPtr(NULL),_("New Instruction"));
	indiceEnEspera=-1;
	long res1=tir->ShowModal();
	long res2=wxID_OK;
	//if (tir->ShowModal()==wxID_OK) anadirSentencia(tir->base->getAccionRobot());
	if (res1==res2) anadirSentencia(tir->base->getAccionRobot());
	delete tir;
	updateButtons();
}

void TabSentenciasSimple::OnanadirControlClick(wxCommandEvent& event)	{
	CDTabInstrSCXML *tis=new CDTabInstrSCXML(estado->getPanel(),estado->getPanel()->getPadre(),AccionPtr(NULL),_("New instruction"));
	indiceEnEspera=-1;
	if (tis->ShowModal()==wxID_OK) anadirSentencia(tis->base->getAccion());
	delete tis;
	updateButtons();
}

void TabSentenciasSimple::OnquitarClick(wxCommandEvent& event)	{
	int sel=sentencias->GetSelection();
	if (sel==wxNOT_FOUND) return;
	instrs.erase(instrs.begin()+sel);
	setSentencias();
	updateButtons();
}

void TabSentenciasSimple::OnlimpiarClick(wxCommandEvent& event)	{
	sentencias->Clear();
	instrs.clear();
	updateButtons();
}

void TabSentenciasSimple::OnarribaClick(wxCommandEvent& event)	{
	int sel=sentencias->GetSelection();
	if (sel==wxNOT_FOUND||sel==0) return;
	swap(instrs[sel],instrs[sel-1]);
	setSentencias();
	sentencias->SetSelection(sel-1);
	updateButtons();
}

void TabSentenciasSimple::OneditarClick(wxCommandEvent& event)	{
	int sel=sentencias->GetSelection();
	if (sel==wxNOT_FOUND) return;
	indiceEnEspera=sel;
	if (IS_DERIVED(instrs[sel],AccionRobot))	{
		CDTabInstrRobot *tir=new CDTabInstrRobot(this,static_cast<AccionRobotPtr>(instrs[sel]),_("Edit Instruction"));
		if (tir->ShowModal()==wxID_OK) anadirSentencia(tir->base->getAccionRobot());
		delete tir;
	}	else	{
		CDTabInstrSCXML *tis=new CDTabInstrSCXML(this,estado->getPanel()->getPadre(),instrs[sel],_("Edit instruction"));
		if (tis->ShowModal()==wxID_OK) anadirSentencia(tis->base->getAccion());
		delete tis;
	}
	updateButtons();
}

void TabSentenciasSimple::OnabajoClick(wxCommandEvent& event)	{
	int sel=sentencias->GetSelection();
	if (sel==wxNOT_FOUND||sel>=static_cast<int>(instrs.size()-1)) return;
	swap(instrs[sel],instrs[sel+1]);
	setSentencias();
	sentencias->SetSelection(sel+1);
	updateButtons();
}

void TabSentenciasSimple::anadirSentencia(const AccionPtr &a)	{
	if (!a) return;
	if (indiceEnEspera==-1) instrs.push_back(a);
	else instrs[indiceEnEspera]=a;
	setSentencias();
	updateButtons();
}

void TabSentenciasSimple::getSentencias(vector<AccionPtr> &sen) const	{
	sen=instrs;
}

void TabSentenciasSimple::setSentencias(const vector<AccionPtr> &sen)	{
	instrs=sen;
	setSentencias();
}

void TabSentenciasSimple::setSentencias()	{
	sentencias->Clear();
	vector<wxString> visibles(instrs.size());
	string aux;
	for (size_t i=0;i<instrs.size();i++)	{
		instrs[i]->getCodigoLegible(aux);
		visibles[i]=_U(aux.c_str());
	}
	
	if (instrs.size()>0) sentencias->Set(instrs.size(),&visibles[0],(void**)NULL);	//Se accede por índice, no por referencia tipo wxClient.
	else sentencias->Set(0,NULL,(void**)NULL);
}

void TabSentenciasSimple::OnsentenciasSelect(wxCommandEvent& event) {
	updateButtons();
}

void TabSentenciasSimple::updateButtons()	{
	int sel=sentencias->GetSelection();
	bool enable=(sel!=wxNOT_FOUND);
	arriba->Enable(enable&&sel!=0);
	quitar->Enable(enable);
	editar->Enable(enable);
	abajo->Enable(enable&&sel<static_cast<int>(instrs.size()-1));
}
