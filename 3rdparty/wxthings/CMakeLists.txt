# ----------------------------------------------------------------------------
#  Auxiliary static library: wxThings
#   Used in some apps/* projects.
# ----------------------------------------------------------------------------

PROJECT(wxThings)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(wxWidgets_USE_LIBS base core adv aui)
FIND_PACKAGE(wxWidgets)

IF (wxWidgets_FOUND)
	INCLUDE(${wxWidgets_USE_FILE})

	ADD_LIBRARY(morawxthings
		STATIC
		src/toggle.cpp    # fancy button
		src/LCDWindow.cpp
		)

	#TARGET_LINK_LIBRARIES(${morawxthings} ${wxWidgets_LIBRARIES})
ENDIF (wxWidgets_FOUND)

