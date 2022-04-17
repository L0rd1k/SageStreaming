cmake_minimum_required(VERSION 3.10.0)

project(SWGraphics)
include_directories(.)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

list(APPEND SOURCES
    ./plotting/DrawGraph.cpp
)

message("Looking for OpenGL...")

if (UNIX)
    find_package(OpenGL REQUIRED) # OPENGL
    find_package(GLUT REQUIRED)   # GLUT
    find_package(glfw3 3.3 REQUIRED)
    if(NOT ${OPENGL_FOUND})
        message("OPENGL not found")
    endif()
    include_directories( ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} )  
    list(APPEND LIBRARIES ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} glfw)
endif (UNIX)

add_library(SWGraphics SHARED ${SOURCES})
target_link_libraries(SWGraphics ${LIBRARIES})
