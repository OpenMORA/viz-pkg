#include "EstadoParalelo.h"
#include <wx/colour.h>

#include "PanelGraf.h"
#include "CDEstadoParalelo.h"

using namespace std;
using namespace mrpt::utils;
using namespace InterfazSancho;

IMPLEMENTS_MRPT_OBJECT(EstadoParalelo,Estado,InterfazSancho)

void EstadoParalelo::activar()	{
	pulsado=true;
}

void EstadoParalelo::anadirSubestado(EstadoPtr &d,int x,int y)	{
	d->moverAbs(x,y);
	int donde=-1;
	for (int i=0;i<static_cast<int>(dims.size());++i) if (x-rect.x<int(dims[i]))	{
		donde=i;
		break;
	}
	if (donde<0) donde=dims.size()-1;
	if (!subestados[donde]) subestados[donde]=d;
	else	{
		//There already is a state where we want to insert this! We will try to insert the state into the deepest parallel or compound state.
		vector<DibujablePtr> jer;
		subestados[donde]->getJerarquiaSubestadosSinTransiciones(wxPoint(x,y),jer);
		bool metido=false;
		for (vector<DibujablePtr>::const_iterator it=jer.begin();it!=jer.end();++it)	{
			if (IS_DERIVED(*it,EstadoCompuesto))	{	//Insert into compound state.
				static_cast<EstadoCompuestoPtr>(*it)->anadirSubestado(d,x,y);
				metido=true;
				break;
			}	else if (IS_DERIVED(*it,EstadoParalelo)&&(it->pointer()!=this))	{	//Insert into parallel state.
				static_cast<EstadoParaleloPtr>(*it)->anadirSubestado(d,x,y);
				metido=true;
				break;
			}
		}
		if (!metido) combinar(subestados[donde],d,subestados[donde]);	//No compound or parallel state found! Two states must be combined into a compound one.
	}
	calcularRectangulo();
}

void EstadoParalelo::anadirTransicion(const TransicionPtr &t)	{
	transiciones.push_back(t);
}

void EstadoParalelo::calcularRectangulo()	{
	dims.resize(subestados.size());
	int x=0;
	int y=yMin;
	for (size_t i=0;i<subestados.size();++i)	{
		wxSize id=tamano(subestados[i]);
		dims[i]=(x+=id.x);
		y=max(y,id.y);
	}
	int xL=centro.x-round(static_cast<double>(x)/2);
	rect=wxRect(xL,centro.y-round(static_cast<double>(y)/2),x,y);
	for (size_t i=0;i<subestados.size();++i) if (subestados[i]) subestados[i]->moverAbs(xL+round(static_cast<double>(((i==0)?0:dims[i-1])+dims[i])/2),centro.y);
}

void EstadoParalelo::combinar(EstadoPtr e1,EstadoPtr e2,EstadoPtr &com)	{
	ostringstream nameCreator;
	nameCreator<<nombre<<"_"<<contador;
	EstadoCompuestoPtr res=EstadoCompuestoPtr(new EstadoCompuesto(nameCreator.str(),0,0,panel));
	addState(res);	//Add to the global state repository...
	contador++;
	wxPoint p1,p2;
	e1->getCentro(p1);
	e2->getCentro(p2);
	res->anadirSubestado(e1,p1.x,p1.y);
	res->anadirSubestado(e2,p2.x,p2.y);
	com=res;
}

void EstadoParalelo::cuadroDeDialogo()	{
	CDEstadoParalelo dlg(static_cast<EstadoParaleloPtr>(Estado::getSmartPtr(this)),getPanel());
}

void EstadoParalelo::desactivar()	{
	pulsado=false;
}

void EstadoParalelo::desaparecer()	{
	for (vector<EstadoPtr>::iterator it=subestados.begin();it!=subestados.end();++it) if (*it) (*it)->desaparecer();
	while (transiciones.size()>0) transiciones[0]->desaparecer();
}

void EstadoParalelo::dibujar(wxDC &dc)	{
	calcularRectangulo();
	const wxPen &penBorde=pulsado?borde2P:bordeP;
	const wxBrush &brushBorde=pulsado?borde2B:bordeB;
	const wxColour &colorBorde=pulsado?borde2C:bordeC;
	dc.SetPen(penBorde);
	dc.SetBrush(brushBorde);
	dc.DrawRoundedRectangle(rect,3);
	dc.SetPen(interiorP);
	dc.SetBrush(interiorB);
	dc.DrawRoundedRectangle(rect.x+GROSOR,rect.y+GROSOR,rect.width-GROSOR-GROSOR,rect.height-GROSOR-GROSOR,3);
	dc.SetPen(penBorde);
	dc.SetBrush(brushBorde);
	for (size_t i=0;i<dims.size()-1;++i)	{
		dc.DrawLine(rect.x+dims[i]-GROSOR/2,rect.y+GROSOR/2,rect.x+dims[i]-GROSOR/2,rect.y+rect.height-GROSOR/2);
		dc.DrawLine(rect.x+dims[i]+GROSOR/2,rect.y+GROSOR/2,rect.x+dims[i]+GROSOR/2,rect.y+rect.height-GROSOR/2);
		dc.FloodFill(rect.x+dims[i],centro.y,colorBorde,wxFLOOD_BORDER);
	}
	for (vector<EstadoPtr>::iterator it=subestados.begin();it!=subestados.end();++it) if (*it) (*it)->dibujar(dc);
}

