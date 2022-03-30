cmake_minimum_required(VERSION 3.10.0)

project(SWGUI)
include_directories(.)

cmake_policy(SET CMP0015 NEW)
cmake_policy(SET CMP0020 NEW)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set (SOURCES
    ./main.cpp
    ./MainWindow.cpp
)

set (MOC_HEADERS
	./MainWindow.h
)

set (UIS
	./MainWindow.ui
)

set (RESOURCES
)

if(UNIX)
	message("Looking for Qt...")
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	set(CMAKE_AUTOUIC ON)
	set(CMAKE_AUTOMOC ON)
	set(CMAKE_AUTORCC ON)
	find_package(Qt5 COMPONENTS Widgets Core REQUIRED )
	message(STATUS "	> QT_VERSION: ${Qt5Widgets_VERSION}")
	message(STATUS "	> QT_LIBRARIES: ${Qt5Widgets_LIBRARIES}")
	list(APPEND QTLIBS Qt5::Core Qt5::Widgets)
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

target_link_libraries(SWGUI ${LIBRARIES} ${QTLIBS})