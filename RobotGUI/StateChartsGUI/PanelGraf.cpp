#include <wx/intl.h>
#include <wx/string.h>

#include "Main.h"
#include "CDBorrar.h"
#include "VentanaP.h"
#include "Dibujable.h"
#include "PanelGraf.h"
#include "Transicion.h"
#include "CDTransicion.h"

#include <map>
#include <algorithm>
#include <stdexcept>

using namespace std;
using namespace InterfazSancho;

BEGIN_EVENT_TABLE(PanelGraf,wxPanel)
END_EVENT_TABLE()

const wxSize PanelGraf::minSize=wxSize(800,600);

PanelGraf::PanelGraf(wxWindow *parent,Main *main,wxWindowID id):padre(main)	{
	Create(parent,id,wxDefaultPosition,wxDefaultSize,wxVSCROLL|wxHSCROLL,_T("id"));
	
	SetSize(minSize);
	//SetVirtualSize(minSize);
	//SetScrollbars(1,1,minSize.x+1,minSize.y+1);
	pulsando=DibujablePtr(NULL);
	estado=0;
	contador=1;
	//Flex's default colour is (134,156,167)
	SetBackgroundColour(wxColour(130,150,160));	//I've used this colour, instead of the Flex default, but it has nothing special. Change at will.
	Connect(id,wxEVT_LEFT_DOWN,wxMouseEventHandler(PanelGraf::eventoMouseDown),0,this);
	Connect(id,wxEVT_LEFT_UP,wxMouseEventHandler(PanelGraf::eventoMouseUp),0,this);
}

void PanelGraf::OnDraw(wxDC &dc)	{
	wxScrolledWindow::OnDraw(dc);
	//Changes the size as necessary...
/*	int xMax=minSize.x,yMax=minSize.y;
	wxRect r;
	for (vector<EstadoSimplePtr>::const_iterator it=estadosSimples.begin();it!=estadosSimples.end();++it)	{
		(*it)->rectMinimo(r);
		xMax=max(xMax,static_cast<int>(r.x+r.width));
		yMax=max(yMax,static_cast<int>(r.y+r.height));
	}
	for (vector<EstadoPtr>::const_iterator it=estadosCompuestos.begin();it!=estadosCompuestos.end();++it)	{
		(*it)->rectMinimo(r);
		xMax=max(xMax,static_cast<int>(r.x+r.width));
		yMax=max(yMax,static_cast<int>(r.y+r.height));
	}
	int ox=GetScrollRange(wxHORIZONTAL);
	int oy=GetScrollRange(wxVERTICAL);
	if (ox!=xMax||oy!=yMax)	{
		int px=GetScrollPos(wxHORIZONTAL);
		int py=GetScrollPos(wxVERTICAL);
		SetVirtualSize(wxSize(xMax,yMax));
		SetScrollbars(1,1,xMax,yMax);
		SetScrollPos(wxHORIZONTAL,px,false);
		SetScrollPos(wxVERTICAL,py,false);
	}
	*/
	//... and then draws all the objects.
	for (vector<EstadoPtr>::iterator it=estadosCompuestos.begin();it!=estadosCompuestos.end();++it) (*it)->dibujar(dc);
	for (vector<EstadoSimplePtr>::iterator it=estadosSimples.begin();it!=estadosSimples.end();++it) (*it)->dibujar(dc);
	for (vector<EstadoPtr>::iterator it=estadosCompuestos.begin();it!=estadosCompuestos.end();++it) (*it)->dibujarTransiciones(dc);
	for (vector<EstadoSimplePtr>::iterator it=estadosSimples.begin();it!=estadosSimples.end();++it) (*it)->dibujarTransiciones(dc);

	}

void PanelGraf::generarSCXMLSentencias(const vector<AccionPtr> &instrs,ostringstream &out)	{
	vector<int> ifs;
	for (vector<AccionPtr>::const_iterator it=instrs.begin();it!=instrs.end();++it)	{
		switch ((*it)->getTipoSentencia())	{
			case 1:ifs.push_back(1);break;
			case 2:
				if (ifs.size()==0) throw logic_error("\"Elseif\" found outside of an \"If\" block.");
				else if (ifs.back()!=1) throw logic_error("\"Elseif\" found after \"Else\".");
				break;
			case 3:
				if (ifs.size()==0) throw logic_error("\"Else\" found outside of an \"If\" block.");
				else if (ifs.back()==3) throw logic_error("More than one \"Else\" block found in the same \"If\" block.");
				else ifs.back()=3;
				break;
			case 4:
				if (ifs.size()==0) throw logic_error("\"Endif\" found outside an \"If\" block.");
				ifs.pop_back();
		}
		out<<'<';
		(*it)->getCodigoSCXML(out);
		out<<"/>\n";
	}
}