void EstadoParalelo::dibujarTransiciones(wxDC &dc)	{
	for (vector<TransicionPtr>::iterator it=transiciones.begin();it!=transiciones.end();++it) (*it)->dibujar(dc);
	for (vector<EstadoPtr>::iterator it=subestados.begin();it!=subestados.end();++it) if (*it) (*it)->dibujarTransiciones(dc);
}

void EstadoParalelo::eliminarObjeto(DibujablePtr &d)	{
	if (IS_DERIVED(d,Transicion))	{
		if (findSP(transiciones,d)!=transiciones.end()) d->desaparecer();
	}	else	{
		for (size_t i=0;i<subestados.size();++i) if (subestados[i].pointer()==d.pointer())	{
			subestados[i]=EstadoPtr(NULL);
			cleanStateList();
			calcularRectangulo();
			return;
		}
		for (vector<TransicionPtr>::iterator it=transiciones.begin();it!=transiciones.end();++it) if ((*it)->esEntrante(static_cast<EstadoPtr>(d))) (*it)->desaparecer();
	}
	for (vector<EstadoPtr>::iterator it=subestados.begin();it!=subestados.end();++it) if (*it) (*it)->eliminarObjeto(d);
	cleanStateList();
	calcularRectangulo();
}

void EstadoParalelo::eliminarTransicion(const TransicionPtr &t)	{
	vector<TransicionPtr>::iterator pos=findSP(transiciones,t);
	if (pos!=transiciones.end()) transiciones.erase(pos);
	int enBucle=0;
	for (vector<TransicionPtr>::iterator it=transiciones.begin();it!=transiciones.end();++it) if ((*it)->esEntrante(getSmartPtr(this))) (*it)->setEnBucle(++enBucle);
	Transicion::cleanTransitionList();
}

void EstadoParalelo::eliminarTransiciones()	{
	transiciones.clear();
	panel->Refresh();
	Transicion::cleanTransitionList();
}

bool EstadoParalelo::esPunto(const wxPoint &p) const	{
	return rect.Contains(p);
}

void EstadoParalelo::generarSCXML(ostringstream &oss) const	{
	oss<<"<parallel id=\""<<nombre<<"\">\n";
	try	{
		for (vector<TransicionPtr>::const_iterator it=transiciones.begin();it!=transiciones.end();++it) (*it)->generarSCXML(oss);
		if (onentry.size()>0)	{
			oss<<"<onentry>";
			PanelGraf::generarSCXMLSentencias(onentry,oss);
			oss<<"</onentry>";
		}
		if (onexit.size()>0)	{
			oss<<"<onexit>";
			PanelGraf::generarSCXMLSentencias(onexit,oss);
			oss<<"</onexit>";
		}
		int cuantos=0;
		for (vector<EstadoPtr>::const_iterator it=subestados.begin();it!=subestados.end();++it) if (*it)	{
			cuantos++;
			(*it)->generarSCXML(oss);
		}
		if (cuantos<2) throw logic_error("Parallel state is not completely specified. Not enough parallel children.\n");
	}	catch (const logic_error &e)	{
		ostringstream errstrm;
		errstrm<<nombre<<": "<<e.what();
		throw logic_error(errstrm.str());
	}
	oss<<"</parallel>\n";
}

void EstadoParalelo::getCentro(wxPoint &p) const	{
	p=centro;
}

void EstadoParalelo::getJerarquiaSubestados(const wxPoint &p,vector<DibujablePtr> &vec) const	{
	vec.clear();
	for (vector<TransicionPtr>::const_iterator it=transiciones.begin();it!=transiciones.end();++it) if ((*it)->esPunto(p))	{
		vec.push_back(*it);
		return;
	}
	for (vector<EstadoPtr>::const_iterator it=subestados.begin();it!=subestados.end();++it) if (*it)	{
		(*it)->getJerarquiaSubestados(p,vec);
		if (vec.size()!=0&&!IS_DERIVED(vec[0],Transicion))	{
			vec.push_back(getSmartPtr(this));
			return;
		}
	}
	if (esPunto(p)) vec.push_back(getSmartPtr(this));
}

void EstadoParalelo::getJerarquiaSubestadosSinTransiciones(const wxPoint &p,vector<DibujablePtr> &vec) const	{
	for (vector<EstadoPtr>::const_iterator it=subestados.begin();it!=subestados.end();++it) if (*it)	{
		(*it)->getJerarquiaSubestadosSinTransiciones(p,vec);
		if (vec.size()!=0)	{
			vec.push_back(getSmartPtr(this));
			return;
		}
	}
	if (esPunto(p)) vec.push_back(getSmartPtr(this));
}

