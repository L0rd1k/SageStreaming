cmake_minimum_required(VERSION 3.10.0)

project(SageStream)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

if(WIN32)
    # Add .exe and .dll in the same folder
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out CACHE STRING "")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out CACHE STRING "")

    if(MSVC) # Create .lib files for MSVC compiler on Windows platform.
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
        set(BUILD_SHARED_LIBS TRUE)
    endif(MSVC)
endif(WIN32)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

option(USE_ML "Machine learning support" ON)
option(USE_LIB "Additional Library support" ON)
option(USE_GRAPHICS "Additional OPENGL support" ON)
option(USE_VIDEO "Additional FFMPEG support" ON)
option(USE_GUI "Additional QT support" ON)
option(USE_TEST "Test area" ON)
option(USE_BOOST "USE Boost library support" ON)
option(USE_GLUT "Use GLUT support" OFF)
option(USE_GLFW "Use GLFW support" ON)
option(USE_IMGUI "USE IMGUI support" ON)

if(USE_GLFW)
    add_subdirectory(3rdParty/glad)
    add_subdirectory(3rdParty/glfw)
endif()

if(USE_BOOST)
    add_definitions(-DUSE_BOOST)
endif()

# Create .lib files for MSVC compiler on Windows platform.
if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)
endif()

if(USE_GLUT)
    set(USE_GLUT ON)
    set(USE_GLFW OFF)
    add_definitions(-DUSE_GLUT)
endif()

if(USE_GLFW)
    set(USE_GLFW ON)
    set(USE_GLUT OFF)
    add_definitions(-DUSE_GLFW)
endif()

if(USE_IMGUI)
    set(USE_IMGUI ON)
    add_definitions(-DUSE_IMGUI)
endif()

if(USE_LIB)
    set(USE_LIB ON)
    add_definitions(-DUSE_LIB)
    add_subdirectory(SageLib)
endif()

if(USE_ML)
    set(USE_ML ON)
    add_definitions(-DUSE_ML)
    add_subdirectory(SageML)
endif()

if(USE_GRAPHICS)
    set(USE_GRAPHICS ON)
    add_definitions(-DUSE_GRAPHICS)
    add_subdirectory(SageGraphics)
endif()

if(USE_VIDEO)
    set(USE_VIDEO ON)
    add_definitions(-DUSE_VIDEO)
    add_subdirectory(SageVideo)
endif()

if(USE_GUI)
    set(USE_GUI ON)
    add_definitions(-DUSE_GUI)
    add_subdirectory(SageGUI)
endif()

if(USE_TEST)
    set(USE_TEST ON)
    add_definitions(-DUSE_TEST)
    add_subdirectory(SageTest)
endif()

message(STATUS "    > USE_ML: ${USE_ML}")
message(STATUS "    > USE_LIB: ${USE_LIB}")
message(STATUS "    > USE_GRAPHICS: ${USE_GRAPHICS}")
message(STATUS "    > USE_VIDEO: ${USE_VIDEO}")
message(STATUS "    > USE GUI: ${USE_GUI}")
message(STATUS "    > USE TEST: ${USE_TEST}")
message(STATUS "    > USE GLFW: ${USE_GLFW}")
message(STATUS "    > USE GLUT: ${USE_GLUT}")
message(STATUS "    > USE IMGUI: ${USE_IMGUI}")

configure_file("./data/cameraConf.ini" "./data/cameraConf.ini" COPYONLY)

add_subdirectory(SageCore)
