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

#FILE(GLOB TinyXMLSrc TinyXML/tinystr.cpp TinyXML/tinyxml.cpp TinyXML/tinyxmlerror.cpp TinyXML/tinyxmlparser.cpp)
#FILE(GLOB TinyXMLHdr TinyXML/tinystr.h TinyXML/tinyxml.h)
 
 SOURCE_GROUP("TinyXML Files" FILES ${TinyXMLSrc} ${TinyXMLHdr})


# Declare the target (an executable)
ADD_EXECUTABLE(InterfazSancho WIN32
	PaintBoardElement.cpp	PaintBoardElement.h
	PaintBoardEvent.cpp	PaintBoardEvent.h
	PaintBoardAction.cpp	PaintBoardAction.h
	PaintBoardElementConfigurator.cpp	PaintBoardElementConfigurator.h
	PaintBoardNode.cpp	PaintBoardNode.h
	PaintBoardLink.cpp	PaintBoardLink.h
	PaintBoard.cpp	PaintBoard.h
	tinystr.cpp 	tinystr.h
	tinyxml.cpp	tinyxml.h
	tinyxmlerror.cpp
	tinyxmlparser.cpp
	)
TARGET_LINK_LIBRARIES(InterfazSancho ${MRPT_LIBS} ${wxWidgets_LIBRARIES})
