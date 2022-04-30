cmake_minimum_required(VERSION 3.10.0)

project(SWGraphics)
include_directories(.)
include_directories(../SWLib)
include_directories(../SWVideo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(LIBRARIES SWLib SWVideo)

list(APPEND SOURCES
    ./plotting/DrawGraph.cpp
    ./window/WindowPainterBase.cpp
    ./window/WindowPainterGlut.cpp
    ./image/PicturePainter.cpp
    ./texture/Texture.cpp
)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS})
link_directories( ${OpenCV_LIB_DIR} )

message(STATUS "OpenCV library status:")


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
target_link_libraries(SWGraphics ${LIBRARIES}  ${OpenCV_LIBS})
