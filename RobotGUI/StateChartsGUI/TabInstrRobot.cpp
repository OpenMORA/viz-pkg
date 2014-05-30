#include "TabInstrRobot.h"

//(*InternalHeaders(TabInstrRobot)
#include <wx/intl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/string.h>
#include <wx/stattext.h>
//*)

#include <algorithm>

using namespace std;
using namespace InterfazSancho;

//(*IdInit(TabInstrRobot)
const long TabInstrRobot::ID_TID=wxNewId();
const long TabInstrRobot::ID_COMBO=wxNewId();
const long CDTabInstrRobot::ID_OK=wxNewId();
const long CDTabInstrRobot::ID_CANCEL=wxNewId();
//*)


BEGIN_EVENT_TABLE(TabInstrRobot,wxPanel)
	//(*EventTable(TabInstrRobot)
	//*)
END_EVENT_TABLE()

/**
  * Searchs for an instruction inside a vector of smart pointers.
  * Note that this is not exactly the same as Dibujable::findSP, as it will just compare pointers, while this functions compares the content.
  */
inline vector<InstruccionPtr>::iterator searchFor(vector<InstruccionPtr> &vIn,const InstruccionPtr &p)	{
	for (vector<InstruccionPtr>::iterator it=vIn.begin();it!=vIn.end();++it) if (*(*it)==*p) return it;
	return vIn.end();
}

