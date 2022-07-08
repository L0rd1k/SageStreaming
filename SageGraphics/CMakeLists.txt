cmake_minimum_required(VERSION 3.10.0)

project(SageGraphics)

include_directories(.)
include_directories(../SageLib)
include_directories(../SageVideo)
include_directories(../3rdParty/glfw)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(LIBRARIES SageLib SageVideo glfw)

list(APPEND SOURCES
    ./plotting/DrawGraph.cpp
    ./window/WindowPainterBase.cpp
    ./window/WindowPainterGlut.cpp
    ./image/PicturePainter.cpp
    ./texture/Texture.cpp
)

message("Looking for OpenGL...")
if (UNIX)
    find_package(OpenGL REQUIRED) # OPENGL
    find_package(GLUT REQUIRED)   # GLUT
    if(NOT ${OPENGL_FOUND})
        message("OPENGL not found")
    endif()

    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW lib only")
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW lib only")
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
    set(GLFW_BUILD_INSTALL OFF CACHE BOOL "GLFW lib only")

    include_directories( ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})  
    list(APPEND LIBRARIES ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
endif (UNIX)

add_library(SageGraphics SHARED ${SOURCES})
target_link_libraries(SageGraphics ${LIBRARIES} ${OpenCV_LIBS} glfw)
