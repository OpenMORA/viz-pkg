PROJECT(RobotGUI)

# Use MRPT:
FIND_PACKAGE( MRPT REQUIRED slam gui  hwdrivers)

INCLUDE_DIRECTORIES( ${MOOS_INCLUDE_DIRS}  )
LINK_DIRECTORIES( ${MOOS_LIBRARY_DIRS} )

INCLUDE_DIRECTORIES("${viz_pkg_SOURCE_DIR}/3rdparty/wxthings/")


SET(wxWidgets_USE_LIBS base core gl adv aui )
FIND_PACKAGE(wxWidgets REQUIRED)
INCLUDE(${wxWidgets_USE_FILE})

FILE(GLOB StatechartsEditSrc StateChartsEdit/*.cpp)
FILE(GLOB StatechartsEditHdr StateChartsEdit/*.h)
 
SOURCE_GROUP("StatechartsEdit Files" FILES ${StatechartsEditSrc} ${StatechartsEditHdr})
SOURCE_GROUP("VirtualWorld Files" FILES CVirtualWorld.cpp CVirtualWorld.hpp)
 
 
# Add executable
ADD_EXECUTABLE( ${PROJECT_NAME} WIN32
	GUIMOOSApp.cpp GUIMOOSApp.h
	MyGLCanvas.cpp MyGLCanvas.h
	RobotGUI2009_guidesignApp.cpp RobotGUI2009_guidesignApp.h
	RobotGUI2009_guidesignMain.cpp RobotGUI2009_guidesignMain.h
	process_new_vars.cpp
	subscribe_vars.cpp
	CVirtualWorld.cpp
	CVirtualWorld.hpp
	${StatechartsEditSrc}
	${StatechartsEditHdr}
	)

# Link the executable
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${MOOS_LIBRARIES} ${MRPT_LIBS} ${wxWidgets_LIBRARIES})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} morawxthings)
ADD_DEPENDENCIES(${PROJECT_NAME} morawxthings)