EstadoPtr PanelGraf::getEstadoPorNombre(const string &s) const	{
	EstadoPtr res=buscarEstadoEnLista(s,estadosSimples);
	return (res?res:buscarEstadoEnLista(s,estadosCompuestos));
}

void PanelGraf::getTodosLosEventos(vector<wxString> &out) const	{
	padre->getEventos(out);
	vector<wxString> est;
	getTodosLosEstados(est);
	vector<string> sentencias,estConFin;
	vector<AccionPtr> se;
	ostringstream oss;
	for (vector<wxString>::const_iterator it=est.begin();it!=est.end();++it)	{
		EstadoPtr e=getEstadoPorNombre(string(it->mbc_str()));
		e->getTodasLasSentencias(se);
		for (vector<AccionPtr>::const_iterator it=se.begin();it!=se.end();++it)	{
			string s=obtenerIDDeSentencia(*it);
			oss.str(s);
			oss<<".done";
			if (s.length()>0) insertarCadenaOrdenada(oss.str(),sentencias);
		}
		if (IS_DERIVED(e,EstadoCompuesto)||IS_DERIVED(e,EstadoParalelo))	{
			oss.str(e->getNombre());
			oss<<".DONE";
			insertarCadenaOrdenada(oss.str(),estConFin);
		}
	}
	for (vector<string>::const_iterator it=sentencias.begin();it!=sentencias.end();++it) out.push_back(_U(it->c_str()));
	for (vector<string>::const_iterator it=estConFin.begin();it!=estConFin.end();++it) out.push_back(_U(it->c_str()));
}

void PanelGraf::getTodosLosEstados(vector<wxString> &out) const	{
	out.clear();
	anadirEstados(estadosSimples,out);
	anadirEstados(estadosCompuestos,out);
}

void PanelGraf::eventoBorrar(DibujablePtr &d)	{
	d->desaparecer();
	{
		vector<EstadoSimplePtr>::iterator itS=findSP(estadosSimples,d);
		if (itS!=estadosSimples.end()) estadosSimples.erase(itS);
		else	{
			vector<EstadoPtr>::iterator itC=findSP(estadosCompuestos,d);
			if (itC!=estadosCompuestos.end()) estadosCompuestos.erase(itC);
		}
	}
	for (vector<EstadoSimplePtr>::iterator it=estadosSimples.begin();it!=estadosSimples.end();++it) (*it)->eliminarObjeto(d);
	for (vector<EstadoPtr>::iterator it=estadosCompuestos.begin();it!=estadosCompuestos.end();++it) (*it)->eliminarObjeto(d);
	Estado::cleanStateList();
	Transicion::cleanTransitionList();
	Refresh();
}

void PanelGraf::eventoCreacionParalelo(size_t i,int px,int py,vector<DibujablePtr> &jer)	{
	ostringstream oss;
	oss<<'S'<<contador;
	EstadoParaleloPtr nuevo=EstadoParaleloPtr(new EstadoParalelo(oss.str(),px,py,i,this));
	Estado::addState(nuevo);
	contador++;
	if (jer.size()>0)	{
		bool b=false;
		for (vector<DibujablePtr>::iterator it=jer.begin();it!=jer.end();++it) if (IS_DERIVED(*it,EstadoCompuesto))	{
			EstadoCompuestoPtr ec=static_cast<EstadoCompuestoPtr>(*it);
			if (b) ec->calcularRectangulo();
			else	{
				ec->anadirSubestado(nuevo,px,py);
				b=true;
			}
		}	else if (IS_DERIVED(*it,EstadoParalelo))	{
			EstadoParaleloPtr ep=static_cast<EstadoParaleloPtr>(*it);
			if (b) ep->calcularRectangulo();
			else	{
				ep->anadirSubestado(nuevo,px,py);
				b=true;
			}
		}
		if (!b) anadirEstadoParalelo(nuevo);
	}	else anadirEstadoParalelo(nuevo);
	Refresh();
}

