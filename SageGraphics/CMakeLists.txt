cmake_minimum_required(VERSION 3.10.0)

project(SageGraphics)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories(.)
include_directories(../SageLib)
include_directories(../SageVideo)
include_directories(../3rdParty/glad)
if(USE_GLFW)
    include_directories(../3rdParty/glfw)
endif()

set(LIBRARIES SageLib SageVideo)

list(APPEND SOURCES
    ./plotting/draw_graph.cpp
    ./window/window_painter_base.cpp
    ./image/picture_painter.cpp
    ./texture/texture.cpp
)

if(USE_GLUT)
    list(APPEND SOURCES ./window/window_painter_glut.cpp)
endif()

if(USE_GLFW)
    list(APPEND SOURCES ./window/window_painter_glfw.cpp)
endif()

message("Looking for OpenGL ...")

if(UNIX)
    find_package(OpenGL REQUIRED) # OPENGL
    if(NOT ${OPENGL_FOUND})
        message("OPENGL not found")
    endif()
    include_directories(${OPENGL_INCLUDE_DIRS})
    list(APPEND LIBRARIES ${OPENGL_LIBRARIES})
    if(USE_GLUT)
        find_package(GLUT REQUIRED) # GLUT
        include_directories(${GLUT_INCLUDE_DIRS})
        list(APPEND LIBRARIES ${GLUT_LIBRARY} glad)
    endif()

    if(USE_GLFW)
        set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW lib only")
        set(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW lib only")
        set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
        set(GLFW_BUILD_INSTALL OFF CACHE BOOL "GLFW lib only")
        list(APPEND LIBRARIES glfw glad)
    endif()

endif(UNIX)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES} ${OpenCV_LIBS})