TabInstrRobot::TabInstrRobot(const AccionRobotPtr &instr,wxWindow* parent)
{
	//(*Initialize(TabInstrRobot)
	Create(parent,wxID_ANY,wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxSizer *vb=new wxBoxSizer(wxVERTICAL);
	wxSizer *hb=new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *label=new wxStaticText(this,wxID_ANY,_("Action identifier: "),wxDefaultPosition,wxDefaultSize,wxALIGN_RIGHT);
	hb->Add(label,1,wxALIGN_CENTRE_VERTICAL);
	tID=new wxTextCtrl(this,ID_TID,_(""),wxDefaultPosition,wxDefaultSize,wxTE_LEFT|wxEXPAND);
	hb->Add(tID,3);
	vb->Add(hb);
	wxStaticText *label2=new wxStaticText(this,wxID_ANY,_("Action: "),wxDefaultPosition,wxDefaultSize,wxALIGN_LEFT);
	vb->Add(label2,0,wxALIGN_CENTRE_VERTICAL);
	vector<InstruccionPtr> accs;
	AccionRobot::getAccionesDisponibles(accs);
	cbAcc=new wxChoicebook(this,ID_COMBO,wxDefaultPosition,wxDefaultSize,wxCHB_TOP|wxEXPAND);
	for (vector<InstruccionPtr>::const_iterator it=accs.begin();it!=accs.end();++it)	{
		wxPanel *panel=new wxPanel(cbAcc);
		wxSizer *b=new wxBoxSizer(wxVERTICAL);
		ContainerMultiple *cm=new ContainerMultiple(*it,panel);
		containers.push_back(cm);
		b->Add(cm);
		panel->SetSizer(b);
		b->Fit(panel);
		b->SetSizeHints(panel);
		cbAcc->AddPage(panel,_U((*it)->nombreVisible.c_str()));
	}
	vb->Add(cbAcc);
	SetSizer(vb);
	vb->Fit(this);
	vb->SetSizeHints(this);
	//After the GUI is created, it's refilled with the given instruction (if any).
	if (instr)	{
		InstruccionPtr i;
		instr->getInstruccion(i);
		vector<InstruccionPtr>::iterator pos=searchFor(accs,i);
		if (pos!=accs.end())	{
			string str;
			instr->getParametro2(str);
			tID->SetValue(_U(str.c_str()));
			int si=distance(accs.begin(),pos);
			cbAcc->SetSelection(si);
			vector<ParametroInstruccion> ps;
			instr->getParametros(ps);
			containers[si]->setValores(ps);
		}	else throw logic_error("Error parsing instrucion.");
	}	else if (accs.size()>0) cbAcc->SetSelection(0);
	//*)
	Connect(ID_TID,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&TabInstrRobot::OnTID);
	Connect(ID_COMBO,wxEVT_COMMAND_COMBOBOX_SELECTED,(wxObjectEventFunction)&TabInstrRobot::OnAct);
}

bool TabInstrRobot::parse(const string &in,pair<vector<InstruccionPtr>,vector<ParametroInstruccion> > &out)	{
	size_t i=in.find('.');
	if (i==string::npos) return false;
	try	{
		Instruccion::parseMultiple(in.substr(0,i),out.first);
		ParametroInstruccion::parseMultiple(in.substr(i+1),out.second);
		return true;
	}	catch (const logic_error &)	{
		return false;
	}
}

//Small function to generate an instruction from a string. Instructions generated this way have the same readable and internal name, and have no parameters.
inline InstruccionPtr instrFromString(const string &in)	{
	static Instruccion i;
	i.nombreReal=i.nombreVisible=in;
	i.listaTipos=vector<string>(0);
	return InstruccionPtr(new Instruccion(i));
}

bool TabInstrRobot::setAcciones()	{
	//This function generates a very basic set of instructions. Although it seems a good idea to make these functions need a "free string" (or any other type) as a
	//parameter, that shouldn't really matter, since the set of instructions should be replaced immediately with external data.
	vector<InstruccionPtr> acciones;
	acciones.push_back(instrFromString("MOVE"));
	acciones.push_back(instrFromString("SAY"));
	acciones.push_back(instrFromString("SAY_TIME"));
	acciones.push_back(instrFromString("SAY_DATE"));
	acciones.push_back(instrFromString("STOP_NAV"));
	AccionRobot::setAccionesDisponibles(acciones);
	AccionRobot::setArgumentosDisponibles(vector<ParametroInstruccion>());
	return false;
	/*	//Uncomment this code, changing the "getInstructionsFromNet" call, when complete.
	string cadena="";
	if (!getInstructionsFromNet(cadena)) return false;
	pair<vector<InstruccionPtr>,vector<ParametroInstruccion> > data;
	if (!parse(cadena,data)) return false;
	AccionRobot::setAccionesDisponibles(data.first);
	AccionRobot::setArgumentosDisponibles(data.second);
	return true;
	*/
}

InstruccionPtr TabInstrRobot::getAccion() const	{
	vector<InstruccionPtr> accs;
	AccionRobot::getAccionesDisponibles(accs);
	wxString s=cbAcc->GetPageText(cbAcc->GetSelection());
	for (vector<InstruccionPtr>::const_iterator it=accs.begin();it!=accs.end();++it) if (_U(((*it)->nombreVisible).c_str())==s) return *it;
	return instrFromString(string(s.mbc_str()));
}

void TabInstrRobot::setAccion(const InstruccionPtr &i)	{
	vector<InstruccionPtr> accs;
	AccionRobot::getAccionesDisponibles(accs);
	string str=i->nombreVisible;
	for (size_t i=0;i<cbAcc->GetPageCount();++i) if (string((cbAcc->GetPageText(i)).mbc_str())==str)	{
		cbAcc->ChangeSelection(i);
		return;
	}
}

void TabInstrRobot::setParametros(const vector<ParametroInstruccion> &ps)	{
	int i=cbAcc->GetSelection();
	if (i>0) containers[i]->setValores(ps);
}

AccionRobotPtr TabInstrRobot::getAccionRobot() const	{
	int i=cbAcc->GetSelection();
	if (i==-1) return AccionRobotPtr(NULL);
	AccionRobotPtr res=containers[i]->getAccionRobot();
	res->setEtiqueta(string(tID->GetValue().mbc_str()));
	return res;
}

void TabInstrRobot::OnTID(wxEvent &event)	{
	if (idFun.fun) idFun.fun(idFun.data);
}

void TabInstrRobot::OnAct(wxEvent &event)	{
	if (actFun.fun) actFun.fun(actFun.data);
}

BEGIN_EVENT_TABLE(CDTabInstrRobot,wxDialog)

END_EVENT_TABLE()

CDTabInstrRobot::CDTabInstrRobot(wxWindow *parent,const AccionRobotPtr &instr,const wxString &tit)	{
	Create(parent,wxID_ANY,tit);
	wxSizer *sz=new wxBoxSizer(wxVERTICAL);
	base=new TabInstrRobot(instr,this);
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
	Connect(ID_OK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CDTabInstrRobot::OnOK);
	Connect(ID_CANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CDTabInstrRobot::OnCancel);
}

void CDTabInstrRobot::OnOK(wxCommandEvent &)	{
	EndModal(wxID_OK);
}

void CDTabInstrRobot::OnCancel(wxCommandEvent &)	{
	EndModal(wxID_CANCEL);
}
