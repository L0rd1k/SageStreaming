cmake_minimum_required(VERSION 3.10.0)

project(SageGraphics)

if (POLICY CMP0072)
   cmake_policy (SET CMP0072 NEW)
endif(POLICY CMP0072)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories(.)
include_directories(../SageLib)
include_directories(../SageVideo)
include_directories(../3rdParty/glad)
include_directories(../3rdParty/imgui)

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
        list(APPEND LIBRARIES ${GLUT_LIBRARY})
    endif()

    if(USE_GLFW)
        set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW lib only")
        set(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW lib only")
        set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
        set(GLFW_BUILD_INSTALL OFF CACHE BOOL "GLFW lib only")
        list(APPEND LIBRARIES glfw glad)
    endif()
endif(UNIX)

if(USE_IMGUI)
    list(APPEND SOURCES
        ./layer/imgui_layer.cpp
        ./layer/imgui_main_handler.cpp
        ./imgui/imgui_impl_opengl3.cpp
        ./imgui/imgui_impl_opengl3.h
        ./imgui_node/crude_json.cpp
        ./imgui_node/crude_json.h
        ./imgui_node/imgui_bezier_math.h
        ./imgui_node/imgui_bezier_math.inl
        ./imgui_node/imgui_canvas.cpp
        ./imgui_node/imgui_canvas.h
        ./imgui_node/imgui_extra_math.h
        ./imgui_node/imgui_extra_math.inl
        ./imgui_node/imgui_node_editor_api.cpp
        ./imgui_node/imgui_node_editor_internal.h
        ./imgui_node/imgui_node_editor_internal.inl
        ./imgui_node/imgui_node_editor.cpp
        ./imgui_node/imgui_node_editor.h

        )
    list(APPEND LIBRARIES ImGui)
endif()

if(USE_GLUT AND USE_IMGUI)
    list(APPEND SOURCES         
    ./imgui/imgui_impl_glut.cpp
    ./imgui/imgui_impl_glut.h
    )
endif()

if(USE_GLFW AND USE_IMGUI)
    list(APPEND SOURCES         
    ./imgui/imgui_impl_glfw.cpp
    ./imgui/imgui_impl_glfw.h
    )
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES} ${OpenCV_LIBS})