void EstadoParalelo::getListaTransiciones(vector<TransicionPtr> &vec) const	{
	vec=transiciones;
}

const string &EstadoParalelo::getNombre() const	{
	return nombre;
}

void EstadoParalelo::getOnentry(vector<AccionPtr> &vec) const	{
	vec=onentry;
}

void EstadoParalelo::getOnexit(vector<AccionPtr> &vec) const	{
	vec=onexit;
}

PanelGraf *EstadoParalelo::getPanel() const	{
	return panel;
}

int EstadoParalelo::getSiguienteIDInstruccion()	{
	return ++idinstr;
}

void EstadoParalelo::getTodasLasSentencias(vector<AccionPtr> &vec) const	{
	vec=onentry;
	vec.insert(vec.end(),onexit.begin(),onexit.end());
	vector<AccionPtr> tmp;
	for (vector<TransicionPtr>::const_iterator it=transiciones.begin();it!=transiciones.end();++it)	{
		(*it)->getListaSentencias(tmp);
		vec.insert(vec.end(),tmp.begin(),tmp.end());
	}
}

void EstadoParalelo::getTodosLosObjetos(const wxPoint &p,vector<DibujablePtr> &vec) const	{
	vec.clear();
	for (vector<TransicionPtr>::const_iterator it=transiciones.begin();it!=transiciones.end();++it)	if ((*it)->esPunto(p)) vec.push_back(*it);
	vector<DibujablePtr> tmp;
	for (vector<EstadoPtr>::const_iterator it=subestados.begin();it!=subestados.end();++it) if (*it)	{
		(*it)->getTodosLosObjetos(p,tmp);
		vec.insert(vec.end(),tmp.begin(),tmp.end());
	}
	if (esPunto(p)) vec.push_back(getSmartPtr(this));
}

int EstadoParalelo::getTransicionesEnBucle() const	{
	int res=0;
	for (vector<TransicionPtr>::const_iterator it=transiciones.begin();it!=transiciones.end();++it) if ((*it)->esEntrante(getSmartPtr(this))) res++;
	return res;
}

void EstadoParalelo::mover(double x,double y)	{
	centro.x+=x;
	centro.y+=y;
	for (std::vector<EstadoPtr>::iterator it=subestados.begin();it!=subestados.end();++it) if (*it) (*it)->mover(x,y);
	calcularRectangulo();
}

void EstadoParalelo::moverAbs(double x,double y)	{
	mover(x-centro.x,y-centro.y);
}

void EstadoParalelo::puntosDeSalida(vector<wxPoint> &vec) const	{
	vec.resize(8);
	//The order is not the same that in the Flex code. This shouldn't matter.
	vec[0]=wxPoint(rect.x+rect.width,centro.y);
	vec[1]=wxPoint(rect.x+rect.width,rect.y);
	vec[2]=wxPoint(centro.x,rect.y);
	vec[3]=wxPoint(rect.x,rect.y);
	vec[4]=wxPoint(rect.x,centro.y);
	vec[5]=wxPoint(rect.x,rect.y+rect.height);
	vec[6]=wxPoint(centro.x,rect.y+rect.height);
	vec[7]=wxPoint(rect.x+rect.width,rect.y+rect.height);
}

void EstadoParalelo::rectMinimo(wxRect &r) const	{
	r=rect;
}

void EstadoParalelo::redibujar()	{
	panel->Refresh();
}

void EstadoParalelo::redimensionar(int t)	{
	int oldSize=subestados.size();
	if (t==oldSize) return;
	if (t>oldSize)	{
		subestados.resize(t);
		for (int i=oldSize;i<t;++i) subestados[i]=EstadoPtr(NULL);
	}	else	{
		for (int i=t;i<oldSize;++i) if (subestados[i]) subestados[i]->desaparecer();
		subestados.resize(t);
		cleanStateList();	//Clean unused states from the global repository
	}
	calcularRectangulo();
	redibujar();
}

void EstadoParalelo::setNombre(const string &s)	{
	nombre=s;
}

void EstadoParalelo::setOnentry(const vector<AccionPtr> &vec)	{
	onentry=vec;
}

void EstadoParalelo::setOnexit(const vector<AccionPtr> &vec)	{
	onexit=vec;
}

wxSize EstadoParalelo::tamano(const EstadoPtr &e)	{
	if (!e) return wxSize(xMin,yMin);
	if (IS_DERIVED(e,EstadoCompuesto)) static_cast<EstadoCompuestoPtr>(e)->calcularRectangulo();
	if (IS_DERIVED(e,EstadoParalelo)) static_cast<EstadoParaleloPtr>(e)->calcularRectangulo();
	wxRect r;
	e->rectMinimo(r);
	return wxSize(r.width+ESPACIO,r.height+ESPACIO);
}
