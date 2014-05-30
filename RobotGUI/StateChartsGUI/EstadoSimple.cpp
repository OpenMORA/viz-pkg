#include "EstadoSimple.h"

#include <vector>
#include <stdexcept>
#include <algorithm>

#include "PanelGraf.h"
#include "CDEstadoSimple.h"

using namespace std;
using namespace mrpt::utils;
using namespace InterfazSancho;

IMPLEMENTS_MRPT_OBJECT(EstadoSimple,Estado,InterfazSancho)

void EstadoSimple::activar()	{
	pulsado=true;
}

void EstadoSimple::anadirTransicion(const TransicionPtr &t)	{
	transiciones.push_back(t);
}

void EstadoSimple::cuadroDeDialogo()	{
	CDEstadoSimple dlg(static_cast<EstadoSimplePtr>(Estado::getSmartPtr(this)),getPanel());
}

void EstadoSimple::desactivar()	{
	pulsado=false;
}

void EstadoSimple::desaparecer()	{
	while (transiciones.size()>0) transiciones[0]->desaparecer();
}

void EstadoSimple::dibujar(wxDC &dc)	{
	dc.SetPen(pulsado?borde2P:bordeP);
	dc.SetBrush(pulsado?borde2B:bordeB);
	dc.DrawCircle(centro,RADIO);
	dc.SetPen(interiorP);
	dc.SetBrush(interiorB);
	dc.DrawCircle(centro,RADIOMENOR);
}

void EstadoSimple::dibujarTransiciones(wxDC &dc)	{
	for (vector<TransicionPtr>::iterator it=transiciones.begin();it!=transiciones.end();++it) (*it)->dibujar(dc);
}

void EstadoSimple::eliminarObjeto(DibujablePtr &d)	{
	if (findSP(transiciones,d)!=transiciones.end()) d->desaparecer();
	else	{
		size_t idx=0;
		EstadoPtr e;
		while (idx<transiciones.size())	{
			TransicionPtr t=transiciones[idx];
			t->getEntrante(e);
			if (e.pointer()==d.pointer()) t->desaparecer();
			else idx++;
		}
	}
}

void EstadoSimple::eliminarTransicion(const TransicionPtr &t)	{
	vector<TransicionPtr>::iterator pos=findSP(transiciones,t);
	if (pos!=transiciones.end()) transiciones.erase(pos);
	int enBucle=0;
	for (vector<TransicionPtr>::iterator it=transiciones.begin();it!=transiciones.end();++it) if ((*it)->esEntrante(getSmartPtr(this))) (*it)->setEnBucle(++enBucle);
	Transicion::cleanTransitionList();
}

void EstadoSimple::eliminarTransiciones()	{
	transiciones.clear();
	padre->Refresh();
	Transicion::cleanTransitionList();
}

bool EstadoSimple::esPunto(const wxPoint &p) const	{
	return square(static_cast<double>(p.x-centro.x))+square(static_cast<double>(p.y-centro.y))<=RADIO*RADIO;
}

void EstadoSimple::generarSCXML(ostringstream &oss) const	{
	if (esFinal) oss<<"<final id=\""<<nombre<<"\"/>\n";
	else if (transiciones.empty()&&onentry.empty()&&onexit.empty()) oss<<"<state id=\""<<nombre<<"\"/>\n";
	else	{
		oss<<"<state id=\""<<nombre<<"\">\n";
		try	{
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
		}	catch (logic_error &e)	{
			ostringstream errstrm;
			errstrm<<nombre<<": "<<e.what();
			throw logic_error(errstrm.str());
		}
		oss<<"</state>\n";
	}
}
void EstadoSimple::getCentro(wxPoint &p) const	{
	p=centro;
}

void EstadoSimple::getJerarquiaSubestados(const wxPoint &p,vector<DibujablePtr> &vec) const	{
	vec.clear();
	for (vector<TransicionPtr>::const_iterator it=transiciones.begin();it!=transiciones.end();++it) if ((*it)->esPunto(p))	{
		vec.push_back(*it);
		return;
	}
	vector<DibujablePtr> estados;
	getJerarquiaSubestadosSinTransiciones(p,estados);
	vec.insert(vec.end(),estados.begin(),estados.end());
}

void EstadoSimple::getJerarquiaSubestadosSinTransiciones(const wxPoint &p,vector<DibujablePtr> &vec) const	{
	vec.clear();
	if (esPunto(p)) vec.push_back(getSmartPtr(this));
}

void EstadoSimple::getListaTransiciones(vector<TransicionPtr> &vec) const	{
	vec=transiciones;
}

const string &EstadoSimple::getNombre() const	{
	return nombre;
}

void EstadoSimple::getOnentry(vector<AccionPtr> &vec) const	{
	vec=onentry;
}

void EstadoSimple::getOnexit(vector<AccionPtr> &vec) const	{
	vec=onexit;
}

PanelGraf *EstadoSimple::getPanel() const	{
	return padre;
}

int EstadoSimple::getSiguienteIDInstruccion()	{
	return ++idinstr;
}

void EstadoSimple::getTodasLasSentencias(vector<AccionPtr> &vec) const	{
	vec=onentry;
	vec.insert(vec.end(),onexit.begin(),onexit.end());
	vector<AccionPtr> tmp;
	for (vector<TransicionPtr>::const_iterator it=transiciones.begin();it!=transiciones.end();++it)	{
		(*it)->getListaSentencias(tmp);
		vec.insert(vec.end(),tmp.begin(),tmp.end());
	}
}

void EstadoSimple::getTodosLosObjetos(const wxPoint &p,vector<DibujablePtr> &vec) const	{
	vec.clear();
	for (vector<TransicionPtr>::const_iterator it=transiciones.begin();it!=transiciones.end();++it) if ((*it)->esPunto(p)) vec.push_back(*it);
	if (esPunto(p)) vec.push_back(getSmartPtr(this));
}

int EstadoSimple::getTransicionesEnBucle() const	{
	int res=0;
	for (vector<TransicionPtr>::const_iterator it=transiciones.begin();it!=transiciones.end();++it) if ((*it)->esEntrante(getSmartPtr(this))) res++;
	return res;
}

void EstadoSimple::mover(double x,double y)	{
	pulsado=false;
	centro.x+=x;
	centro.y+=y;
}

void EstadoSimple::moverAbs(double x,double y)	{
	pulsado=false;
	centro.x=x;
	centro.y=y;
}

void EstadoSimple::puntosDeSalida(vector<wxPoint> &vec) const	{
	vec.resize(8);
	for (size_t i=0;i<8;++i)	{
		vec[i].x=centro.x+RADIO*cos((M_PI*i)/4);
		vec[i].y=centro.y+RADIO*sin((M_PI*i)/4);
	}
}

void EstadoSimple::rectMinimo(wxRect &r) const	{
	r.x=centro.x-RADIO;
	r.y=centro.y-RADIO;
	r.width=RADIO+RADIO;
	r.height=RADIO+RADIO;
}

void EstadoSimple::redibujar()	{
	padre->Refresh();
}

void EstadoSimple::setNombre(const string &s)	{
	nombre=s;
}

void EstadoSimple::setOnentry(const vector<AccionPtr> &vec)	{
	onentry=vec;
}

void EstadoSimple::setOnexit(const vector<AccionPtr> &vec)	{
	onexit=vec;
}
