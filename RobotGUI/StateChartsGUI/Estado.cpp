#include "Estado.h"
#include <wx/colour.h>

using namespace std;
using namespace mrpt::utils;
using namespace InterfazSancho;

IMPLEMENTS_VIRTUAL_MRPT_OBJECT(Estado,Dibujable,InterfazSancho)

std::list<EstadoPtr> Estado::allStates(0);
const wxColour Estado::bordeC(255,255,0);	//Normal border: yellow.
const wxColour Estado::borde2C(255,0,0);	//Selected border: red.
const wxColour Estado::interiorC(191,191,255);	//Inner region: cyanish.
const wxBrush Estado::bordeB(bordeC);
const wxBrush Estado::borde2B(borde2C);
const wxBrush Estado::interiorB(interiorC);
const wxPen Estado::bordeP(bordeC);
const wxPen Estado::borde2P(borde2C);
const wxPen Estado::interiorP(interiorC);

void Estado::cleanStateList()	{
	size_t N;
	do	{
		N=allStates.size();
		list<list<EstadoPtr>::iterator> toErase(0);
		if (N>0)	{
			for (list<EstadoPtr>::iterator it=allStates.begin();it!=allStates.end();++it) if (findSP(allStates.begin(),it,*it)!=it) toErase.push_back(it);
			for (list<list<EstadoPtr>::iterator>::iterator it=toErase.begin();it!=toErase.end();++it) allStates.erase(*it);
			toErase.clear();
		}
		for (list<EstadoPtr>::iterator it=allStates.begin();it!=allStates.end();++it) if (it->alias_count()==1) toErase.push_back(it);
		for (list<list<EstadoPtr>::iterator>::iterator it=toErase.begin();it!=toErase.end();++it) allStates.erase(*it);
	}	while (N>allStates.size());
}