void PanelGraf::eventoMouseDown(wxMouseEvent &e)	{
	pulsando=DibujablePtr(NULL);
	wxCoord x1=e.GetX(),y1=e.GetY();
	wxCoord x,y;
	CalcUnscrolledPosition(x1,y1,&x,&y);
	vector<DibujablePtr> jer=getObjetos(x,y,estado);
	switch (estado)	{
		case -1:	//Options/move. Mark the object as selected.
			if (jer.size()>0)	{
				pulsando=jer[0];
				pulsadoP=wxPoint(x,y);
				jerPulsando=vector<DibujablePtr>(jer.begin()+1,jer.end());
				pulsando->activar();
				Refresh();
			}
			return;
		case 0:	//New simple state (shares most code with the compound state).
		case 1:	{	//New compound state.
			ostringstream oss;
			oss<<'S'<<contador;
			contador++;
			EstadoPtr e(NULL);
			if (estado==0)	{
				EstadoSimplePtr es=EstadoSimplePtr(new EstadoSimple(oss.str(),x,y,this));
				if (jer.size()==0) anadirEstadoSimple(es);
				e=es;
			}	else	{
				EstadoCompuestoPtr ec=EstadoCompuestoPtr(new EstadoCompuesto(oss.str(),x,y,this));
				if (jer.size()==0) anadirEstadoCompuesto(ec);
				e=ec;
			}
			Estado::addState(e);
			if (jer.size()>0)	{
				//The state must be added to a hierarchy
				bool b=false;
				for (vector<DibujablePtr>::iterator it=jer.begin();it!=jer.end();++it) if (IS_DERIVED(*it,EstadoCompuesto))	{
					EstadoCompuestoPtr ec=static_cast<EstadoCompuestoPtr>(*it);
					if (b) ec->calcularRectangulo();
					else	{
						ec->anadirSubestado(e,x,y);
						b=true;
					}
				}	else if (IS_DERIVED(*it,EstadoParalelo))	{
					EstadoParaleloPtr ep=static_cast<EstadoParaleloPtr>(*it);
					if (b) ep->calcularRectangulo();
					else	{
						ep->anadirSubestado(e,x,y);
						b=true;
					}
				}
				if (!b)	{
					if (estado==0) anadirEstadoSimple(static_cast<EstadoSimplePtr>(e));
					else anadirEstadoCompuesto(static_cast<EstadoCompuestoPtr>(e));
				}
			}
			Refresh();
			return;
		}	case 2:	//New parallel state.
			new VentanaP(this,x,y,jer);
			return;
		case 3:	//Delete an object.
			if (jer.size()==0) return;
			else if (jer.size()==1) eventoBorrar(jer[0]);
			else	{
				CDBorrar dlg(jer,this);
				DibujablePtr p=dlg.getSelected();
				if (p) eventoBorrar(p);
			}
			return;
		case 4:	//Create a transition.
			if (jer.size()==0) return;
			if (!IS_DERIVED(jer[0],Estado)) return;
			if (IS_DERIVED(jer[0],EstadoSimple)&&(static_cast<EstadoSimplePtr>(jer[0])->esFinal)) return;
			pulsando=static_cast<EstadoPtr>(jer[0]);
			pulsadoP=wxPoint(x,y);
			pulsando->activar();
			Refresh();
			return;
	}
}

template<typename STATE_LIST> inline void deleteState(const EstadoPtr &e,STATE_LIST &lists)	{
	for (typename STATE_LIST::iterator it=lists.begin();it!=lists.end();++it) if (it->pointer()==e.pointer())	{
		lists.erase(it);
		return;
	}
}

bool isInside(const EstadoPtr &e1,const EstadoPtr &e2)	{
	//e2 is either an EstadoCompuesto or an EstadoParalelo
	if (!e2) return false;
	vector<EstadoPtr> subestados;
	if (IS_DERIVED(e2,EstadoCompuesto)) static_cast<EstadoCompuestoPtr>(e2)->getSubestados(subestados);
	else if (IS_DERIVED(e2,EstadoParalelo)) static_cast<EstadoParaleloPtr>(e2)->getSubestados(subestados);
	for (vector<EstadoPtr>::const_iterator it=subestados.begin();it!=subestados.end();++it) if (*it)	{
		if (it->pointer()==e1.pointer()) return true;
		if (IS_DERIVED(*it,EstadoCompuesto)||IS_DERIVED(*it,EstadoParalelo)) if (isInside(e1,*it)) return true;
	}
	return false;
}

