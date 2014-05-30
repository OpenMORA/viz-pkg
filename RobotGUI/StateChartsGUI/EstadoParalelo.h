#ifndef _INTERFAZSANCHO_ESTADOPARALELO_H
#define _INTERFAZSANCHO_ESTADOPARALELO_H

#include "Estado.h"
#include "Transicion.h"
#include <vector>
#include <string>
#include <wx/gdicmn.h>
#include "Instruccion.h"
#include <wx/pen.h>
#include <utility>
#include "EstadoCompuesto.h"

namespace InterfazSancho	{
class EstadoParalelo;
DEFINE_MRPT_OBJECT_PRE_CUSTOM_BASE_NO_LINKAGE(EstadoParalelo,Estado)
class PanelGraf;
/**
  * This class models a parallel states, that is, a number of substates which execute concurrently. It's possible that some of them are completely empty, although the
  * state will be considered to be incompletely defined if there aren't at least two defined substates.
  * The states are drawn as an horizontal streak, so a parallel state is vertically as big as the biggest of its children, and horizontally as big as the sum of every
  * children's width.
  * \sa Dibujable, Estado, PanelGraf
  */
class EstadoParalelo:public Estado	{
	DEFINE_MRPT_OBJECT(EstadoParalelo)
private:
	std::vector<EstadoPtr> subestados;	//!< List of substates.
	std::string nombre;	//!< State name.
	wxPoint centro;	//!< Central point.
	wxRect rect;	//!< Minimal rectangle containing the state.
	std::vector<size_t> dims;	//!< List of substate separators, inside the rectangle.
	bool pulsado;	//!< Whether this state is selected.
	std::vector<TransicionPtr> transiciones;	//!< Transitions departing from this state.
	int contador;	//!< Internal counter used to create unique substate identifiers.
	PanelGraf *panel;	//!< Parent panel where this object will be drawn.
	std::vector<AccionPtr> onentry;	//!< List of actions to take on state entry.
	std::vector<AccionPtr> onexit;	//!< List of actions to take on state exit.
	int idinstr;	//!< Internal counter used to create unique action identifiers.
	static const int xMin=30;	//!< Horizontal dimensions of an empty substate (minimal horizontal dimension).
	static const int yMin=60;	//!< Vertical dimensions of an empty substate (minimal vetical dimension).
	static const int GROSOR=2;	//!< Line thickness.
	static const int ESPACIO=5;	//!< Minimal space between a children and the borders of the state.
public:
	inline EstadoParalelo(const std::string &n="",int x=0,int y=0,size_t cuantos=0,PanelGraf *p=NULL):subestados(cuantos),nombre(n),centro(x,y),rect(0,0,0,0),dims(cuantos),pulsado(false),transiciones(),contador(0),panel(p),onentry(),onexit(),idinstr(0)	{	//!< Constructor.
		calcularRectangulo();
	}
	virtual ~EstadoParalelo()	{}	//!< Virtual destructor.
	virtual void activar();	//!< Mark the state as selected.
	void anadirSubestado(EstadoPtr &,int,int);	//!< Add a substate at a given position.
	virtual void anadirTransicion(const TransicionPtr &);	//!< Adds a transiton to this state's transition list.
	void calcularRectangulo();	//!< Recalculates the minimal rectangle.
private:
	void combinar(EstadoPtr e1,EstadoPtr e2,EstadoPtr &com);	//!< Creates a compound state from two children.
public:
	virtual void cuadroDeDialogo();	//!< Calls a dialog box to configure the state.
	virtual void desactivar();	//!< Mark the state as not selected.
	virtual void desaparecer();	//!< Make this state and its children disappear.
	virtual void dibujar(wxDC &);	//!< Draws this state, not its transition, using the given context.
	virtual void dibujarTransiciones(wxDC &);	//!< Draws this state's transitions, and those of its descendants.
	virtual void eliminarObjeto(DibujablePtr &);	//!< Erases a substate or transition.
	virtual void eliminarTransicion(const TransicionPtr &);	//!< Erases a transition.
	virtual void eliminarTransiciones();	//!< Completely empties the list of transitions.
	virtual bool esPunto(const wxPoint &) const;	//!< Returns true if the given point lies inside the state.
	virtual void generarSCXML(std::ostringstream &) const;	//!< Writes the SCXML code of this state into the stream.
	virtual void getCentro(wxPoint &) const;	//!< Returns the centre of this state.
	virtual void getJerarquiaSubestados(const wxPoint &,std::vector<DibujablePtr> &) const;	//!< Gets the list of objects present in this point, starting from the farthest descendant.
	virtual void getJerarquiaSubestadosSinTransiciones(const wxPoint &,std::vector<DibujablePtr> &) const;	//!< Gets the list of objects present in this point, excluding transition.
	virtual void getListaTransiciones(std::vector<TransicionPtr> &) const;	//!< Returns the list of transitions.
	virtual const std::string &getNombre() const;	//!< Gets this state's name.
	virtual void getOnentry(std::vector<AccionPtr> &) const;	//!< Retrieves the list of actions to take on state entry.
	virtual void getOnexit(std::vector<AccionPtr> &) const;	//!< Retrieves the list of actions to take on state exit.
	virtual PanelGraf *getPanel() const;	//!< Gets the panel on which this state is included.
	virtual int getSiguienteIDInstruccion();	//!< Returns the next unique action identifier of this state.
	inline void getSubestados(std::vector<EstadoPtr> &vec) const	{	//!< Gets the list of substates. Note that some of this may be null smart pointers.
		vec=subestados;
	}
	virtual void getTodasLasSentencias(std::vector<AccionPtr> &) const;	//!< Returns every sentence included in this state, including every transition and children.
	virtual void getTodosLosObjetos(const wxPoint &,std::vector<DibujablePtr> &) const;	//!< Returns a complete list of objects present in this point.
	virtual int getTransicionesEnBucle() const;	//!< Returns the amount of transitions departing from this state which also go into this state.
	virtual void mover(double,double);	//!< Moves this state, including children, a certain distance.
	virtual void moverAbs(double,double);	//!< Moves this state into a certain point, dragging its children.
	virtual void puntosDeSalida(std::vector<wxPoint> &) const;	//!< Gets a list of possible starting and ending points for a transition.
	virtual void rectMinimo(wxRect &) const;	//!< Returns the minimal rectangle containing this state.
	void redibujar();	//!< Refreshes the panel.
	/**
	  * Changes the amount of children this state has. If the new amount is smaller than the former, then several states will be destroyed; if it's bigger, some null
	  *	states will be created.
	  */
	void redimensionar(int);
	virtual void setNombre(const std::string &);	//!< Changes the name of the state.
	virtual void setOnentry(const std::vector<AccionPtr> &);	//!< Sets the list of actions to take on state entry
	virtual void setOnexit(const std::vector<AccionPtr> &);	//!< Sets the list of actions to take on state exit.
private:
	static wxSize tamano(const EstadoPtr &);	//!< Static function to determine the exact size of a child state.
};

}	//End of namespace
#endif
