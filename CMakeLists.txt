cmake_minimum_required(VERSION 3.10.0)

project(SW)

if(WIN32) 
    # Add .exe and .dll in the same folder
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out CACHE STRING "" )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out CACHE STRING "" )
    if(MSVC)  # Create .lib files for MSVC compiler on Windows platform.
       set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
       set(BUILD_SHARED_LIBS TRUE)
    endif(MSVC)
endif(WIN32)

option(USE_ML "Machine learning support" ON)
option(USE_LIB "Additional Library support" ON)
option(USE_GRAPHICS "Additional OPENGL support" ON)
option(USE_VIDEO "Additional FFMPEG support" ON)
option(USE_GUI "Additional QT support" ON)
option(USE_TEST "Test area" ON)

# Create .lib files for MSVC compiler on Windows platform.
if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)
endif()

if(USE_LIB)
    set(USE_LIB ON)
    add_definitions(-DUSE_LIB)    
    add_subdirectory(SWLib)  
endif()

if(USE_ML)
    set(USE_ML ON)
    add_definitions(-DUSE_ML)
    add_subdirectory(SWMachineLearning)
endif()

if(USE_GRAPHICS)
    set(USE_GRAPHICS ON)
    add_definitions(-DUSE_GRAPHICS)
    add_subdirectory(SWGraphics)
endif()

if(USE_VIDEO)
    set(USE_GRAPHICS ON)
    add_definitions(-DUSE_VIDEO)
    add_subdirectory(SWVideo)
endif()

if(USE_GUI)
    set(USE_GRAPHICS ON)
    add_definitions(-DUSE_GUI)
    add_subdirectory(SWGUI)
endif()

if(USE_TEST)
    set(USE_TEST ON)
    add_definitions(-DUSE_TEST)
    add_subdirectory(SWTest)
endif()

message(STATUS "    > USE_ML: ${USE_ML}")
message(STATUS "    > USE_LIB: ${USE_LIB}")
message(STATUS "    > USE_GRAPHICS: ${USE_GRAPHICS}")
message(STATUS "    > USE_VIDEO: ${USE_VIDEO}")
message(STATUS "    > USE GUI: ${USE_GUI}")
message(STATUS "    > USE TEST: ${USE_TEST}")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

add_subdirectory(SWGeneral)
