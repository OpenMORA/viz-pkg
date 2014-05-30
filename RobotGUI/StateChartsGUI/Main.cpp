#include "Main.h"

//(*InternalHeaders(Main)
#include <wx/intl.h>
#include <wx/sizer.h>
#include <wx/msgdlg.h>
#include <wx/string.h>
#include <wx/bmpbuttn.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/stattext.h>
#include <wx/scrolwin.h>
#include <wx/textctrl.h>

#include <fstream>
#include <sstream>
#include <stdexcept>

#include "Accion.h"
#include "PanelGraf.h"
#include "TabInstrRobot.h"
#include "CDNuevoEvento.h"
#include "CDNuevaVariable.h"
#include "CDPedirEstadoDestino.h"
//*)

using namespace std;
using namespace InterfazSancho;

inline static std::vector<long> createIDsVector(size_t N)	{
	std::vector<long> res(N);
	for (std::vector<long>::iterator it=res.begin();it!=res.end();++it) *it=wxNewId();
	return res;
}

//(*IdInit(Main)
const vector<long> Main::IDS_S=createIDsVector(6);
const vector<long> Main::IDS_E=createIDsVector(4);
const long Main::ID_EVENTOS=wxNewId();
const long Main::ID_VARS=wxNewId();
const long Main::ID_STOP=wxNewId();
const long Main::ID_RESUME=wxNewId();
const long Main::ID_SCXML=wxNewId();
const long Main::ID_PANELGRAF=wxNewId();
const long SCXMLCodeDialog::ID_OK=wxNewId();
const long SCXMLCodeDialog::ID_CANCEL=wxNewId();
const long SCXMLCodeDialog::ID_WRITE=wxNewId();
//*)

BEGIN_EVENT_TABLE(Main,wxPanel)
	//(*EventTable(Main)
	//*)
END_EVENT_TABLE()

Main::Main(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(Main)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	//*)
	wxFlexGridSizer *hb=new wxFlexGridSizer(0,0,0,0);
	hb->AddGrowableCol(0);
	//hb->AddGrowableRow(0);

	cp=new wxNotebook(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxNB_LEFT);
	cp->AddPage(tabEstados(cp),_("State machine"));
	cp->AddPage(tabEventos(cp),_("Variable and event definition"));
	hb->Add(cp);


	wxFlexGridSizer *vb=new wxFlexGridSizer(0,0,0,0);
	hb->AddGrowableCol(0);
	//hb->AddGrowableRow(0);

	wxBitmap bitmapStop = wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("ICON_STOP")));
	wxBitmapButton *b1=new wxBitmapButton(this,ID_STOP,bitmapStop);
	vb->Add(b1);
	vb->AddSpacer(25);
	wxBitmap bitmapResume = wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("ICON_RESUME")));
	wxBitmapButton *b2=new wxBitmapButton(this,ID_RESUME,bitmapResume);
	vb->Add(b2);
	Connect(ID_STOP,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Main::OnStop);
	Connect(ID_STOP,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Main::OnResume);
	hb->Add(vb);
	SetSizer(hb);
	hb->Fit(this);
	hb->SetSizeHints(this);

	//TODO: start a timer (may not be necessary) and communicate (sockets/MORA) to get the actions for the robot.
	//TabInstrRobot::parse(str,instrList,paramList);
	//AccionRobot::setAccionesDisponibles(instrList);
	//AccionRobot::setParametrosDisponibles(paramList);
	cambiarEstado(containerEstados->estado);
	//This is a good place to register every "smartpointer-able" class, since this should be called exactly once.
	mrpt::utils::registerClass(CLASS_ID(Accion));
	mrpt::utils::registerClass(CLASS_ID(AccionAssign));
	mrpt::utils::registerClass(CLASS_ID(AccionElse));
	mrpt::utils::registerClass(CLASS_ID(AccionElseif));
	mrpt::utils::registerClass(CLASS_ID(AccionEndif));
	mrpt::utils::registerClass(CLASS_ID(AccionEvent));
	mrpt::utils::registerClass(CLASS_ID(AccionSend));
	mrpt::utils::registerClass(CLASS_ID(AccionRobot));
	mrpt::utils::registerClass(CLASS_ID(Instruccion));
	mrpt::utils::registerClass(CLASS_ID(Dibujable));
	mrpt::utils::registerClass(CLASS_ID(Transicion));
	mrpt::utils::registerClass(CLASS_ID(Estado));
	mrpt::utils::registerClass(CLASS_ID(EstadoSimple));
	mrpt::utils::registerClass(CLASS_ID(EstadoCompuesto));
	mrpt::utils::registerClass(CLASS_ID(EstadoParalelo));
}

