#include "CDEstadoSimple.h"

//(*InternalHeaders(CDEstadoSimple)
#include <wx/intl.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/msgdlg.h>
#include <wx/string.h>
#include <wx/valtext.h>
#include <wx/stattext.h>
//*)

#include <cstdio>
#include <vector>
#include <sstream>

#include "Acciones.h"
#include "PanelGraf.h"
#include "Instruccion.h"
#include "CDPedirEstadoDestino.h"

using namespace std;
using namespace InterfazSancho;

//(*IdInit(CDEstadoSimple)
const long CDEstadoSimple::ID_NOMBREESTADO=wxNewId();
const long CDEstadoSimple::ID_T2_VECES=wxNewId();
const long CDEstadoSimple::ID_T3_NOMBREEVENTO=wxNewId();
const long CDEstadoSimple::ID_T3_CONDICION=wxNewId();
const long CDEstadoSimple::ID_FINAL=wxNewId();
const long CDEstadoSimple::ID_OK=wxNewId();
const long CDEstadoSimple::ID_CANCEL=wxNewId();
//*)

BEGIN_EVENT_TABLE(CDEstadoSimple,wxDialog)
	//(*EventTable(CDEstadoSimple)
	//*)
END_EVENT_TABLE()

inline bool nonEqual(const vector<ParametroInstruccion> &p1,const vector<ParametroInstruccion> &p2)	{
	if (p1.size()!=p2.size()) return false;
	for (size_t i=0;i<p1.size();++i) if (!(p1[i]==p2[i])) return false;
	return true;
}

