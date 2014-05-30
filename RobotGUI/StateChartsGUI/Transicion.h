#ifndef _INTERFAZSANCHO_TRANSICION_H
#define _INTERFAZSANCHO_TRANSICION_H

#include "Dibujable.h"
#include "Instruccion.h"
#include <string>
#include <vector>
#include <wx/gdicmn.h>
#include <utility>
#include <wx/colour.h>
#include <cmath>
#include "Estado.h"
#include <wx/pen.h>
#include <wx/brush.h>
#include <list>

#ifndef M_PI
#define M_PI=acos(-1);
#endif

namespace InterfazSancho	{
DEFINE_MRPT_OBJECT_PRE_CUSTOM_BASE_NO_LINKAGE(Transicion,Dibujable)
/**
  * This class represents a transition between two states. When a transition executes, not only the current state (or hierarchy of states) is changed, but also some
  * code may be executed. Note that a transition can have a null destination state, which means that the transition code is executed but no state change takes place.
  * \sa CDTransicion, Estado, Dibujable, PanelGraf
  */
class Transicion:public Dibujable	{
	DEFINE_MRPT_OBJECT(Transicion)
protected:
	/**
	  * This variable is used as a workaround to simulate the smart pointer use of "this". That is, getting a smart pointer from a normal one.
	  * This artificially adds another reference to each state, so a primitive form of garbage collection must be performed.
	  * \sa addState, cleanStateList, getSmartPtr, Estado::allStates
	  */
	static std::list<TransicionPtr> allTransitions;
private:
	/**
	  * Private class used to store the drawn objects. This is used to determine whether a point falls inside the transition.
	  */
	struct PointOrNumber	{
	private:
		bool isAPoint;
	public:
		double number;	//!< Numeric data.
		wxPoint point;	//!< Point data.
		inline PointOrNumber():isAPoint(false),number(0)	{}	//!< Basic constructor.
		inline PointOrNumber(double d):isAPoint(false),number(d)	{}	//!< Constructor from a number.
		inline PointOrNumber(const wxPoint &p):isAPoint(true),point(p)	{}	//!< Constructor from a point.
		inline bool isPoint() const	{	//!< Returns whether this object is a point (true) or number (false).
			return isAPoint;
		}
	};
	EstadoPtr saliente;	//!< State from which the transition starts. This is necessarily non-null.
	EstadoPtr entrante;	//!< State where the transition ends. This may be null.
	std::string evento;	//!< Event on which the transition starts, if any.
	std::string condicion;	//!< Condition on which the transition starts, if any.
	std::vector<AccionPtr> sentencias;	//!< This transition's action list.
	bool pulsado;	//!< Whether the object is selected.
	/**
	  * List of drawn objects. This list has two types of pairs of objects.
	  * - Two points. In this case, the drawn object is a line.
	  * - A point and a number. The drawn object is the tip of an arrow, centered in the point and oriented with the angle indicated by the number.
	  * This is used to determine whether a point lies inside the transition; in the case of the line, the space considered as lying into the transition is an ellipse.
	  */
	std::vector<std::pair<wxPoint,PointOrNumber> > dibujos;
	int enBucle;	//!< Degree of reflexivity in an auto-transition state.

