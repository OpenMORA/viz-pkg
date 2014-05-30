#include "EstadoCompuesto.h"

#include <stdexcept>

#include "PanelGraf.h"
#include "CDEstadoCompuesto.h"

using namespace std;
using namespace mrpt::utils;
using namespace InterfazSancho;

IMPLEMENTS_MRPT_OBJECT(EstadoCompuesto,Estado,InterfazSancho)

void EstadoCompuesto::activar()	{
	pulsado=true;
}

void EstadoCompuesto::anadirSubestado(EstadoPtr &d,int x,int y)	{
	d->moverAbs(x,y);
	estados.push_back(d);
	calcularRectangulo();
}

void EstadoCompuesto::anadirTransicion(const TransicionPtr &t)	{
	transiciones.push_back(t);
}

void EstadoCompuesto::calcularRectangulo()	{
	if (estados.size()==0)	{
		rect.x=centro.x-espX;
		rect.y=centro.y-espY;
		rect.width=espX+espX;
		rect.height=espY+espY;
	}	else	{
		int xL=centro.x,xR=centro.x,yU=centro.y,yD=centro.y;
		wxRect r;
		for (vector<EstadoPtr>::const_iterator it=estados.begin();it!=estados.end();++it)	{
			if (IS_DERIVED(*it,EstadoCompuesto)) static_cast<EstadoCompuestoPtr>(*it)->calcularRectangulo();
			else if (IS_DERIVED(*it,EstadoParalelo)) static_cast<EstadoParaleloPtr>(*it)->calcularRectangulo();
			(*it)->rectMinimo(r);
			xL=min(xL,r.x);
			xR=max(xR,r.x+r.width);
			yU=min(yU,r.y);
			yD=max(yD,r.y+r.height);
		}
		xL-=espX;
		xR+=espX;
		yU-=espY;
		yD+=espY;
		centro.x=(xL+xR)/2;
		centro.y=(yU+yD)/2;
		rect.x=xL;
		rect.y=yU;
		rect.width=xR-xL;
		rect.height=yD-yU;
	}
}

void EstadoCompuesto::cuadroDeDialogo()	{
	CDEstadoCompuesto dlg(static_cast<EstadoCompuestoPtr>(Estado::getSmartPtr(this)),getPanel());
}

void EstadoCompuesto::desactivar()	{
	pulsado=false;
}

void EstadoCompuesto::desaparecer()	{
	for (vector<EstadoPtr>::iterator it=estados.begin();it!=estados.end();++it) (*it)->desaparecer();
	while (transiciones.size()>0) transiciones[0]->desaparecer();
}

void EstadoCompuesto::dibujar(wxDC &dc)	{
	calcularRectangulo();
	dc.SetPen(pulsado?borde2P:bordeP);
	dc.SetBrush(pulsado?borde2B:bordeB);
	dc.DrawRoundedRectangle(rect,3);
	dc.SetPen(interiorP);
	dc.SetBrush(interiorB);
	dc.DrawRoundedRectangle(rect.x+GROSOR,rect.y+GROSOR,rect.width-GROSOR-GROSOR,rect.height-GROSOR-GROSOR,3);
	for (vector<EstadoPtr>::iterator it=estados.begin();it!=estados.end();++it) (*it)->dibujar(dc);
}

void EstadoCompuesto::dibujarTransiciones(wxDC &dc)	{
	for (vector<TransicionPtr>::iterator it=transiciones.begin();it!=transiciones.end();++it) (*it)->dibujar(dc);
	for (vector<EstadoPtr>::iterator it=estados.begin();it!=estados.end();++it) (*it)->dibujarTransiciones(dc);
}

void EstadoCompuesto::eliminarObjeto(DibujablePtr &d)	{
	if (IS_DERIVED(d,Transicion))	{
		if (findSP(transiciones,d)!=transiciones.end()) d->desaparecer();
	}	else	{
		vector<EstadoPtr>::iterator dPos=findSP(estados,d);
		if (dPos!=estados.end())	{
			estados.erase(dPos);
			if (d.pointer()==eInicial.pointer()) eInicial=EstadoPtr(NULL);
		}
		size_t idx=0;
		EstadoPtr e;
		while (idx<transiciones.size())	{
			TransicionPtr t=transiciones[idx];
			t->getEntrante(e);
			if (e.pointer()==d.pointer()) t->desaparecer();
			else idx++;
		}
	}
	for (vector<EstadoPtr>::iterator it=estados.begin();it!=estados.end();++it) (*it)->eliminarObjeto(d);
}

void EstadoCompuesto::eliminarTransicion(const TransicionPtr &t)	{
	vector<TransicionPtr>::iterator pos=findSP(transiciones,t);
	if (pos==transiciones.end()) return;
	transiciones.erase(pos);
	int enBucle=0;
	for (vector<TransicionPtr>::iterator it=transiciones.begin();it!=transiciones.end();++it) if ((*it)->esEntrante(getSmartPtr(this))) (*it)->setEnBucle(++enBucle);
	Transicion::cleanTransitionList();
}

void EstadoCompuesto::eliminarTransiciones()	{
	transiciones.clear();
	padre->Refresh();
	Transicion::cleanTransitionList();
}

bool EstadoCompuesto::esPunto(const wxPoint &p) const	{
	return rect.Contains(p);
}