CDEstadoSimple::CDEstadoSimple(const EstadoSimplePtr &es,wxWindow* parent):estado(es)	{
	pos=0;
	ostringstream oss;
	oss<<"Configuration: "<<es->getNombre();
	//(*Initialize(CDEstadoSimple)
	Create(parent,wxID_ANY,_U(oss.str().c_str()), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	//*)
	wxSizer *container=new wxBoxSizer(wxVERTICAL);
	/*TODO:
		The button streak is curently placed at the top, instead of the bottom, because of problems with hiding the tabs panel.
		This may be fixed (i.e. buttons placed at the bottom, where they belong) if the tab panel were substituted by another tab-like panel with two tabs,
		one of which would be the former tab panel, the another being an empty panel. The panel selection would be toggled by the "final" checkbox.
		The problem is, I still don't know how to hide the tabs or how to securely switch between them. Currently, if the tab panel starts as hidden and then is showed
		because of the checkboc being unchecked, it hides the button streak. So, the "hideable" panel must be at the very bottom of the dialog to prevent this.
	*/
	wxSizer *hb=new wxBoxSizer(wxHORIZONTAL);
	hb->AddStretchSpacer(1);
	wxButton *ac=new wxButton(this,ID_OK,_("OK"),wxDefaultPosition,wxDefaultSize);
	hb->Add(ac,1);
	hb->AddStretchSpacer(1);
	wxButton *ca=new wxButton(this,ID_CANCEL,_("Cancel"),wxDefaultPosition,wxDefaultSize);
	hb->Add(ca,1);
	hb->AddStretchSpacer(1);
	container->Add(hb,0,wxALIGN_CENTRE_HORIZONTAL|wxEXPAND);
	hb=new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *label1=new wxStaticText(this,wxID_ANY,_("Name: "),wxDefaultPosition,wxDefaultSize,wxALIGN_RIGHT);
	hb->Add(label1,1,wxALIGN_CENTRE_VERTICAL);
	nombreEstado=new wxTextCtrl(this,ID_NOMBREESTADO,_U(estado->getNombre().c_str()),wxDefaultPosition,wxDefaultSize,wxTE_LEFT);
	hb->Add(nombreEstado,5);
	hb->AddStretchSpacer(1);
	checkFinal=new wxCheckBox(this,ID_FINAL,_("Final"));
	checkFinal->SetValue(estado->esFinal);
	hb->Add(checkFinal);
	container->Add(hb);
	ostringstream n;
	n<<estado->getNombre()<<"_id";
	tabs=new wxNotebook(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxNB_TOP);
	/* First tab */	{
		t1r=new TabInstrRobot(AccionRobotPtr(),tabs);
		t1r->setID(n.str());
		tabs->AddPage(t1r,_("Simple action"));
	}
	/* Second tab */	{
		wxPanel *panel=new wxPanel(tabs,wxID_ANY);
		wxSizer *v=new wxBoxSizer(wxVERTICAL);
		t2r=new TabInstrRobot(AccionRobotPtr(),panel);
		t2r->setID(n.str());
		p1to2=make_pair(t1r,t2r);
		p2to1=make_pair(t2r,t1r);
		t1r->addEventListenerAccion(TabInstrRobot::GenericFunctorWithArgument(&CDEstadoSimple::changeAction,static_cast<void *>(&p1to2)));
		t1r->addEventListenerID(TabInstrRobot::GenericFunctorWithArgument(&CDEstadoSimple::changeID,static_cast<void *>(&p1to2)));
		t2r->addEventListenerAccion(TabInstrRobot::GenericFunctorWithArgument(&CDEstadoSimple::changeAction,static_cast<void *>(&p2to1)));
		t2r->addEventListenerID(TabInstrRobot::GenericFunctorWithArgument(&CDEstadoSimple::changeID,static_cast<void *>(&p2to1)));
		t2r->setID(n.str());
		v->Add(t2r);
		wxSizer *h=new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *label=new wxStaticText(panel,wxID_ANY,_("Times: "),wxDefaultPosition,wxDefaultSize,wxALIGN_RIGHT);
		h->Add(label,0,wxALIGN_CENTRE_VERTICAL);
		t2veces=new wxTextCtrl(panel,ID_T2_VECES,_("1"),wxDefaultPosition,wxDefaultSize,wxTE_LEFT,wxTextValidator(wxFILTER_NUMERIC));
		h->Add(t2veces);
		v->Add(h);
		panel->SetSizer(v);
		v->Fit(panel);
		v->SetSizeHints(panel);
		tabs->AddPage(panel,_("Loop"));
	}
	/* Third tab */	{
		wxPanel *panel=new wxPanel(tabs,wxID_ANY);
		wxSizer *v=new wxBoxSizer(wxVERTICAL);
		wxSizer *h=new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *label=new wxStaticText(panel,wxID_ANY,_("Event to wait for: "),wxDefaultPosition,wxDefaultSize,wxALIGN_RIGHT);
		h->Add(label,1,wxALIGN_CENTRE_VERTICAL);
		vector<wxString> evts;
		estado->getPanel()->getTodosLosEventos(evts);
		size_t N=evts.size();
		t3NombreEvento=new wxComboBox(panel,ID_T3_NOMBREEVENTO,_(""),wxDefaultPosition,wxDefaultSize,N,(N==0)?NULL:(&evts[0]),wxCB_READONLY);
		h->Add(t3NombreEvento,2);
		v->Add(h);
		h=new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *label2=new wxStaticText(panel,wxID_ANY,_("Condition: "),wxDefaultPosition,wxDefaultSize,wxALIGN_RIGHT);
		h->Add(label2,0,wxALIGN_CENTRE_VERTICAL);
		t3Condicion=new wxTextCtrl(panel,ID_T3_CONDICION,_(""),wxDefaultPosition,wxDefaultSize,wxTE_LEFT);
		h->Add(t3Condicion);
		v->Add(h);
		panel->SetSizer(v);
		v->Fit(panel);
		v->SetSizeHints(panel);
		tabs->AddPage(panel,_("Wait for event"));
	}
	/* Fourth tab */	{
		t4=new TabSentencias(estado,tabs);
		tabs->AddPage(t4,_("Free definition"));
	}
	container->Add(tabs);
	SetSizer(container);
	container->Fit(this);
	container->SetSizeHints(this);
	Connect(ID_OK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CDEstadoSimple::OnOK);
	Connect(ID_CANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CDEstadoSimple::OnCancel);
	Connect(ID_FINAL,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&CDEstadoSimple::OnFinalCheckBox);
	if (estado->esFinal) tabs->Hide();
	else	{
		//The following chunk of code determines the default value of the controls, based on the state information
		vector<AccionPtr> onentry,onexit;
		vector<TransicionPtr> trans;
		estado->getOnentry(onentry);
		estado->getOnexit(onexit);
		estado->getListaTransiciones(trans);
		if (onexit.size()>0) tabs->SetSelection(3);
		else switch (onentry.size())	{
			case 0:	//No actions on state entry, neither on state exit. May be a case of event and/or condition wait.
				//Checks to perform: there is exactly a transition, which has either an event, a condition, or both.
				if (trans.size()==1)	{
					string ne,co;
					trans[0]->getEvento(ne);
					trans[0]->getCondicion(co);
					if (ne.length()>0||co.length()>0)	{
						tabs->SetSelection(2);
						t3NombreEvento->SetValue(_U(ne.c_str()));
						t3Condicion->SetValue(_U(co.c_str()));
					}	else tabs->SetSelection(3);
				}	else tabs->SetSelection(3);
				break;
			case 1:	//No actions on state exit, one on state entry. May be a case of simple state.
				//Checks: the "on entry" action is a robot action, and there is a transition which will be triggered only when the action finishes.
				if (onentry[0]&&IS_DERIVED(onentry[0],AccionRobot)&&trans.size()==1)	{
					AccionRobotPtr ar=static_cast<AccionRobotPtr>(onentry[0]);
					string ev,ev2;
					ar->getParametro2(ev);
					trans[0]->getEvento(ev2);
					string tmp;
					trans[0]->getCondicion(tmp);
					ostringstream oss("");
					oss<<ev<<".done";
					if ((tmp.length()==0)&&(oss.str()==ev2))	{
						tabs->SetSelection(0);
						t1r->setID(ev);
						InstruccionPtr instr;
						ar->getInstruccion(instr);
						t1r->setAccion(instr);
						vector<ParametroInstruccion> pars;
						ar->getParametros(pars);
						t1r->setParametros(pars);
						t2r->setID(ev);
						t2r->setAccion(instr);
						t2r->setParametros(pars);
					}	else tabs->SetSelection(3);
				}	else tabs->SetSelection(3);
				break;
			case 3:
				//Three actions on state entry, none on state exit: may be a loop.
				/* This huge chunk of code checks that the sentences and transitions correspond exactly to those of a loop. There are a lot of checks:
					- There are exactly two transitions.
					- The first two actions on state entry are of type "AccionAssign", while the third is a robot action.
					- The first sentence is a "Count=0" assignment.
					- The second sentence is a "Amount=X" assignment.
					- The first transition is triggered on robot action finish and the condition "Count<Amount", doesn't have a destination state (which means that it
					won't call onentry/onexit actions) and its code consists of a "Count=Count+1" assignment.
					- The second transition is triggered on robot action finish and the condition "Count==Amount".
				*/
				if (trans.size()!=2) tabs->SetSelection(3);
				else	{
					varcuenta="";
					vartotal="";
					int vecesBucle=-1;
					string idAcc="";
					InstruccionPtr instr(NULL);
					vector<ParametroInstruccion> param;
					if (!(IS_DERIVED(onentry[0],AccionAssign)&&IS_DERIVED(onentry[1],AccionAssign)&&IS_DERIVED(onentry[2],AccionRobot)))	{
						tabs->SetSelection(3);
						break;
					}
					AccionAssignPtr aa1=static_cast<AccionAssignPtr>(onentry[0]),aa2=static_cast<AccionAssignPtr>(onentry[1]);
					AccionRobotPtr ar=static_cast<AccionRobotPtr>(onentry[2]);
					ar->getParametro2(idAcc);
					string tmp,tmp2;
					aa2->getParametro2(tmp);
					if (sscanf(tmp.c_str(),"%d",&vecesBucle)!=1)	{
						tabs->SetSelection(3);
						break;
					}
					aa1->getParametro2(tmp);
					if (tmp!="0")	{
						tabs->SetSelection(3);
						break;
					}
					aa1->getParametro(varcuenta);
					aa2->getParametro(vartotal);
					ar->getInstruccion(instr);
					ar->getParametros(param);
					TransicionPtr tra=trans[0];
					string ne,co;
					tra->getEvento(ne);
					tra->getCondicion(co);
					ostringstream oss(idAcc);
					oss<<".done";
					if (oss.str()!=ne)	{
						tabs->SetSelection(3);
						break;
					}
					oss.str(varcuenta);
					oss<<'<'<<vartotal;
					if (oss.str()!=co)	{
						tabs->SetSelection(3);
						break;
					}
					vector<AccionPtr> acTrans;
					tra->getListaSentencias(acTrans);
					if ((acTrans.size()!=2)||!IS_DERIVED(acTrans[0],AccionRobot)||!IS_DERIVED(acTrans[1],AccionAssign))	{
						tabs->SetSelection(3);
						break;
					}
					AccionRobotPtr trac=static_cast<AccionRobotPtr>(acTrans[0]);
					vector<ParametroInstruccion> ps;
					trac->getNombreVisible(tmp);
					trac->getParametros(ps);
					trac->getParametro2(tmp2);
					if ((tmp2!=idAcc)||(tmp!=instr->nombreVisible)||nonEqual(param,ps))	{
						tabs->SetSelection(3);
						break;
					}
					AccionAssignPtr trac2=static_cast<AccionAssignPtr>(acTrans[1]);
					trac2->getParametro(tmp);
					trac2->getParametro2(tmp2);
					oss.str(varcuenta);
					oss<<"+1";
					if ((tmp!=varcuenta)||(tmp2!=oss.str()))	{
						tabs->SetSelection(3);
						break;
					}
					tra=trans[1];
					tra->getListaSentencias(acTrans);
					if (acTrans.size()>0)	{
						tabs->SetSelection(3);
						break;
					}
					tra->getEvento(ne);
					tra->getCondicion(co);
					oss.str(idAcc);
					oss<<".done";
					if (ne!=oss.str())	{
						tabs->SetSelection(3);
						break;
					}
					oss.str(varcuenta);
					oss<<"=="<<vartotal;
					if (co!=oss.str())	{
						tabs->SetSelection(3);
						break;
					}
					//At this point, AT LAST, everything is checked and the state actually corresponds to a loop.
					string estadoDestino=tra->getEntranteString();
					tabs->SetSelection(1);
					t2r->setID(idAcc);
					t2r->setAccion(instr);
					t2r->setParametros(param);
					t1r->setID(idAcc);
					t1r->setAccion(instr);
					t1r->setParametros(param);
					oss.str("");
					oss<<vecesBucle;
					t2veces->ChangeValue(_U(oss.str().c_str()));
				}
				break;
			default:tabs->SetSelection(3);
		}
		if (tabs->GetSelection()==3)	{
			t4->setOnentry(onentry);
			t4->setOnexit(onexit);
		}
	}
	ShowModal();
}

void CDEstadoSimple::OnFinalCheckBox(wxEvent &)	{
	if (checkFinal->GetValue()) tabs->Hide();
	else tabs->Show();
}

void CDEstadoSimple::OnOK(wxEvent &)	{
	//Whenever the user presses OK, the state is configured as needed.
	string acc,par;
	vector<AccionPtr> onentry,onexit;
	string s=string(nombreEstado->GetValue().mbc_str());
	if (s.length()==0)	{
		wxMessageBox(_("The state must have a name."),_("Error"),wxICON_ERROR);
		nombreEstado->SetFocus();
		return;
	}
	EstadoPtr ne=estado->getPanel()->getEstadoPorNombre(s);
	if (ne&&(ne.pointer()!=estado.pointer()))	{
		wxMessageBox(_("There already exists a state with that name."),_("Error"),wxICON_ERROR);
		nombreEstado->SetFocus();
		return;
	}
	if (true==(estado->esFinal=checkFinal->GetValue()))	{
		estado->setOnentry(vector<AccionPtr>());
		estado->setOnexit(vector<AccionPtr>());
		estado->eliminarTransiciones();
	}	else switch (pos=tabs->GetSelection())	{
		case 0:	{
			if (t1r->getID().size()==0)	{
				wxMessageBox(_("The instruction must be given an identifier."),_("Error"),wxICON_ERROR);
				t1r->makeFocusID();
				return;
			}
			onentry.push_back(t1r->getAccionRobot());
			estado->eliminarTransiciones();
			dato=estado->getNombre();
			CDPedirEstadoDestino *dlg=new CDPedirEstadoDestino(estado->getPanel(),_("Destination state"));
			enviarDestino(dlg->getSelection());
			delete dlg;
			break;
		}	case 1:	{
			int v=-1;
			if ((sscanf(t2veces->GetValue().mbc_str(),"%d",&v)<0)||v<=0)	{
				wxMessageBox(_("The loop count must be greater than zero."),_("Error"),wxICON_ERROR);
				t2veces->SetFocus();
				return;
			}
			dato=estado->getNombre();
			ostringstream oss;
			oss<<v;
			onentry.push_back(AccionAssignPtr(new AccionAssign(varcuenta,"0")));
			onentry.push_back(AccionAssignPtr(new AccionAssign(vartotal,oss.str())));
			onentry.push_back(t2r->getAccionRobot());
			vector<AccionPtr> sen;
			sen.push_back(t2r->getAccionRobot());
			oss.str(varcuenta);
			oss<<"+1";
			sen.push_back(AccionAssignPtr(new AccionAssign(varcuenta,oss.str())));
			estado->eliminarTransiciones();
			oss.str(dato);
			oss<<".done";
			string ev=oss.str();
			oss.str(varcuenta);
			oss<<'<'<<vartotal;
			TransicionPtr nt=TransicionPtr(new Transicion(estado,EstadoPtr(NULL),ev,oss.str(),sen));
			estado->anadirTransicion(nt);
			Transicion::addTransition(nt);
			CDPedirEstadoDestino *dlg=new CDPedirEstadoDestino(estado->getPanel(),_("Destination state"));
			enviarDestino(dlg->getSelection());
			delete dlg;
			break;
		}	case 2:	{
			dato=string(t3NombreEvento->GetValue().mbc_str());
			dato2=string(t3Condicion->GetValue().mbc_str());
			if (dato==""||dato2=="")	{
				wxMessageBox(_("An event or condition must be provided."),_("Error"),wxICON_ERROR);
				t3Condicion->SetFocus();
				return;
			}
			CDPedirEstadoDestino *dlg=new CDPedirEstadoDestino(estado->getPanel(),_("Destination state"));
			enviarDestino(dlg->getSelection());
			delete dlg;
			estado->eliminarTransiciones();
			break;
		}	case 3:
			t4->getOnentry(onentry);
			t4->getOnexit(onexit);
			estado->redibujar();
			break;
		default:return;
	}
	estado->setOnentry(onentry);
	estado->setOnexit(onexit);
	estado->setNombre(s);
	EndModal(wxID_OK);
}

void CDEstadoSimple::OnCancel(wxEvent &)	{
	EndModal(wxID_CANCEL);
}

void CDEstadoSimple::changeAction(void *pairOfObjects)	{
	pair<TabInstrRobot *,TabInstrRobot *> *objs=static_cast<pair<TabInstrRobot *,TabInstrRobot *> *>(pairOfObjects);
	objs->second->setAccion(objs->first->getAccion());
}

void CDEstadoSimple::changeID(void *pairOfObjects)	{
	pair<TabInstrRobot *,TabInstrRobot *> *objs=static_cast<pair<TabInstrRobot *,TabInstrRobot *> *>(pairOfObjects);
	objs->second->setID(objs->first->getID());
}

void CDEstadoSimple::enviarDestino(const string &s)	{
	string ne="",co="";
	ostringstream oss;
	switch (pos)	{
		case 1:
			//non-common code
			oss.str(varcuenta);
			oss<<"=="<<vartotal;
			co=oss.str();
			//NO BREAK! The "case 0" code also corresponds to case 1.
		case 0:
			oss.str(dato);
			oss<<".done";
			ne=oss.str();
			break;
		case 2:
			ne=dato;
			co=dato2;
			break;
		default:return;
	}
	TransicionPtr nt=TransicionPtr(new Transicion(estado,estado->getPanel()->getEstadoPorNombre(s),ne,co));
	estado->anadirTransicion(nt);
	Transicion::addTransition(nt);
	estado->redibujar();
	EndModal(wxID_OK);
}
