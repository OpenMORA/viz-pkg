#include "Transicion.h"
#include "Estado.h"
#include <sstream>
#include <stdexcept>
#include <climits>

#include "PanelGraf.h"
#include "CDTransicion.h"

#include <mrpt/base.h>

using namespace InterfazSancho;
using namespace std;
using namespace mrpt::utils;

const wxColour Transicion::negro(0,0,0);
const wxColour Transicion::rojo(1,0,0);
const wxPen Transicion::lapiz1(negro,1);
const wxPen Transicion::lapiz2(rojo,1);
const wxBrush Transicion::tri1(negro);
const wxBrush Transicion::tri2(rojo);
const double Transicion::anguloFlecha(M_PI/9);
const double Transicion::longitudFlecha(8);
const double Transicion::L(10);

list<TransicionPtr> Transicion::allTransitions(0);

IMPLEMENTS_MRPT_OBJECT(Transicion,Dibujable,InterfazSancho)

Transicion::Transicion(const EstadoPtr &e1,const EstadoPtr &e2,const string &ev,const string &con,const vector<AccionPtr> &sen):saliente(e1),entrante(e2),evento(ev),condicion(con),sentencias(sen),pulsado(false),dibujos(0),enBucle(e1->getTransicionesEnBucle()+1)	{}

void Transicion::activar()	{
	pulsado=true;
}

void Transicion::cuadroDeDialogo()	{
	InterfazSancho::TransicionPtr thisPtr= InterfazSancho::Transicion::getSmartPtr(this);
	CDTransicion dlg(saliente->getPanel(),thisPtr);
}

void Transicion::desactivar()	{
	pulsado=false;
}

void Transicion::desaparecer()	{
	InterfazSancho::TransicionPtr thisPtr= getSmartPtr(this);
	saliente->eliminarTransicion(thisPtr);	
}

void Transicion::dibujar(wxDC &dc)	{
	dc.SetPen(pulsado?lapiz2:lapiz1);
	dc.SetBrush(pulsado?tri2:tri1);
	if (esEntrante(saliente))	{
		//Reflexive transition. Draw a "circular" transition.
		wxRect r;
		saliente->rectMinimo(r);
		double ym=r.y+(r.height)/2,xr=r.x+r.width;
		dibujos.resize(7);
		double aux=L*enBucle;
		wxPoint p[]={wxPoint(xr,ym),wxPoint(xr+aux,ym),wxPoint(xr+aux,r.y-aux),wxPoint(r.x-aux-aux,r.y-aux),wxPoint(r.x-aux-aux,ym),wxPoint(r.x,ym)};
		for (size_t i=0;i<5;++i)	{
			dibujos[i]=make_pair(p[i],PointOrNumber(p[i+1]));
			dc.DrawLine(p[i].x,p[i].y,p[i+1].x,p[i+1].y);
		}
		dibujos[6]=make_pair(p[5],PointOrNumber(M_PI));
		wxPoint triangle[]={p[5],wxPoint(p[5].x-longitudFlecha*cos(anguloFlecha),p[5].y+longitudFlecha*sin(anguloFlecha)),wxPoint(p[5].x-longitudFlecha*cos(anguloFlecha),p[5].y-longitudFlecha*sin(anguloFlecha))};
		dc.DrawPolygon(3,triangle);
	}	else	{
		//Non reflexive transition. Draw a "line-like" transition.
		vector<wxPoint> a1,a2;
		entrante->puntosDeSalida(a1);
		saliente->puntosDeSalida(a2);
		double min=ULLONG_MAX;
		wxPoint p1(a1[0]);
		wxPoint p2(a2[0]);
		for (vector<wxPoint>::const_iterator it=a1.begin();it!=a1.end();++it) for (vector<wxPoint>::const_iterator it2=a2.begin();it2!=a2.end();++it2)	{
			double d=distancia(*it,*it2);
			if (d<min)	{
				p1=*it;
				p2=*it2;
				min=d;
			}
		}
		double alpha=atan2(double(p2.y-p1.y),double(p2.x-p1.x));
		dibujos.resize(2);
		dibujos[0]=make_pair(p1,PointOrNumber(p2));
		dibujos[1]=make_pair(p1,PointOrNumber(alpha));
		dc.DrawLine(p2.x,p2.y,p1.x,p1.y);
		double alpha1=alpha-anguloFlecha,alpha2=alpha+anguloFlecha;
		wxPoint triangle[]={p1,wxPoint(p1.x+longitudFlecha*cos(alpha1),p1.y+longitudFlecha*sin(alpha1)),wxPoint(p1.x+longitudFlecha*cos(alpha2),p1.y+longitudFlecha*sin(alpha2))};
		dc.DrawPolygon(3,triangle);
	}
}

bool Transicion::esPunto(const wxPoint &p) const	{
	for (vector<pair<wxPoint,PointOrNumber> >::const_iterator it=dibujos.begin();it!=dibujos.end();++it) if (!it->second.isPoint())	{
		if (distancia(it->first,p)>longitudFlecha) continue;
		if (fabs(mrpt::math::wrapToPi(atan2(double(p.y-it->first.y),double(p.x-it->first.x))-it->second.number))<=anguloFlecha) return true;
	}	else if ((distancia(p,it->first)+distancia(p,it->second.point))<1.01*distancia(it->first,it->second.point)) return true;
	return false;
}

void Transicion::generarSCXML(ostringstream &oss) const	{
	try	{
		oss<<"<transition";
		if (entrante) oss<<" target=\""<<entrante->getNombre()<<'\"';
		if (evento.length()>0) oss<<" event=\""<<evento<<'\"';
		if (condicion.length()>0) oss<<" cond=\""<<condicion<<'\"';
		if (sentencias.size()>0)	{
			oss<<">\n";
			PanelGraf::generarSCXMLSentencias(sentencias,oss);
			oss<<"</transition>";
		}	else oss<<"/>\n";
	}	catch (const logic_error &l)	{
		ostringstream errstrm;
		errstrm<<"Transición: "<<l.what();
		throw logic_error(errstrm.str());
	}
}

void Transicion::cleanTransitionList()	{
	list<list<TransicionPtr>::iterator> toErase(0);
	if (allTransitions.size()>0)	{
		for (list<TransicionPtr>::iterator it=allTransitions.begin();it!=allTransitions.end();++it) if (findSP(allTransitions.begin(),it,*it)!=it) toErase.push_back(it);
		for (list<list<TransicionPtr>::iterator>::iterator it=toErase.begin();it!=toErase.end();++it) allTransitions.erase(*it);
		toErase.clear();
	}
	for (list<TransicionPtr>::iterator it=allTransitions.begin();it!=allTransitions.end();++it) if (it->alias_count()==1) toErase.push_back(it);
	for (list<list<TransicionPtr>::iterator>::iterator it=toErase.begin();it!=toErase.end();++it) allTransitions.erase(*it);
}
