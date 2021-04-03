cmake_minimum_required(VERSION 2.8)

project(SWGUI)
include_directories(.)

cmake_policy(SET CMP0015 NEW)
cmake_policy(SET CMP0020 NEW)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set (SOURCES
    main.cpp
    mainwin.cpp
)

set (MOC_HEADERS
    mainwin.h
)

set (UIS
    mainwin.ui
)

set (RESOURCES

)

if(UNIX)
	message("Looking for Qt...")
	find_package( Qt4 COMPONENTS QtMain QtCore QtGui QtNetwork REQUIRED) 
	include(${QT_USE_FILE})
	add_definitions(${QT_DEFINITIONS})
	include_directories(${CMAKE_BINARY_DIR})
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	QT4_ADD_RESOURCES(RES_SOURCES ${RESOURCES} )
	QT4_WRAP_UI(UI_HEADERS ${UIS} )
	QT4_WRAP_CPP(MOC_SRCS ${MOC_HEADERS} )
	get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
endif(UNIX)

if(WIN32)
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	set(CMAKE_AUTOMOC ON)
	set(CMAKE_AUTOUIC ON)
	message("Looking for Qt...")
	find_package(Qt5 COMPONENTS Widgets Core Qml REQUIRED)
	add_definitions(${QT_DEFINITIONS})
	include_directories(${CMAKE_BINARY_DIR})
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	message(" > ${Qt5Widgets_VERSION}")
	list(APPEND QTLIBS Qt5::Core Qt5::Widgets)
	get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
endif(WIN32)


link_directories(.)
add_executable (SWGUI ${SOURCES} ${MOC_SRCS} ${RES_SOURCES} ${UI_HEADERS})

# add_library(SWGUI SHARED ${SOURCES})
target_link_libraries(SWGUI ${LIBRARIES} ${QTLIBS})