void Main::cambiarEstado(int ne)	{
	for (size_t i=0;i<6;++i) botonesS[i]->SetValue((ne+1)==static_cast<int>(i));
	containerEstados->estado=ne;
}

void Main::getVariables(vector<wxString> &out) const	{
	out.clear();
	size_t N=lVars->GetCount();
	for (size_t i=0;i<N;++i)	{
		wxString tmp=lVars->GetString(i);
		tmp=tmp.substr(0,tmp.find('='));
		out.push_back(tmp);
	}
}

void Main::getEventos(vector<wxString> &out) const	{
	out.clear();
	size_t N=lEventos->GetCount();
	for (size_t i=0;i<N;++i) out.push_back(lEventos->GetString(i));
}

void Main::nuevaVariable(const string &n,const string &v)	{
	size_t N=lVars->GetCount();
	ostringstream oss;
	oss<<"dat."<<n;
	for (size_t i=0;i<N;++i)	{
		wxString tmp=lVars->GetString(i);
		if (tmp.substr(0,tmp.find('='))==_U(oss.str().c_str()))	{
			oss<<'='<<v;
			lVars->SetString(i,_U(oss.str().c_str()));
			return;
		}
	}
	oss<<'='<<v;
	lVars->Insert(_U(oss.str().c_str()),N);
}

void Main::nuevoEvento(const string &n)	{
	size_t N=lEventos->GetCount();
	for (size_t i=0;i<N;++i) if (lEventos->GetString(i)==_U(n.c_str())) return;
	lEventos->Insert(_U(n.c_str()),N);
}

wxPanel *Main::tabEstados(wxWindow *parent)	{
	wxPanel *res=new wxPanel(parent,wxID_ANY);
	wxFlexGridSizer *h=new wxFlexGridSizer(0,0,0,0);

	h->AddGrowableCol(0);
	//h->AddGrowableRow(0);


	wxFlexGridSizer *panelBotones=new wxFlexGridSizer(0,1,0,0);

	panelBotones->AddGrowableCol(0);
	//panelBotones->AddGrowableRow(0);


	panelBotones->AddStretchSpacer(1);
	botonesS.resize(6);
	wxString captions[6]={_("Options"),_("Simple state"),_("Compound state"),_("Parallel state"),_("Delete"),_("Transition")};
	wxString tooltips[6]={_("Moves or customizes the selected element."),_("Creates a simple state."),_("Creates a compound state."),_("Creates a parallel state."),_("Deletes the selected element."),_("Creates a transition between two states.")};
	//wxString filenames[6]={_("Opts.bmp"),_("Sim.bmp"),_("Com.bmp"),_("Par.bmp"),_("Del.bmp"),_("Tra.bmp")};
	wxObjectEventFunction funs[6]={(wxObjectEventFunction)&Main::OnOptions,(wxObjectEventFunction)&Main::OnSimple,(wxObjectEventFunction)&Main::OnCompound,(wxObjectEventFunction)&Main::OnParallel,(wxObjectEventFunction)&Main::OnDelete,(wxObjectEventFunction)&Main::OnTransition};
	for (size_t i=0;i<6;++i)	{
		//TODO: "botonesS[i]" should be a wxCustomButton
		botonesS[i]=new wxToggleButton(res,IDS_S[i],captions[i]);
		botonesS[i]->SetToolTip(tooltips[i]);
		panelBotones->Add(botonesS[i],1,wxEXPAND);
		panelBotones->AddSpacer(5);
		Connect(IDS_S[i],wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,funs[i]);
	}
	panelBotones->AddStretchSpacer(1);
	wxButton *bSCXML=new wxButton(res,ID_SCXML,_("SCXML"));
	panelBotones->Add(bSCXML,1,wxEXPAND);
	panelBotones->AddStretchSpacer(1);
	Connect(ID_SCXML,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Main::OnSCXML);

	h->Add(panelBotones,1,wxEXPAND);
	containerEstados=panelEstados(res);
	containerEstados->estado=0;
	h->AddSpacer(15);
	h->Add(containerEstados,1,wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL);
	res->SetSizer(h);
	h->Fit(res);
	h->SetSizeHints(res);
	return res;
}

