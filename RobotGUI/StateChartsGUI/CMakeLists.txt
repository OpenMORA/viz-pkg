PROJECT(InterfazSancho)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

FIND_PACKAGE(MRPT REQUIRED slam gui )

FIND_PACKAGE(wxWidgets REQUIRED COMPONENTS "base;core;adv;aui")
INCLUDE(${wxWidgets_USE_FILE})
LINK_DIRECTORIES(${wxWidgets_LIBRARY_DIRS})
IF(MSVC)
	ADD_DEFINITIONS(-DwxUSE_NO_MANIFEST=1)
ENDIF(MSVC)


# Declare the target (an executable)
ADD_EXECUTABLE(InterfazSancho WIN32
	Accion.cpp	Accion.h
	Acciones.cpp	Acciones.h
	CDBorrar.cpp	CDBorrar.h
	CDEstadoCompuesto.cpp	CDEstadoCompuesto.h
	CDEstadoParalelo.cpp	CDEstadoParalelo.h
	CDEstadoSimple.cpp	CDEstadoSimple.h
	CDNuevaVariable.cpp	CDNuevaVariable.h
	CDNuevoEvento.cpp	CDNuevoEvento.h
	CDPedirEstadoDestino.cpp	CDPedirEstadoDestino.h
	CDTransicion.cpp	CDTransicion.h
	ContainerMultiple.cpp	ContainerMultiple.h
	Dibujable.cpp	Dibujable.h
	Estado.cpp	Estado.h
	EstadoCompuesto.cpp	EstadoCompuesto.h
	EstadoParalelo.cpp	EstadoParalelo.h
	EstadoSimple.cpp	EstadoSimple.h
	Instruccion.cpp	Instruccion.h
	InterfazApp.cpp	InterfazApp.h
	InterfazMain.cpp	InterfazMain.h
	Main.cpp	Main.h
	PanelGraf.cpp	PanelGraf.h
	TabInstrRobot.cpp	TabInstrRobot.h
	TabInstrSCXML.cpp	TabInstrSCXML.h
	TabSentencias.cpp	TabSentencias.h
	TabSentenciasSimple.cpp	TabSentenciasSimple.h
	TextoYCombo.cpp	TextoYCombo.h
	Transicion.cpp	Transicion.h
	VentanaP.cpp	VentanaP.h
	)
TARGET_LINK_LIBRARIES(InterfazSancho ${MRPT_LIBS} ${wxWidgets_LIBRARIES})
