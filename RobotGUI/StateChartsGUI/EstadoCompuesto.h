#ifndef _INTERFAZSANCHO_ESTADOCOMPUESTO_H
#define _INTERFAZSANCHO_ESTADOCOMPUESTO_H

#include "Estado.h"
#include "Transicion.h"
#include <vector>
#include <string>
#include <wx/gdicmn.h>
#include "Instruccion.h"

namespace InterfazSancho	{
class PanelGraf;
class EstadoCompuesto;
DEFINE_MRPT_OBJECT_PRE_CUSTOM_BASE_NO_LINKAGE(EstadoCompuesto,Estado)
/**
  * This class models a compound state, that is, a state which contains a list of substates. Each compound state must have exactly one initial state, and can have any
  * amount of final states. Whenever a "final state" is reached, the state ends and a "<state name>.DONE" event is raised. Transiting between children of the same
  * compound state will not trigger its "on entry" and "on exit" actions, but transiting to a children from outside, or vice versa, will.
  * \sa Dibujable, Estado, PanelGraf
  */
class EstadoCompuesto:public Estado	{
	DEFINE_MRPT_OBJECT(EstadoCompuesto)
private:
	std::vector<EstadoPtr> estados;	//!< List of children states.
	std::string nombre;	//!< State name.
	wxPoint centro;	//!< Central point.
	EstadoPtr eInicial;	//!< Initial state. The SCXML generation will fail if this is null.
	std::vector<AccionPtr> onentry;	//!< Actions to take on state entry.
	std::vector<AccionPtr> onexit;	//!< Actions to take on state exit.
	PanelGraf *padre;	//!< Panel on which this state is going to be drawn.
	wxRect rect;	//!< Rectangle containing the state.
	bool pulsado;	//!< Whether this state is selected.
	std::vector<TransicionPtr> transiciones;	//!< Transitions starting in this state.
	int idinstr;	//!< Private action identifier counter.
	static const int espX=25;	//!< Minimum horizontal space between each children and the state border.
	static const int espY=25;	//!< Minimum vertical space between each children and the state border.
	static const int GROSOR=2;	//!< Border thickness.
public:
	inline EstadoCompuesto(const std::string &n="",int x=0,int y=0,PanelGraf *p=NULL):estados(),nombre(n),centro(x,y),eInicial(),onentry(),onexit(),padre(p),rect(0,0,0,0),pulsado(false),transiciones(),idinstr(0)	{	//!< Constructor.
		calcularRectangulo();
	}
	virtual ~EstadoCompuesto()	{}	//!< Virtual, empty destructor.
	virtual void activar();	//!< Mark this state as selected.
	void anadirSubestado(EstadoPtr &,int,int);	//Adds a substate in the given position.
	virtual void anadirTransicion(const TransicionPtr &);	//!< Adds a transition starting from this state.
	void calcularRectangulo();	//!< Recalculates the minimum rectangle.
	virtual void cuadroDeDialogo();	//!< Calls a dialog box to configurate this state.
	virtual void desactivar();	//!< Mark this state as not selected.
	virtual void desaparecer();	//!< Signals this state, and each of its children, to disappear.
	virtual void dibujar(wxDC &);	//!< Draws this state (but not its transition) into the given drawing context.
	virtual void dibujarTransiciones(wxDC &);	//!< Draws all the transitions starting from this state.
	virtual void eliminarObjeto(DibujablePtr &);	//!< Erases a children (either substate or transition) from this state.
	virtual void eliminarTransicion(const TransicionPtr &);	//!< Deletes a transition.
	virtual void eliminarTransiciones();	//!< Deletes all transitions.
	virtual bool esPunto(const wxPoint &) const;	//!< Checks whether a given point is inside this state.
	virtual void generarSCXML(std::ostringstream &) const;	//!< Generates the SCXML code associated to this state. Will fail if there is no initial state.
	virtual void getCentro(wxPoint &) const;	//!< Returns the central point of the state.
	inline void getInicial(EstadoPtr &e) const	{	//!< Retrieves the initial state of this compound state.
		e=eInicial;
	}
	virtual void getJerarquiaSubestados(const wxPoint &,std::vector<DibujablePtr> &) const;	//!< Returns a list of all objects present in a point, ordered from the deepest to the shallowest.
	virtual void getJerarquiaSubestadosSinTransiciones(const wxPoint &,std::vector<DibujablePtr> &) const;	//!< Returns a list of the substates present in a point, ordered from the deepest to the shallowest.
	virtual void getListaTransiciones(std::vector<TransicionPtr> &) const;	//!< Returns a list with all the transitions departing from this state.
	virtual const std::string &getNombre() const;	//!< Gets the state name.
	virtual void getOnentry(std::vector<AccionPtr> &) const;	//!< Returns the list of actions to take on state entry.
	virtual void getOnexit(std::vector<AccionPtr> &) const;	//!< Returns the list of actions to take on state exit.
	virtual PanelGraf *getPanel() const;	//!< Gets the panel in which this state is to be drawn.
	virtual int getSiguienteIDInstruccion();	//!< Gets un unique ID to identify robot actions.
	inline void getSubestados(std::vector<EstadoPtr> &vec) const	{	//!< Gets the complete list of immediate children (not grandchildren and further descendants).
		vec=estados;
	}
	virtual void getTodasLasSentencias(std::vector<AccionPtr> &) const;	//!< Gets every sentence present in this state, its transition, its descendants, and its descendants' transitions.
	virtual void getTodosLosObjetos(const wxPoint &,std::vector<DibujablePtr> &) const;	//!< Gets a list of every object present in a given point.
	virtual int getTransicionesEnBucle() const;	//!< Gets the amount of transitions departing from this state which already arrive to it.
	virtual void mover(double,double);	//!< Moves this state (and its children) a certain distance.
	virtual void moverAbs(double,double);	//!< Moves this state to an absolute position. Its children will also move.
	virtual void puntosDeSalida(std::vector<wxPoint> &) const;	//!< Gets a list of possible points which a transition can depart from or arrive into.
	virtual void rectMinimo(wxRect &) const;	//!< Returns the minimal rectangle that contains the state and its substates (not the transitions).
	inline void setInicial(const EstadoPtr &e)	{	//!< Sets the initial state. This should be a substate (not necessary direct children) of this state.
		eInicial=e;
	}
	void setInicialPorNombre(const std::string &);	//!< Sets the initial state, which should be a descendant, from its name.
	virtual void setNombre(const std::string &);	//!< Changes the name of this state.
	virtual void setOnentry(const std::vector<AccionPtr> &vec);	//!< Sets the list of actions to take on state entry.
	virtual void setOnexit(const std::vector<AccionPtr> &vec);	//!< Sets the list of actions to take on state exit.
};

}	//End of namespace
#endif