void PanelGraf::eventoMouseUp(wxMouseEvent &e)	{
	//Mouse raised. Some actions (options/move/create transition) only take place now, and not when the mouse is pressed.
	if (!pulsando) return;
	pulsando->desactivar();
	wxCoord x1=e.GetX(),y1=e.GetY();
	wxCoord x,y;
	CalcUnscrolledPosition(x1,y1,&x,&y);
	if (estado!=-1&&estado!=4) return;
	vector<DibujablePtr> jer=getObjetos(x,y,estado);
	switch (estado)	{
		case -1:
			if (pulsadoP.x==x&&pulsadoP.y==y)	{	//The state hasn't been moved. So, the dialog box is called.
				pulsando->cuadroDeDialogo();
				Refresh();
				return;
			}
			if (IS_DERIVED(pulsando,Transicion)) for (vector<DibujablePtr>::iterator it=jer.begin();it!=jer.end();++it)	{
				if (IS_DERIVED(*it,Estado))	{
					static_cast<TransicionPtr>(pulsando)->setEntrante(static_cast<EstadoPtr>(*it));
					break;
				}
			}	else	{
				//A state has been moved.
				EstadoPtr e=static_cast<EstadoPtr>(pulsando);
				e->moverAbs(x,y);
				/*
					Now, there are four possible cases:
					A) The state is free (i.e. not inside other state) and the destination spot is also free.
					B) The state is free but is now being introduced into another state, as a substate.
					C) The state was inside other state, but is going to be extracted into a free spot.
					D) The state was inside other state and is being introduced into a third one.
					In any case, all the information we need is:
						- The compound state inside which this state was (NULL if free).
						- The compound state into which this state is going to enter (NULL if free).
					We can extract both data from "jer" and "jerPulsando" variables.
				*/
				EstadoPtr movingFrom(NULL),movingInto(NULL);
				for (vector<DibujablePtr>::iterator it=jerPulsando.begin();it!=jerPulsando.end();++it) if (IS_DERIVED(*it,EstadoCompuesto)||IS_DERIVED(*it,EstadoParalelo))	{
					movingFrom=static_cast<EstadoPtr>(*it);
					break;
				}
				for (vector<DibujablePtr>::iterator it=jer.begin();it!=jer.end();++it) if ((it->pointer()!=e.pointer())&&(IS_DERIVED(*it,EstadoCompuesto)||IS_DERIVED(*it,EstadoParalelo)))	{
					movingInto=static_cast<EstadoPtr>(*it);
					if (isInside(movingInto,e)) movingInto=EstadoPtr(NULL);	//This will avoid stack overflows: do not try to insert a state into one of its children.
					break;
				}
				//An additional check may be done to guarantee that both "father" states are different, but there would be no difference in the behaviour.
				if (movingFrom||movingInto)	{
					if (movingFrom) movingFrom->eliminarObjeto(e);	//Delete from movingFrom
					else if (IS_DERIVED(e,EstadoSimple)) deleteState(e,estadosSimples);	//The state was not inside another; delete from PanelGraf's state lists.
					else deleteState(e,estadosCompuestos);
					if (movingInto)	{	//Insert into movingInto
						if (IS_DERIVED(movingInto,EstadoCompuesto)) static_cast<EstadoCompuestoPtr>(movingInto)->anadirSubestado(e,x,y);
						else static_cast<EstadoParaleloPtr>(movingInto)->anadirSubestado(e,x,y);
					}	else if (IS_DERIVED(e,EstadoSimple)) anadirEstadoSimple(static_cast<EstadoSimplePtr>(e));	//Not inserting into a state, but extracting from one...
					else if (IS_DERIVED(e,EstadoCompuesto)) anadirEstadoCompuesto(static_cast<EstadoCompuestoPtr>(e));
					else anadirEstadoParalelo(static_cast<EstadoParaleloPtr>(e));
				}
				//Now there should be a piece of code recalculating each state's rectangle, but that will be done the next time Refresh() is called. Which is right now ;).
			}
			break;
		case 4:	{
			if (!IS_DERIVED(pulsando,Estado)) break;
			EstadoPtr e=static_cast<EstadoPtr>(pulsando);
			for (vector<DibujablePtr>::iterator it=jer.begin();it!=jer.end();++it) if (IS_DERIVED(*it,Estado))	{
				TransicionPtr t=TransicionPtr(new Transicion(e,static_cast<EstadoPtr>(*it)));
				Transicion::addTransition(t);
				e->anadirTransicion(t);
				CDTransicion(this,t);
				break;
			}
			break;
		}
	}
	Refresh();
}

