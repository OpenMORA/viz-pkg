#include "Accion.h"

using namespace std;
using namespace mrpt::utils;
using namespace InterfazSancho;

IMPLEMENTS_VIRTUAL_MRPT_OBJECT(Accion,CObject,InterfazSancho)

void Accion::getNombreVisible(string &str) const	{
	//Only AccionRobot will override this method.
	getNombreReal(str);
}

int Accion::getTipoSentencia() const	{
	//Most actions don't have any impact in the program flow, so this is the default.
	return 0;
}

void Accion::getParametro(string &str) const	{
	//The default behaviour is to have zero parameters.
	str="";
}

void Accion::getParametro2(string &str) const	{
	//The default behaviour is to have zero parameters.
	str="";
}
