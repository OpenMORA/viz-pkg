#ifndef _INTERFAZSANCHO_INSTRUCCION_H
#define _INTERFAZSANCHO_INSTRUCCION_H

#include <string>
#include <vector>
#include <mrpt/utils/CObject.h>

namespace InterfazSancho	{
DEFINE_MRPT_OBJECT_PRE_NO_LINKAGE(Instruccion)
/**
  * This class models a robot instruction. It contains a readable name, an internal name and a comprehensive list of accepted parameters.
  * Note that this class has no constructor.
  * \sa AccionRobot, ParametroInstruccion
  */
class Instruccion:public mrpt::utils::CObject	{
	DEFINE_MRPT_OBJECT(Instruccion)
public:
	std::string nombreVisible;	//!< Readable name.
	std::string nombreReal;	//!< Internal name (will feed the robot's task manager or equivalent processor).
	/**
	  * List of accepted parameter types. Each AccionRobot object which refers to this instruction must have a parameter list matching this one.
	  * For example, if an instruction has a text and a location, the AccionRobot object must have a list with two parameters, one being a text and the other a location.
	  * \sa ParametroInsruccion
	  */
	std::vector<std::string> listaTipos;
	static void parseMultiple(const std::string &,std::vector<InstruccionPtr> &);	//!< Parses a list of possible instructions.
	static void parse(const std::string &,InstruccionPtr &);	//!< Parses a single instruction.
	inline bool operator==(const Instruccion &i) const	{	//!< Equality operator.
		return i.nombreReal==nombreReal;
	}
private:
	static void parseLista(const std::string &,std::vector<std::string> &);	//!< Parse a list of strings.
};

/**
  * This class models a robot instruction parameter. It contains a readable name, an internal name and the type of the parameter.
  * The type of the parameter must match that which the instruction expects.
  * \sa AccionRobot, Instruccion
  */
class ParametroInstruccion	{
public:
	std::string nombreVisible;	//!< Readable name.
	std::string nombreReal;	//!< Internal name.
	std::string tipo;	//!< Type of this parameter (text, location, free string)
	static void getPorTipo(const std::vector<ParametroInstruccion> &,const std::string &,std::vector<ParametroInstruccion> &);	//!< Filters a parameter list by type.
	static void parseMultiple(const std::string &,std::vector<ParametroInstruccion> &);	//!< Parses a list of parameters.
	static void parse(const std::string &,ParametroInstruccion &);	//!< Parses an instruction parameter.
	inline bool operator==(const ParametroInstruccion &p1) const	{	//!< Equivalence operator.
		return (p1.nombreVisible==nombreVisible)&&(p1.nombreReal==nombreReal)&&(p1.tipo==tipo);
	}
};

}	//End of namespace
#endif