vector<DibujablePtr> PanelGraf::getObjetos(int x,int y,int est) const	{
	wxPoint p(x,y);
	if (est==3) return getTodosLosObjetos(p);
	//The list is covered backwards, to obtain firstle the "nearest" (most to the front) object.
	vector<DibujablePtr> res;
	for (vector<EstadoSimplePtr>::const_reverse_iterator it=estadosSimples.rbegin();it!=estadosSimples.rend();++it)	{
		if (estado!=4) (*it)->getJerarquiaSubestados(p,res);
		else (*it)->getJerarquiaSubestadosSinTransiciones(p,res);
		if (res.size()>0) return res;
	}
	for (vector<EstadoPtr>::const_reverse_iterator it=estadosCompuestos.rbegin();it!=estadosCompuestos.rend();++it)	{
		if (estado!=4) (*it)->getJerarquiaSubestados(p,res);
		else (*it)->getJerarquiaSubestadosSinTransiciones(p,res);
		if (res.size()>0) return res;
	}
	return vector<DibujablePtr>(0);
}

vector<DibujablePtr> PanelGraf::getTodosLosObjetos(const wxPoint &p) const	{
	vector<DibujablePtr> res,tmp;
	for (vector<EstadoSimplePtr>::const_iterator it=estadosSimples.begin();it!=estadosSimples.end();++it)	{
		(*it)->getTodosLosObjetos(p,tmp);
		res.insert(res.end(),tmp.begin(),tmp.end());
	}
	for (vector<EstadoPtr>::const_iterator it=estadosCompuestos.begin();it!=estadosCompuestos.end();++it)	{
		(*it)->getTodosLosObjetos(p,tmp);
		res.insert(res.end(),tmp.begin(),tmp.end());
	}
	return res;
}

/**
  * Helper type to write recursive treelike data models.
  * This is quite a complex type, especially for a temporary, private one. It basically works this way: starging from an empty variable container, the variables are
  * added, so that the generation fails if a value is tried to be inserted where a subtree already exists, or vice versa. If all the variables have been successfully
  * added to the tree, then the SCXML code can be very easily generated. This is somewhat complex, but works perfectly and does everything, including sanity checks,
  * exactly when and where they are needed.
  */
