#include "Acciones.h"

#include <stdexcept>

using namespace std;
using namespace mrpt::utils;
using namespace InterfazSancho;

inline vector<InstruccionPtr> accionesPorDefecto()	{
	vector<InstruccionPtr> res(5);
	for (size_t i=0;i<5;++i) res[i]=InstruccionPtr(new Instruccion());
	res[0]->nombreVisible="MOVE";
	res[1]->nombreVisible="SAY";
	res[2]->nombreVisible="STOP";
	res[3]->nombreVisible="SAY_TIME";
	res[4]->nombreVisible="SAY_DATE";
	for (size_t i=0;i<5;++i)	{
		res[i]->nombreReal=res[i];
		res[i]->listaTipos.resize(0);
	}
	return res;
}

vector<InstruccionPtr> AccionRobot::accionesDisponibles=accionesPorDefecto();
vector<ParametroInstruccion> AccionRobot::argumentosDisponibles(0);

IMPLEMENTS_MRPT_OBJECT(AccionAssign,Accion,InterfazSancho)
IMPLEMENTS_MRPT_OBJECT(AccionElse,Accion,InterfazSancho)
IMPLEMENTS_MRPT_OBJECT(AccionElseif,Accion,InterfazSancho)
IMPLEMENTS_MRPT_OBJECT(AccionEndif,Accion,InterfazSancho)
IMPLEMENTS_MRPT_OBJECT(AccionEvent,Accion,InterfazSancho)
IMPLEMENTS_MRPT_OBJECT(AccionIf,Accion,InterfazSancho)
IMPLEMENTS_MRPT_OBJECT(AccionSend,Accion,InterfazSancho)
IMPLEMENTS_MRPT_OBJECT(AccionRobot,Accion,InterfazSancho)

void AccionAssign::getNombreReal(string &str) const	{
	str="Assign";
}

void AccionAssign::getCodigoSCXML(ostringstream &oss) const	{
	oss<<"assign location=\""<<nombreVar<<"\" expr=\""<<expr<<'\"';
}

void AccionAssign::getParametro(string &str) const	{
	str=nombreVar;
}

void AccionAssign::getParametro2(string &str) const	{
	str=expr;
}

void AccionAssign::getCodigoLegible(string &str) const	{
	std::ostringstream oss;
	oss<<nombreVar<<'='<<expr<<';';
	str=oss.str();
}

void AccionElse::getNombreReal(string &str) const	{
	str="Else";
}

void AccionElse::getCodigoSCXML(ostringstream &oss) const	{
	oss<<"else";
}

int AccionElse::getTipoSentencia() const	{
	return 3;
}

void AccionElse::getCodigoLegible(string &str) const	{
	str="} else {";
}

void AccionElseif::getNombreReal(string &str) const	{
	str="Elseif";
}

void AccionElseif::getCodigoSCXML(ostringstream &oss) const	{
	oss<<"elseif cond=\""<<cond<<'\"';
}

int AccionElseif::getTipoSentencia() const	{
	return 2;
}

void AccionElseif::getParametro(string &str) const	{
	str=cond;
}

void AccionElseif::getCodigoLegible(string &str) const	{
	std::ostringstream oss;
	oss<<"} elseif ("<<cond<<") {";
	str=oss.str();
}

void AccionEndif::getNombreReal(string &str) const	{
	str="Endif";
}

void AccionEndif::getCodigoSCXML(ostringstream &oss) const	{
	oss<<"/if";
}

int AccionEndif::getTipoSentencia() const	{
	return 4;
}

void AccionEndif::getCodigoLegible(string &str) const	{
	str="}";
}

void AccionEvent::getNombreReal(string &str) const	{
	str="Event";
}

void AccionEvent::getCodigoSCXML(ostringstream &oss) const	{
	oss<<"event name=\""<<event<<'\"';
}

void AccionEvent::getParametro(string &str) const	{
	str=event;
}

void AccionEvent::getCodigoLegible(string &str) const	{
	std::ostringstream oss;
	oss<<"event("<<event<<");";
	str=oss.str();
}

void AccionIf::getNombreReal(string &str) const	{
	str="If";
}

void AccionIf::getCodigoSCXML(ostringstream &oss) const	{
	oss<<"if cond=\""<<cond<<'\"';
}

int AccionIf::getTipoSentencia() const	{
	return 1;
}

void AccionIf::getParametro(string &str) const	{
	str=cond;
}

void AccionIf::getCodigoLegible(string &str) const	{
	std::ostringstream oss;
	oss<<"if ("<<cond<<") {";
	str=oss.str();
}

void AccionSend::getNombreReal(string &str) const	{
	str="Send";
}

void AccionSend::getCodigoSCXML(ostringstream &oss) const	{
	//Change the last string from «'\"'» to «"ms\""» to use milliseconds instead of seconds (not 100% sure of this, though).
	oss<<"send event=\""<<event<<"\" delay=\""<<delay<<'\"';
}

void AccionSend::getParametro(string &str) const	{
	str=event;
}

void AccionSend::getParametro2(string &str) const	{
	std::ostringstream oss;
	oss<<delay;
	str=oss.str();
}

void AccionSend::getCodigoLegible(string &str) const	{
	std::ostringstream oss;
	oss<<"send("<<event<<','<<delay<<");";
	str=oss.str();
}

void AccionRobot::getNombreVisible(string &str) const	{
	str=instruccion->nombreVisible;
}

void AccionRobot::getNombreReal(string &str) const	{
	str=instruccion->nombreReal;
}

void AccionRobot::getCodigoSCXML(ostringstream &oss) const	{
	oss<<"log ";
	if (etiqueta.length()>0) oss<<"label=\""<<etiqueta<<"\" ";
	oss<<"expr=\"\'"<<instruccion->nombreReal;
	for (vector<ParametroInstruccion>::const_iterator it=argumentos.begin();it!=argumentos.end();++it) oss<<' '<<it->nombreReal;
	oss<<"\'\"";
}

void AccionRobot::getParametro(string &str) const	{
	ostringstream oss;
	string aux;
	for (vector<ParametroInstruccion>::const_iterator it=argumentos.begin();it!=argumentos.end();++it)	{
		oss<<it->nombreVisible;
		if (it+1!=argumentos.end()) oss<<' ';
	}
	str=oss.str();
}

void AccionRobot::getParametro2(string &str) const	{
	str=etiqueta;
}

void AccionRobot::getCodigoLegible(string &str) const	{
	ostringstream oss;
	oss<<'['<<etiqueta<<"] ";
	oss<<instruccion->nombreVisible;
	string aux;
	getParametro(aux);
	if (aux.length()>0)	oss<<'('<<aux<<");";
	else oss<<';';
	str=oss.str();
}