wxPanel *Main::tabEventos(wxWindow *parent)	{
	wxPanel *res=new wxPanel(parent,wxID_ANY);
	wxFlexGridSizer *h=new wxFlexGridSizer(1,1,0,0);

	h->AddGrowableCol(0);
	h->AddGrowableRow(0);

	wxFlexGridSizer *panelBotones=new wxFlexGridSizer(1,1,0,0);

	panelBotones->AddGrowableCol(0);
	panelBotones->AddGrowableRow(0);


	panelBotones->AddStretchSpacer(1);
	botonesE.resize(4);
	wxString captions[4]={_("Add variable"),_("Remove variable(s)"),_("Add event"),_("Remove event(s)")};
	wxString tooltips[4]={_("Adds a new variable."),_("Removes a variable or set of variables."),_("Adds a new event."),_("Removes an event or set of events-")};
	wxObjectEventFunction funs[4]={(wxObjectEventFunction)&Main::OnAddVariable,(wxObjectEventFunction)&Main::OnRemoveVariable,(wxObjectEventFunction)&Main::OnAddEvent,(wxObjectEventFunction)&Main::OnRemoveEvent};
	for (size_t i=0;i<4;++i)	{
		botonesE[i]=new wxButton(res,IDS_E[i],captions[i]);
		botonesE[i]->SetToolTip(tooltips[i]);
		panelBotones->Add(botonesE[i],1,wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL);
		panelBotones->AddStretchSpacer(1);
		Connect(IDS_E[i],wxEVT_COMMAND_BUTTON_CLICKED,funs[i]);
	}
	h->Add(panelBotones,1,wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL);
	h->AddSpacer(10);
	wxFlexGridSizer *v=new wxFlexGridSizer(1,0);
	v->AddGrowableRow(1);
	wxStaticText *label1=new wxStaticText(res,wxID_ANY,_("Variables:"));
	v->Add(label1,1,wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTRE_VERTICAL);
	lVars=new wxListBox(res,ID_VARS,wxDefaultPosition,wxDefaultSize,0,NULL,wxLB_EXTENDED|wxLB_NEEDED_SB);
	v->Add(lVars,1,wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL);
	h->Add(v,1,wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL);
	v=new wxFlexGridSizer(1,0);
	v->AddGrowableRow(1);
	wxStaticText *label2=new wxStaticText(res,wxID_ANY,_("Events:"));
	v->Add(label2,1,wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTRE_VERTICAL);
	lEventos=new wxListBox(res,ID_EVENTOS,wxDefaultPosition,wxDefaultSize,0,NULL,wxLB_EXTENDED|wxLB_NEEDED_SB);
	v->Add(lEventos,1,wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL);
	h->Add(v,1,wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL);
	res->SetSizer(h);
	h->Fit(res);
	h->SetSizeHints(res);
	return res;
}

PanelGraf *Main::panelEstados(wxWindow *parent)	{
	return new PanelGraf(parent,this,ID_PANELGRAF);
}

void Main::OnStop(wxEvent &)	{
	//TODO: communication. Send a "stop" event
	;
}

void Main::OnResume(wxEvent &)	{
	//TODO: communication. Send a "resume" event
	;
}

void Main::OnAddVariable(wxEvent &)	{
	CDNuevaVariable(this);
}

size_t findMax(const wxArrayInt &a,size_t from)	{
	size_t res=from;
	int mx=a[res];
	for (size_t i=from+1;i<a.size();++i) if (a[i]>mx)	{
		mx=a[i];
		res=i;
	}
	return res;
}

void sortDescending(wxArrayInt &a)	{
	for (size_t i=0;i<a.size()-1;++i)	{
		size_t idx=findMax(a,i);
		if (idx!=i) swap(a[i],a[idx]);
	}
}

