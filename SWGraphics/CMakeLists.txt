cmake_minimum_required(VERSION 2.8)

project(SWGraphics)
include_directories(.)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

list(APPEND SOURCES
    ./Plotting/DrawGraph.cpp
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

if (WIN32) 
    find_package(OpenGL REQUIRED) # OPENGL
    find_package(glfw3 REQUIRED)    
    set(GLFWInclude "C:\\Program Files (x86)\\GLFW\\include\\GLFW\\")
    find_library(GLFW glfw3 PATHS "C:\\Program Files (x86)\\GLFW\\lib")
    if(NOT ${OPENGL_FOUND})
        message("OPENGL not found")
    endif()
    include_directories(${OPENGL_INCLUDE_DIRS} ${GLFWInclude})
    list(APPEND LIBRARIES ${OPENGL_LIBRARIES} ${GLFW})
endif (WIN32)


add_library(SWGraphics SHARED ${SOURCES})
target_link_libraries(SWGraphics ${LIBRARIES})