struct VariableMap	{
private:
	string name;	//!< Name of this variable or subtree.
	union	{	//!< Union of pointers. Maximum efficiency!
		string *val;	//!< String with the value.
		vector<VariableMap *> *internalData;	//!< String with the subtree data.
	}	data;
	/**
	  * If this variable is true AND the pointer is not null, then no subtree can be added.
	  * If this variable is false AND the pointer is not null, then no value can be added.
	  * If the pointer is null, then anything can be added, since the object exists but hasn't been refilled.
	  */
	bool isValue;
	inline VariableMap(const VariableMap &m);	//!< Copy constructor. Private. Without code. No copy can be performed, then. Use pointers!
	inline void operator=(const VariableMap &m);	//!< Copy operator. Private. Without code. No copy can be performed, so use pointers.
	static void generateSCXML(const VariableMap *vm,ostringstream &oss)	{	//!< Generates the SCXML code of this variable tree.
		oss<<'<'<<vm->name<<'>';
		if (vm->isValue) oss<<(*(vm->data.val));
		else	{
			oss<<'\n';
			for (vector<VariableMap *>::const_iterator it=vm->data.internalData->begin();it!=vm->data.internalData->end();++it) generateSCXML(*it,oss);
		}
		oss<<"</"<<vm->name<<">\n";
	}
public:
	inline VariableMap()	{	//!< Basic constructor.
		name="";
		data.val=NULL;
		isValue=false;
	}
	inline VariableMap(const string &varNameAndValue)	{	//!< Constructor with name (possibly with a hierarchy) and value of a variable in the same string.
		name="";
		data.val=NULL;
		isValue=false;
		addVariable(varNameAndValue);
	}
	inline VariableMap(const string &varName,const string &value)	{	//!< Constructor with name (possibly with a hierarchy) and value of a variable.
		name="";
		data.val=NULL;
		isValue=false;
		addVariable(varName,value);
	}
	inline VariableMap(const vector<string> &varHierarchy,const string &value)	{	//!< Constructor with hierarchy of names and value.
		name="";
		data.val=NULL;
		isValue=false;
		addVariable(varHierarchy,value);
	}
	bool addVariable(const string &varName,const string &value)	{	//!< Adds a variable, stored as two strings (name and value).
		static vector<string> hierarchy;
		parseVariableName(varName,hierarchy);
		return addVariable(hierarchy,value);
	}
	bool addVariable(const string &varNameAndValue)	{	//!< Adds a variable, stored as a string (name=value).
		static string vName,value;
		if (!parseVariable(varNameAndValue,vName,value)) return false;
		else return addVariable(vName,value);
	}
	bool addVariable(const vector<string> &hierarchy,const string &value)	{	//!< Adds a variable, with its hierarchy unrolled.
		if (hierarchy.size()==0) return false;
		if (!data.val)	{
			name=hierarchy[0];
			if (hierarchy.size()==1)	{
				data.val=new string(value);
				isValue=true;
			}	else	{
				data.internalData=new vector<VariableMap *>();
				data.internalData->push_back(new VariableMap(vector<string>(hierarchy.begin()+1,hierarchy.end()),value));
				isValue=false;
			}
		}	else if (hierarchy[0]!=name) return false;
		else if (hierarchy.size()==1) return false;
		else if (isValue) return false;
		else	{
			bool inserted;
			string newName=hierarchy[1];
			for (vector<VariableMap *>::iterator it=data.internalData->begin();it!=data.internalData->end();++it) if ((*it)->name==newName)	{
				(*it)->addVariable(vector<string>(hierarchy.begin()+1,hierarchy.end()),value);
				inserted=true;
				break;
			}
			if (!inserted) data.internalData->push_back(new VariableMap(vector<string>(hierarchy.begin()+1,hierarchy.end()),value));
		}
		return true;
	}
	void generateSCXML(ostringstream &oss) const	{	//!< Generates the SCXML of a TOP-LEVEL map. Will probably fail otherwise.
		if (isValue) throw logic_error("Wrong variable specification");
		if (data.val)	{
			oss<<"<datamodel>\n<data name=\""<<name<<">\n";
			for (vector<VariableMap *>::iterator it=data.internalData->begin();it!=data.internalData->end();++it) generateSCXML(*it,oss);
			oss<<"</data>\n</datamodel>\n";
		}
	}
	inline ~VariableMap()	{	//!< Destructor. Deletes the pointers.
		if (data.val)	{
			if (isValue) delete data.val;
			else	{
				for (vector<VariableMap *>::iterator it=data.internalData->begin();it!=data.internalData->end();++it) delete *it;
				delete data.internalData;
			}
		}
	}
	static bool parseVariable(const string &nameAndValue,string &name,string &value)	{	//!< Parses a string into the variable name and the value.
		string::size_type pos=nameAndValue.find('=');
		if (pos==string::npos) return false;
		name=nameAndValue.substr(0,pos);
		value=nameAndValue.substr(pos+1);
		return true;
	}
	static void parseVariableName(const string &name,vector<string> &hierarchy)	{	//!< Parses a variable name into the full hierarchy.
		hierarchy.clear();
		if (name.length()==0) return;
		string::size_type pos=0;
		for (;;)	{
			string::size_type pos2=name.find('.',pos);
			if (pos2==string::npos)	{
				hierarchy.push_back(name.substr(pos2+1));
				return;
			}	else	{
				hierarchy.push_back(name.substr(pos,pos2));
				pos=pos2+1;
			}
		}
	}
};

string PanelGraf::generarSCXML(const string &initialStateName) const	{
	ostringstream oss;
	oss<<"<?xml version=\"1.0\"?>\n<scxml initialstate=\""<<initialStateName<<"\">\n";
	VariableMap varData;
	vector<string> vars;
	{
		vector<wxString> wxss;
		padre->getVariables(wxss);
		vars.resize(wxss.size());
		for (size_t i=0;i<wxss.size();++i) vars[i]=string(wxss[i].mbc_str());
	}
	for (vector<string>::const_iterator it=vars.begin();it!=vars.end();++it) if (!varData.addVariable(*it)) throw std::logic_error("Wrong variable specification.");
	varData.generateSCXML(oss);
	for (vector<EstadoSimplePtr>::const_iterator it=estadosSimples.begin();it!=estadosSimples.end();++it) (*it)->generarSCXML(oss);
	for (vector<EstadoPtr>::const_iterator it=estadosCompuestos.begin();it!=estadosCompuestos.end();++it) (*it)->generarSCXML(oss);
	oss<<"</scxml>";
	return oss.str();
}

string PanelGraf::obtenerIDDeSentencia(const AccionPtr &ac)	{
	if (IS_DERIVED(ac,AccionRobot))	{
		std::string str;
		ac->getParametro2(str);
		return str;
	}	else return "";
}
