#ifndef _INTERFAZSANCHO_ACCIONES_H
#define _INTERFAZSANCHO_ACCIONES_H

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#include "Instruccion.h"
#include "Accion.h"

namespace InterfazSancho	{
DEFINE_MRPT_OBJECT_PRE_CUSTOM_BASE_NO_LINKAGE(AccionAssign,Accion)
DEFINE_MRPT_OBJECT_PRE_CUSTOM_BASE_NO_LINKAGE(AccionElse,Accion)
DEFINE_MRPT_OBJECT_PRE_CUSTOM_BASE_NO_LINKAGE(AccionElseif,Accion)
DEFINE_MRPT_OBJECT_PRE_CUSTOM_BASE_NO_LINKAGE(AccionEndif,Accion)
DEFINE_MRPT_OBJECT_PRE_CUSTOM_BASE_NO_LINKAGE(AccionEvent,Accion)
DEFINE_MRPT_OBJECT_PRE_CUSTOM_BASE_NO_LINKAGE(AccionIf,Accion)
DEFINE_MRPT_OBJECT_PRE_CUSTOM_BASE_NO_LINKAGE(AccionSend,Accion)
DEFINE_MRPT_OBJECT_PRE_CUSTOM_BASE_NO_LINKAGE(AccionRobot,Accion)

/**
  * This class represents an Assign instruction, that it, an action consisting of the assignment of certain value to a variable.
  */
class AccionAssign:public Accion	{
	DEFINE_MRPT_OBJECT(AccionAssign)
private:
	std::string nombreVar;	//!< The name of the variable which will be overwritten when this sentence is executed.
	std::string expr;	//!< The expression to which the variable is to be assigned.
public:
	inline AccionAssign(const std::string &v="",const std::string &e=""):nombreVar(v),expr(e)	{}	//!< Constructor.
	virtual ~AccionAssign()	{}	//!< Virtual destructor.
	virtual void getNombreReal(std::string &str) const;	//!< Gets the internal name of the instruction, "Assign".
	virtual void getCodigoSCXML(std::ostringstream &oss) const;	//!< Writes the SCXML code of the instruction into the stream.
	virtual void getParametro(std::string &str) const;	//!< Gets the first parameter of the instruction (the variable name).
	virtual void getParametro2(std::string &str) const;	//!< Gets the second parameter of the instruction (the assignment expression).
	virtual void getCodigoLegible(std::string &str) const;	//!< Gets the "readable" code to be written in any instruction list.
};

/**
  * This class represents an Else instruction. Since this class has no parameters, every object is completely equal. The singleton pattern may be used.
  * \sa AccionIf, AccionElseif, AccionEndif
  */
class AccionElse:public Accion	{
	DEFINE_MRPT_OBJECT(AccionElse)
public:
	inline AccionElse()	{}	//!< Constructor.
	virtual ~AccionElse()	{}	//!< Virtual destructor.
	virtual void getNombreReal(std::string &str) const;	//!< Gets the internal name of the instruction, "Else".
	virtual void getCodigoSCXML(std::ostringstream &oss) const;	//!< Gets the SCXML code of the instruction, which is always the string "else".
	virtual int getTipoSentencia() const;	//!< Gets the program flow-related type of the instruction, which is 3.
	virtual void getCodigoLegible(std::string &str) const;	//!< Gets the readable code to be present in the instruction lists.
};

/**
  * This class represents an Elseif instruction. Each object of this class includes a condition.
  * \sa AccionIf, AccionElse, AccionEndif
  */
class AccionElseif:public Accion	{
	DEFINE_MRPT_OBJECT(AccionElseif)
private:
	std::string cond;	//!< Condition.
public:
	inline AccionElseif(const std::string &s=""):cond(s)	{}	//!< Constructor.
	virtual ~AccionElseif()	{}	//!< Virtual destructor.
	virtual void getNombreReal(std::string &str) const;	//!< Gets the internal name of the instruction, "Elseif".
	virtual void getCodigoSCXML(std::ostringstream &oss) const;	//!< Writes the SCXML code of the instruction into the stream.
	virtual int getTipoSentencia() const;	//!< Gets the program flow-related type of the instruction, which is 2.
	virtual void getParametro(std::string &str) const;	//!< Gets the parameter of the instruction, that is, the condition.
	virtual void getCodigoLegible(std::string &str) const;	//!< Retrieves the readable code of this instruction.
};

/**
  * This class represents an Endif instruction, which finishes an If block. Every object of this class is exactly the same, so the singleton pattern may be used.
  * \sa AccionIf, AccionElseif, AccionElse
  */
class AccionEndif:public Accion	{
	DEFINE_MRPT_OBJECT(AccionEndif)
public:
	inline AccionEndif()	{}	//!< Constructor.
	virtual ~AccionEndif()	{}	//!< Virtual destructor.
	virtual void getNombreReal(std::string &str) const;	//!< Gets the internal name of the instruction ("Endif").
	virtual void getCodigoSCXML(std::ostringstream &oss) const;	//!< Writes, into the stream, the SCXML code of the instruction.
	virtual int getTipoSentencia() const;	//!< Gets the program flow-related type of the instruction, which is 4.
	virtual void getCodigoLegible(std::string &str) const;	//!< Gets the readable code of the instruction, which is simply a closing curly brace.
};

/**
  * This class represents an Event instructions, whose only task is to raise an internal event. Currently, there is practically no difference between internal an
  * external events, although this behaviour depends on the SCXML implementation. You may prefer to use this instruction whenever no delay is necessary, since it
  * should propagate a little faster.
  * \sa AccionSend
  */
class AccionEvent:public Accion	{
	DEFINE_MRPT_OBJECT(AccionEvent)
private:
	std::string event;	//!< The event to be raised.
public:
	inline AccionEvent(const std::string &e=""):event(e)	{}	//!< Constructor.
	virtual ~AccionEvent()	{}	//!< Virtual destructor.
	virtual void getNombreReal(std::string &str) const;	//!< Gets the internal name of the instruction, which is "Event".
	virtual void getCodigoSCXML(std::ostringstream &oss) const;	//!< Gets the SCXML code of this instruction.
	virtual void getParametro(std::string &str) const;	//!< Gets the parameter of this instruction, i.e., the event.
	virtual void getCodigoLegible(std::string &str) const;	//!< Gets the readable code, which can be inserted into any instruction list.
};

/**
  * This class represents an If instruction. This instruction starts a conditional block, which can include as many AccionElseif as needed and even an AccionElse.
  * A conditional block must always be finished by an endif instruction. Also, conditional blocks can be freely nested to any desired level of deepness.
  * \sa AccionElseif, AccionElse, AccionEndif
  */
class AccionIf:public Accion	{
	DEFINE_MRPT_OBJECT(AccionIf)
private:
	std::string cond;	//!< Condition.
public:
	inline AccionIf(const std::string &c=""):cond(c)	{}	//!< Constructor.
	virtual ~AccionIf()	{}	//!< Virtual destructor.
	virtual void getNombreReal(std::string &str) const;	//!< Gets the internal name of the instruction, which is "If".
	virtual void getCodigoSCXML(std::ostringstream &oss) const;	//!< Gets the SCXML code of this instruction and writes it into the stream.
	virtual int getTipoSentencia() const;	//!< Gets the program flow-related type of the instruction, which is 1.
	virtual void getParametro(std::string &str) const;	//!< Gets the parameter of this instruction, which is the condition.
	virtual void getCodigoLegible(std::string &str) const;	//!< Gets the readable code, as it will stand on the instruction list.
};

/**
  * This class represents a Send instruction, whose task is to send an external event associated with a delay. Since the current SCXML implementation does only allow
  * a single SCXML machine, the difference between internal and external events is minimal. However, the Event instruction doesn't have a delay, so you may want to
  * resort to this instruction whenever the event mustn't be raised instantly, but after a certain amount of time. However, when no delay is present, internal events
  * may be tangentially faster than external ones, so the use of Event may be justified in many occasions.
  * \sa AccionEvent
  */
class AccionSend:public Accion	{
	DEFINE_MRPT_OBJECT(AccionSend)
private:
	std::string event;	//!< The name of the event to be raised.
	int delay;	//!< The delay. Currently in seconds, although adding "ms" just before the last quote in the "getCodigoSCXML" method would change the unit to milliseconds.
public:
	inline AccionSend(const std::string &e="",int d=0):event(e),delay(d)	{}	//!< Constructor.
	virtual ~AccionSend()	{}	//!< Virtual destructor.
	virtual void getNombreReal(std::string &str) const;	//!< Gets the internal name of the instruction, "Send".
	virtual void getCodigoSCXML(std::ostringstream &oss) const;	//!< Retrieves the SCXML code and inserts it into the stream.
	virtual void getParametro(std::string &str) const;	//!< Gets the first parameter (the name of the event).
	virtual void getParametro2(std::string &str) const;	//!< Gets the second parameter (the delay), as a string.
	virtual void getCodigoLegible(std::string &str) const;	//!< Gets the readable code, ready to be inserted into a list of instructions.
};

/**
  * This class represents an action of the robot, i.e., a non-scxml action which will be interpreted in SCXML as a "log" instruction. Each robot action is determined
  * by a Instruccion object (represented as a smart pointer, since the amount of these instructions will most probably remain unchanged through the execution) and a
  * list of instruction parameters, whose nature is determined by the parameter type list present in the Instruccion object.
  * With little or no changes to this class, but some external manipulation (mostly in the TabInstrRobot code), this class should be ready to be used with free
  * strings, that is, strings entered by the user instead of selected from a list.
  * \sa Instruccion, ParametroInstruccion
  */
class AccionRobot:public Accion	{
	DEFINE_MRPT_OBJECT(AccionRobot)
private:
	static std::vector<InstruccionPtr> accionesDisponibles;	//!< Static list of available instructions.
	static std::vector<ParametroInstruccion> argumentosDisponibles;	//!< Static list of available instruction parameters. Each parameter is associated with a type.
	InstruccionPtr instruccion;	//!< Instruction associated with this action.
	std::vector<ParametroInstruccion> argumentos;	//!< Parameters present in this action. They should correspond, in amount and type, which those expected by the current instruction.
	std::string etiqueta;	//!< Label of the instruction. This, along with identifying it, names the event which happens when the action ends. This may be unique, but it's not necessary.
public:
	static inline void getAccionesDisponibles(std::vector<InstruccionPtr> &d)	{	//!< Retrieves the list of available actions.
		d=accionesDisponibles;
	}
	/**
	  * Sets the list of available actions.
	  * \sa TabInstrRobot::setAcciones
	  */
	static inline void setAccionesDisponibles(const std::vector<InstruccionPtr> &d)	{
		accionesDisponibles=d;
	}
	static inline void getArgumentosDisponibles(std::vector<ParametroInstruccion> &d)	{	//!< Gets the list of available parameters.
		d=argumentosDisponibles;
	}
	/**
	  * Sets the list of available arguments for the actions.
	  * \sa TabInstrRobot::setAcciones
	  */
	static inline void setArgumentosDisponibles(const std::vector<ParametroInstruccion> &d)	{
		argumentosDisponibles=d;
	}
	/**
	  * Constructor.
	  * This constructor DOES NOT check the correctness of the argument list.
	  */
	inline AccionRobot(const InstruccionPtr &i=InstruccionPtr(),const std::vector<ParametroInstruccion> &args=std::vector<ParametroInstruccion>(0),const std::string et=""):instruccion(i),argumentos(args),etiqueta(et)	{}
	virtual ~AccionRobot()	{}	//!< Virtual destructor.
	inline void getInstruccion(InstruccionPtr &i) const	{	//!< Gets the current instruction.
		i=instruccion;
	}
	virtual void getNombreVisible(std::string &str) const;	//!< Gets the readable name of the instruction.
	virtual void getNombreReal(std::string &str) const;	//!< Gets the internal name of the instruction. In the end, this will feed the Task Manager of the robot.
	virtual void getCodigoSCXML(std::ostringstream &oss) const;	//!< Writes the SCXML code (which will be a SCXML "log" instruction) into the stream.
	virtual void getParametro(std::string &str) const;	//!< Gets the first parameter, which is the list of arguments, as a single string with all of them.
	virtual void getParametro2(std::string &str) const;	//!< Gets the second parameter, which is the label of this action.
	/**
	  * Sets the set of arguments.
	  * Note, again, that this doesn't check the parameters against what the instruction expects.
	  */
	inline void getParametros(std::vector<ParametroInstruccion> &p) const	{
		p=argumentos;
	}
	virtual void getCodigoLegible(std::string &str) const;	//!< Gets the readable code of the instruction, as it will appear in the instruction list.
	inline void setEtiqueta(const std::string &str)	{	//!< Changes the label of ths instruction.
		etiqueta=str;
	}
};
}	//End of namespace
#endif