void Main::OnRemoveVariable(wxEvent &)	{
	wxArrayInt a; // JL:??  (NULL,NULL);
	lVars->GetSelections(a);
	sortDescending(a);
	for (size_t i=0;i<a.size();++i) lVars->Delete(a[i]);
}

void Main::OnAddEvent(wxEvent &)	{
	CDNuevoEvento(this);
}

void Main::OnRemoveEvent(wxEvent &)	{
	wxArrayInt a; // JL:??  (NULL,NULL);
	lEventos->GetSelections(a);
	sortDescending(a);
	for (size_t i=0;i<a.size();++i) lEventos->Delete(a[i]);
}

void Main::OnOptions(wxEvent &)	{
	cambiarEstado(-1);
}

void Main::OnSimple(wxEvent &)	{
	cambiarEstado(0);
}

void Main::OnCompound(wxEvent &)	{
	cambiarEstado(1);
}

void Main::OnParallel(wxEvent &)	{
	cambiarEstado(2);
}

void Main::OnDelete(wxEvent &)	{
	cambiarEstado(3);
}

void Main::OnTransition(wxEvent &)	{
	cambiarEstado(4);
}

void Main::OnSCXML(wxEvent &)	{
	CDPedirEstadoDestino dlg(containerEstados,_("Select the initial state"),true);
	string s=dlg.getSelection();
	if (s.size()<=0) return;
	string str;
	try	{
		str=containerEstados->generarSCXML(s);
	}	catch (const logic_error &l)	{
		wxMessageBox(_U(l.what()),_("Error"),wxICON_ERROR);
		return;
	}
	SCXMLCodeDialog *dSCXML=new SCXMLCodeDialog(this,str);
	int retCode=dSCXML->ShowModal();
	delete dSCXML;
	if (retCode==0)	{
		//TODO: communications. The scxml code, which is in "str", should be sent to the external application using MORA.
	}	else if (retCode==2)	{
		wxFileDialog *fd=new wxFileDialog(this,_("Select a path for the SCXML code to be written"),_(""),_(""),_("SCXML files (*.scxml)|*.scxml|All files|*.*"),wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
		if (fd->ShowModal()==wxID_OK)	{
			ofstream file(fd->GetPath().mbc_str());
			if (file.bad()||!file.is_open())	{
				wxMessageBox(_("Can't open the desired file for writing."),_("Error"),wxICON_ERROR);
				file.close();
			}	else	{
				file<<str;
				file.close();
			}
		}
		delete fd;
	}
}

SCXMLCodeDialog::SCXMLCodeDialog(wxWindow *parent,const string &str)	{
	Create(parent,wxID_ANY,_("SCXML code"));
	wxSizer *v=new wxBoxSizer(wxVERTICAL);
	wxTextCtrl *ta=new wxTextCtrl(this,wxID_ANY,_U(str.c_str()),wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|wxTE_READONLY);
	v->Add(ta,0,wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL);
	wxSizer *h=new wxBoxSizer(wxHORIZONTAL);
	h->AddStretchSpacer(1);
	wxButton *ac=new wxButton(this,ID_OK,_("OK"));
	h->Add(ac,1);
	h->AddStretchSpacer(1);
	wxButton *ca=new wxButton(this,ID_CANCEL,_("Cancel"));
	h->Add(ca,1);
	h->AddStretchSpacer(1);
	wxButton *wr=new wxButton(this,ID_WRITE,_("Save..."));
	h->Add(wr);
	h->AddStretchSpacer(1);
	v->Add(h);
	SetSizer(v);
	v->Fit(this);
	v->SetSizeHints(this);
	Connect(ID_OK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SCXMLCodeDialog::OnOK);
	Connect(ID_CANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SCXMLCodeDialog::OnCancel);
	Connect(ID_WRITE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SCXMLCodeDialog::OnWrite);
}

void SCXMLCodeDialog::OnOK(wxEvent &)	{
	EndModal(0);	//Not standard, but useful (see the use of this return value, retrieved as "dSCXML->ShowModal()" in OnSCXML.
}

void SCXMLCodeDialog::OnCancel(wxEvent &)	{
	EndModal(1);
}

void SCXMLCodeDialog::OnWrite(wxEvent &)	{
	EndModal(2);
}
