#ifndef _INTERFAZSANCHO_ACCION_H
#define _INTERFAZSANCHO_ACCION_H

#include <string>
#include <sstream>

#include "Instruccion.h"

#include <mrpt/utils/CObject.h>

namespace InterfazSancho	{
DEFINE_MRPT_OBJECT_PRE_NO_LINKAGE(Accion)
/**
  * This abstract class represents an action, i.e., an instruction which an optional set of additional data configuring it. For example, an "if" instruction includes
  * a condition, while a "send" instruction includes an event and a delay. Each instruction has its own fixed set of parameters, and most have no more than two.
  * This class is abstract and cannot be instantiated directly, although it's present in the code in the form of smart pointers.
  * \sa Instruccion
  */
class Accion:public mrpt::utils::CObject	{
	DEFINE_VIRTUAL_MRPT_OBJECT(Accion)
public:
	/**
	  * Returns the "readable" name of the instruction. It is not necessary unique, although it will most probably be. In fact, most instructions have the same
	  * "readable" and "internal" name.
	  * \sa getNombreReal
	  */
	virtual void getNombreVisible(std::string &str) const;
	/**
	  * Returns the "internal" name of the instruction. It should be an unique identifier. In the case of the robot actions, this is the instruction identifier.
	  * \sa getNombreVisible
	  */
	virtual void getNombreReal(std::string &) const=0;
	/**
	  * Writes the SCXML code associated with this instruction in the stream parameter.
	  * This code DOES NOT include the angular braces in which the actual code is inserted. The calling procedure should insert them.
	  */
	virtual void getCodigoSCXML(std::ostringstream &) const=0;
	/**
	  * Returns the sentence "if" type. This is basically an identifier to allow program flow error checks. It may be substituted by a reflexive analysis of the
	  * instruction, although this way typecastings aren't needed.
	  * The possible return values are:
	  * - 0 if this is a "normal" instruction which doesn't alters the program flow.
	  * - 1 if this is an "if" instruction.
	  * - 2 if this is an "elseif" instruction.
	  * - 3 if this is an "else" instruction.
	  * - 4 if this is an "endif" instruction.
	  */
	virtual int getTipoSentencia() const;
	/**
	  * Returns the first parameter, if any.
	  * \sa getParametro2
	  */
	virtual void getParametro(std::string &str) const;
	/**
	  * Returns the second parameter, if any.
	  * \sa getParametro
	  */
	virtual void getParametro2(std::string &str) const;
	/**
	  * Returns the insturction text, as it's intended to appear in an instruction list. For example, the "if" instruction has the form «if (condition)	{».
	  * \sa getNombreReal, getNombreVisible
	  */
	virtual void getCodigoLegible(std::string &) const=0;
	virtual ~Accion()	{};	//!< This class' destructor is virtual, obviously.
	inline void getCodigoSCXML(std::string &str) const	{	//!< Makes use of the homonym method to get the instruction in a standalone string.
		std::ostringstream tmp("");
		getCodigoSCXML(tmp);
		str=tmp.str();
	}
};
}	//End of namespace
#endif