	static const wxColour negro;	//!< Transition's default colour.
	static const wxColour rojo;	//!< Transition's selected colour.
	static const wxPen lapiz1;	//!< Object used to draw transitions.
	static const wxPen lapiz2;	//!< Object used to draw selected transitions.
	static const wxBrush tri1;	//!< Object used to draw transitions.
	static const wxBrush tri2;	//!< Object used to draw selected transitions.
	static const double anguloFlecha;	//!< Parameter used to draw the tip of the arrow.
	static const double longitudFlecha;	//!< Parameter used to draw the tip of the arrow.
	static const double L;	//!< Parameter used to draw reflexive transitions.
public:
	Transicion(const EstadoPtr &e1=EstadoPtr(),const EstadoPtr &e2=EstadoPtr(),const std::string &ev="",const std::string &con="",const std::vector<AccionPtr> &sen=std::vector<AccionPtr>(0));	//!< Constructor.
	virtual void activar();	//!< Marks the transition as selected.
	inline void anadirSentencia(const AccionPtr &i)	{	//!< Adds a sentence to this transition's list.
		sentencias.push_back(i);
	}
	virtual void cuadroDeDialogo();	//!< Calls a dialog box to configure the transition.
	virtual void desactivar();	//!< Marks the transition as not selected.
	virtual void desaparecer();	//!< Removes this transition from its state.
	virtual void dibujar(wxDC &);	//!< Draws the transition on the drawing context.
private:
	inline static double distancia(const wxPoint &p1,const wxPoint &p2)	{	//!< Gets the distance between two points.
		double dx=p1.x-p2.x;
		double dy=p1.y-p2.y;
		return sqrt(dx*dx+dy*dy);
	}
public:
	inline bool esEntrante(const EstadoPtr &e) const	{	//!< Checks whether the given state is the destination of the transition, taking into account its possible reflexivity.
		return (entrante?entrante.pointer():saliente.pointer())==e.pointer();
	}
	virtual bool esPunto(const wxPoint &) const;	//!< Checks whether this point lies inside the transition.
	inline bool esSaliente(const EstadoPtr &e)	{	//!< Checks whether the given state is the departure of the transition.
		return saliente.pointer()==e.pointer();
	}
	virtual void generarSCXML(std::ostringstream &) const;	//!< Generates the SCXML code associated with this transition.
	inline void getCondicion(std::string &str) const	{	//!< Gets the condition on which the transition happens.
		str=condicion;
	}
	inline void getEntrante(EstadoPtr &e) const	{	//!< Gets the destination state of the transition, even if it's a reflexive one.
		e=entrante?entrante:saliente;
	}
	inline std::string getEntranteString(void) const	{	//!< Gets the name of the destination state, or "" if the transition is reflexive.
		return entrante?(entrante->getNombre()):"";
	}
	inline void getEvento(std::string &str) const	{	//!< Gets the name of the event this transition is waiting for.
		str=evento;
	}
	inline void getListaSentencias(std::vector<AccionPtr> &inss) const	{	//!< Returns the actions list.
		inss=sentencias;
	}
	inline void getSaliente(EstadoPtr &e) const	{	//!< Gets the departure state.
		e=saliente;
	}
	inline void setCondicion(const std::string &str)	{	//!< Gets the condition this transition is waiting for.
		condicion=str;
	}
	inline void setEnBucle(int i)	{	//!< Sets the deepness of the reflexivity.
		enBucle=i;
	}
	inline void setEntrante(const EstadoPtr &e)	{	//!< Sets the destination state (null smart pointer for reflexive transitions).
		entrante=e;
	}
	inline void setEvento(const std::string &str)	{	//!< Sets the name of the event this transition is waiting for.
		evento=str;
	}
	inline void setListaSentencias(const std::vector<AccionPtr> &inss)	{	//!< Sets the actions list.
		sentencias=inss;
	}
	static inline TransicionPtr getSmartPtr(const Transicion *ptr)	{	//!< Given a normal pointer, retrieves the smart "this" pointer.
		for (std::list<TransicionPtr>::iterator it=allTransitions.begin();it!=allTransitions.end();++it) if (it->pointer()==ptr) return *it;
		return TransicionPtr(NULL);
	}
	static inline void addTransition(TransicionPtr &ptr)	{	//!< Adds a transition to the global transitions pool.
		if (findSP(allTransitions,ptr)==allTransitions.end()) allTransitions.push_back(ptr);
	}
	static inline bool removeTransition(TransicionPtr &ptr)	{	//!< Forces a transition to be removed from the global pool.
		std::list<TransicionPtr>::iterator it=findSP(allTransitions,ptr);
		if (it!=allTransitions.end()) {
			allTransitions.erase(it);
			return true;
		} else return false;
	}
	static void cleanTransitionList();	//!< Performs a garbage collection-like procedure to free transitions
};
}	//End of namespace
#endif
