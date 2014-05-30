#ifndef _INTERFAZSANCHO_ESTADO_H
#define _INTERFAZSANCHO_ESTADO_H

#include "Dibujable.h"
#include <wx/dc.h>
#include <wx/gdicmn.h>
#include <vector>
#include <string>
#include "Accion.h"
#include <wx/brush.h>
#include <list>

namespace InterfazSancho	{
DEFINE_MRPT_OBJECT_PRE_CUSTOM_BASE_NO_LINKAGE(Estado,Dibujable)
struct TransicionPtr;	//Forward declaration
class PanelGraf;	//Forward declaration
/**
  *	This virtual class provides the basic functions that every state (either simple, compound or parallel) must provide. It inherits from Dibujable.
  * \sa Dibujable, EstadoSimple, EstadoCompuesto, EstadoParalelo
  */
class Estado:public Dibujable	{
	DEFINE_VIRTUAL_MRPT_OBJECT(Estado)
protected:
	/**
	  * This variable is used as a workaround to simulate the smart pointer use of "this". That is, getting a smart pointer from a normal one.
	  * This artificially adds another reference to each state, so a primitive form of garbage collection must be performed.
	  * \sa addState, cleanStateList, getSmartPtr, Transicion::allTransitions
	  */
	static std::list<EstadoPtr> allStates;
public:
	virtual void anadirTransicion(const TransicionPtr &)=0;	//!< Adds a transition to this state. Each state should have a list of every trnsition which starts from it.
	virtual void dibujarTransiciones(wxDC &)=0;	//!< Given a drawing context, draws all the transitions starting in this state.
	virtual void eliminarTransicion(const TransicionPtr &)=0;	//!< Erases this transition from the state.
	virtual void eliminarTransiciones()=0;	//!< Erases every transition.
	virtual void eliminarObjeto(DibujablePtr &)=0;	//!< Erases some child (either transition or substate) from this.
	virtual void getCentro(wxPoint &) const=0;	//!< Gets the centre of the state.
	/**
	  * Gets every child present at the given point, starting from the deepest one. Includes this object.
	  * \sa getJerarquiaSubestadosSinTransitiones, esPunto
	  */
	virtual void getJerarquiaSubestados(const wxPoint &,std::vector<DibujablePtr> &) const=0;
	/**
	  * Gets every child (not transition) present at the given point, including this state.
	  * \sa getJerarquiaSubestados, esPunto
	  */
	virtual void getJerarquiaSubestadosSinTransiciones(const wxPoint &,std::vector<DibujablePtr> &) const=0;
	virtual void getListaTransiciones(std::vector<TransicionPtr> &) const=0;	//!< Retrieves a list with all the transitions.
	virtual const std::string &getNombre() const=0;	//!< Gets the state name.
	virtual void getOnentry(std::vector<AccionPtr> &) const=0;	//!< Gets a list of the actions to take on state entry.
	virtual void getOnexit(std::vector<AccionPtr> &) const=0;	//!< Gets a list of the actions to take on state exit.
	/**
	  * Gets the parent panel in which this state is to be drawn.
	  * \sa PanelGraf
	  */
	virtual PanelGraf *getPanel() const=0;
	virtual int getSiguienteIDInstruccion()=0;	//!< Gets an action identifier counter, private to this state. This is used to automatically give default identifiers to the actions.
	virtual void getTodasLasSentencias(std::vector<AccionPtr> &) const=0;	//!< Gets a list of every sentence present in this state or its children.
	virtual void getTodosLosObjetos(const wxPoint &,std::vector<DibujablePtr> &) const=0;	//!< Gets a complete list of the objects present at this point.
	virtual int getTransicionesEnBucle() const=0;	//!< Gets the amount of self-transitions this object has.
	virtual void mover(double x,double y)=0;	//!< Moves the state a given distance.
	virtual void moverAbs(double x,double y)=0;	//!< Moves the state to an absolute position.
	virtual void puntosDeSalida(std::vector<wxPoint> &) const=0;	//!< Retrieves a list of points from which a transition can enter or exit.
	virtual void rectMinimo(wxRect &) const=0;	//!< Returns the minimum rectangle containing the state (not the transitions).
	virtual void setNombre(const std::string &)=0;	//!< Changes the state name.
	virtual void setOnentry(const std::vector<AccionPtr> &)=0;	//!< Sets the list of actions to take on state enrty.
	virtual void setOnexit(const std::vector<AccionPtr> &)=0;	//!< Sets the list of actions to take on state exit.
	static inline EstadoPtr getSmartPtr(const Estado *ptr)	{	//!< Given a basic pointer, returns the smart pointer (if any) holding it.
		for (std::list<EstadoPtr>::iterator it=allStates.begin();it!=allStates.end();++it) if (it->pointer()==ptr) return *it;
		return EstadoPtr(NULL);
	}
	/**
	  * Adds a state to the "global state pool" used to get smart pointers from basic ones.
	  * This function MUST be called every time a new state is created. Otherwise, the correct functioning is not guaranteed.
	  */
	static inline void addState(const EstadoPtr &ptr)	{
		if (findSP(allStates,ptr)==allStates.end()) allStates.push_back(ptr);
	}
	static inline bool removeState(const EstadoPtr &ptr)	{	//!< Forces a state to be removed from the "global state pool".
		std::list<EstadoPtr>::iterator it=findSP(allStates,ptr);
		if (it!=allStates.end()) {
			allStates.erase(it);
			return true;
		} else return false;
	}
	static void cleanStateList();	//!< Garbage collection substitute.
protected:
	static const wxColour bordeC;	//!< Normal border colour for every state.
	static const wxColour borde2C;	//!< Border colour to show when each state is selected.
	static const wxColour interiorC;	//!< Colour used to fill the inner region of every state.
	static const wxBrush bordeB;	//!< Object used to paint state borders.
	static const wxBrush borde2B;	//!< Object used to paint selected state borders.
	static const wxBrush interiorB;	//!< Object used to paint states.
	static const wxPen bordeP;	//!< Object used to paint state borders.
	static const wxPen borde2P;	//!< Object used to paint selected state borders.
	static const wxPen interiorP;	//!< Object used to paint states.
};
}	//End of namespace
#endif