void EstadoCompuesto::generarSCXML(ostringstream &oss) const	{
	try	{
		if (!eInicial) throw new logic_error("No initial state has been specified.");
		oss<<"<state id=\""<<nombre<<"\">\n<initial>\n<transition target=\""<<eInicial->getNombre()<<"\"/>\n</initial>\n";
		for (vector<TransicionPtr>::const_iterator it=transiciones.begin();it!=transiciones.end();++it) (*it)->generarSCXML(oss);
		if (onentry.size()>0)	{
			oss<<"<onentry>\n";
			PanelGraf::generarSCXMLSentencias(onentry,oss);
			oss<<"</onentry>\n";
		}
		if (onexit.size()>0)	{
			oss<<"<onexit>\n";
			PanelGraf::generarSCXMLSentencias(onexit,oss);
			oss<<"</onexit>\n";
		}
		for (vector<EstadoPtr>::const_iterator it=estados.begin();it!=estados.end();++it) (*it)->generarSCXML(oss);
		oss<<"</state>\n";
	}	catch (const logic_error &e)	{
		ostringstream errstrm;
		errstrm<<nombre<<": "<<e.what();
		throw logic_error(errstrm.str());
	}
}

void EstadoCompuesto::getCentro(wxPoint &p) const	{
	p=centro;
}

void EstadoCompuesto::getJerarquiaSubestados(const wxPoint &p,vector<DibujablePtr> &vec) const	{
	vec.clear();
	for (vector<TransicionPtr>::const_iterator it=transiciones.begin();it!=transiciones.end();++it) if ((*it)->esPunto(p))	{
		vec.push_back(*it);
		return;
	}
	for (vector<EstadoPtr>::const_iterator it=estados.begin();it!=estados.end();++it)	{
		(*it)->getJerarquiaSubestados(p,vec);
		if (vec.size()!=0&&!IS_DERIVED(vec[0],Transicion))	{
			vec.push_back(getSmartPtr(this));
			return;
		}
	}
	if (esPunto(p)) vec.push_back(getSmartPtr(this));
}

void EstadoCompuesto::getJerarquiaSubestadosSinTransiciones(const wxPoint &p,vector<DibujablePtr> &vec) const	{
	for (vector<EstadoPtr>::const_iterator it=estados.begin();it!=estados.end();++it)	{
		(*it)->getJerarquiaSubestadosSinTransiciones(p,vec);
		if (vec.size()!=0)	{
			vec.push_back(getSmartPtr(this));
			return;
		}
	}
	if (esPunto(p)) vec.push_back(getSmartPtr(this));
}

void EstadoCompuesto::getListaTransiciones(vector<TransicionPtr> &vec) const	{
	vec=transiciones;
}

const string &EstadoCompuesto::getNombre() const	{
	return nombre;
}

void EstadoCompuesto::getOnentry(vector<AccionPtr> &vec) const	{
	vec=onentry;
}

void EstadoCompuesto::getOnexit(vector<AccionPtr> &vec) const	{
	vec=onexit;
}

PanelGraf *EstadoCompuesto::getPanel() const	{
	return padre;
}

int EstadoCompuesto::getSiguienteIDInstruccion()	{
	return ++idinstr;
}

void EstadoCompuesto::getTodasLasSentencias(vector<AccionPtr> &vec) const	{
	vec=onentry;
	vec.insert(vec.end(),onexit.begin(),onexit.end());
	vector<AccionPtr> tmp;
	for (vector<TransicionPtr>::const_iterator it=transiciones.begin();it!=transiciones.end();++it)	{
		(*it)->getListaSentencias(tmp);
		vec.insert(vec.end(),tmp.begin(),tmp.end());
	}
}

void EstadoCompuesto::getTodosLosObjetos(const wxPoint &p,vector<DibujablePtr> &vec) const	{
	vec.clear();
	for (vector<TransicionPtr>::const_iterator it=transiciones.begin();it!=transiciones.end();++it)	if ((*it)->esPunto(p)) vec.push_back(*it);
	vector<DibujablePtr> tmp;
	for (vector<EstadoPtr>::const_iterator it=estados.begin();it!=estados.end();++it)	{
		(*it)->getTodosLosObjetos(p,tmp);
		vec.insert(vec.end(),tmp.begin(),tmp.end());
	}
	if (esPunto(p)) vec.push_back(getSmartPtr(this));
}

int EstadoCompuesto::getTransicionesEnBucle() const	{
	int res=0;
	for (vector<TransicionPtr>::const_iterator it=transiciones.begin();it!=transiciones.end();++it) if ((*it)->esEntrante(getSmartPtr(this))) res++;
	return res;
}

void EstadoCompuesto::mover(double x,double y)	{
	centro.x+=x;
	centro.y+=y;
	for (std::vector<EstadoPtr>::iterator it=estados.begin();it!=estados.end();++it) (*it)->mover(x,y);
	calcularRectangulo();
}

void EstadoCompuesto::moverAbs(double x,double y)	{
	mover(x-centro.x,y-centro.y);
}

void EstadoCompuesto::puntosDeSalida(vector<wxPoint> &vec) const	{
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

void EstadoCompuesto::rectMinimo(wxRect &r) const	{
	r=rect;
}

void EstadoCompuesto::setInicialPorNombre(const string &s)	{
	for (vector<EstadoPtr>::const_iterator it=estados.begin();it!=estados.end();++it) if ((*it)->getNombre()==s)	{
		eInicial=*it;
		return;
	}
	eInicial=EstadoPtr(NULL);
}

void EstadoCompuesto::setNombre(const string &s)	{
	nombre=s;
}

void EstadoCompuesto::setOnentry(const vector<AccionPtr> &vec)	{
	onentry=vec;
}

void EstadoCompuesto::setOnexit(const vector<AccionPtr> &vec)	{
	onexit=vec;
}
