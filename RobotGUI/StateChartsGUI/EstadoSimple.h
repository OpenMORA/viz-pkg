#ifndef _INTERFAZSANCHO_ESTADOSIMPLE_H
#define _INTERFAZSANCHO_ESTADOSIMPLE_H

#include "Estado.h"
#include <string>
#include <wx/gdicmn.h>
#include <vector>
#include "Transicion.h"
#include "Instruccion.h"

namespace InterfazSancho	{
class EstadoSimple;
DEFINE_MRPT_OBJECT_PRE_CUSTOM_BASE_NO_LINKAGE(EstadoSimple,Estado)
class PanelGraf;
/**
  * Simple state. Although it hasn't got the complex structure that a compound or parallel state has, it still mantains the capability of executing actions, waiting
  * for events or switch between destination states according to the holding conditions. It's the atom which higher states are composed from.
  * \sa Dibujable, Estado, EstadoCompuesto, EstadoParalelo, PanelGraf, Transicion
  */
class EstadoSimple:public Estado	{
	DEFINE_MRPT_OBJECT(EstadoSimple)
private:
	std::string nombre;	//!< State name.
	wxPoint centro;	//!< Central point.
	std::vector<TransicionPtr> transiciones;	//!< Transition list.
	std::vector<AccionPtr> onentry;	//!< Actions on entry.
	std::vector<AccionPtr> onexit;	//!< Actions on exit.
	int idinstr;	//!< Unique ID to be given to every newly created action.
	bool pulsado;	//!< Marks whether this state is selected.
	PanelGraf *padre;	//!< Parent panel.
public:
	bool esFinal;	//!< True if this state is final. In that case, the state can't have any action or transition.
private:
	static const int RADIO=18;	//!< Radius of the circle which represents the state.
	static const int RADIOMENOR=16;	//!< Radius of the smaller circle (not counting borders) representing the state.
public:
	EstadoSimple(const std::string &s="",int x=0,int y=0,PanelGraf *p=NULL):nombre(s),centro(x,y),transiciones(),onentry(),onexit(),idinstr(0),pulsado(false),padre(p),esFinal(false)	{}	//!< Constructor.
	virtual ~EstadoSimple()	{}	//!< Virtual destructor.
	virtual void activar();	//!< Mark this state as selected.
	virtual void anadirTransicion(const TransicionPtr &);	//!< Adds a transition to this state's list.
	virtual void cuadroDeDialogo();	//!< Creates a dialog box to configure this state.
	virtual void desactivar();	//!< Marks the state as not selected.
	virtual void desaparecer();	//!< Make this state and its transition disappear.
	virtual void dibujar(wxDC &);	//!< Draw this state in the panel.
	virtual void dibujarTransiciones(wxDC &);	//!< Draw this state's transitions in the panel.
	virtual void eliminarObjeto(DibujablePtr &);	//!< Erases a child object.
	virtual void eliminarTransicion(const TransicionPtr &);	//!< Erases a transition.
	virtual void eliminarTransiciones();	//!< Deletes every transition from the state.
	virtual bool esPunto(const wxPoint &) const;	//!< Checks whether the given point is inside the state.
	virtual void generarSCXML(std::ostringstream &) const;	//!< Writes the SCXML code of this state into the given stream.
	virtual void getCentro(wxPoint &) const;	//!< Returns the center of the state.
	/**
	  * Returns a list of the objects present on this point.
	  * In practice, this function will return either a transition, this state or an empty vector.
	  */
	virtual void getJerarquiaSubestados(const wxPoint &,std::vector<DibujablePtr> &) const;
	/**
	  * Returns a list of the states present on this point.
	  * In practice, this function returns either an empty vector or a vector with only this state.
	  */
	virtual void getJerarquiaSubestadosSinTransiciones(const wxPoint &,std::vector<DibujablePtr> &) const;
	virtual void getListaTransiciones(std::vector<TransicionPtr> &) const;	//!< Gets a list of transitions.
	inline int getMaxX() const	{	//Gets the maximum horizontal coordinate.
		return centro.x+RADIO;
	}
	inline int getMaxY() const	{	//Gets the maximum vertical coordinate.
		return centro.y+RADIO;
	}
	virtual const std::string &getNombre() const;	//!< Gets the name of the state.
	virtual void getOnentry(std::vector<AccionPtr> &vec) const;	//!< Gets the actions to take on state entry.
	virtual void getOnexit(std::vector<AccionPtr> &vec) const;	//!< Gets the actions to take on state exit.
	virtual PanelGraf *getPanel() const;	//!< Returns the parent panel.
	virtual int getSiguienteIDInstruccion();	//!< Returns the next unique instruction ID.
	virtual void getTodasLasSentencias(std::vector<AccionPtr> &) const;	//!< Returns every action present in this state or its transitions.
	virtual void getTodosLosObjetos(const wxPoint &,std::vector<DibujablePtr> &) const;	//!< Returns a list of every object present at a certain point.
	virtual int getTransicionesEnBucle() const;	//!< Returns the amount of transitions departing from this state which also fall into it.
	virtual void mover(double,double);	//!< Displaces the state to a given distance.
	virtual void moverAbs(double x,double y);	//!< Moves the state to a certain point.
	virtual void puntosDeSalida(std::vector<wxPoint> &) const;	//!< Returns a list of possible corners for transitions.
	virtual void rectMinimo(wxRect &) const;	//!< Returns the minimal rectangle containing the state.
	void redibujar();	//!< Refreshes the panel.
	virtual void setNombre(const std::string &);	//!< Changes this state's name.
	virtual void setOnentry(const std::vector<AccionPtr> &vec);	//!< Sets this state's onentry action list.
	virtual void setOnexit(const std::vector<AccionPtr> &vec);	//!< Sets this state's onexit action list.
};
}	//End of namespace
#endif
