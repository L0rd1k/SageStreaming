cmake_minimum_required(VERSION 2.8)

project(SW)

option(USE_ML "Machine learning support" ON)
option(USE_LIB "Additional Library support" ON)
option(USE_GRAPHICS "Additional OPENGL support" ON)

if(USE_LIB)
    set(USE_LIB ON)
    add_subdirectory(SWLib)
    add_definitions(-DUSE_LIB)
endif()

if(USE_ML)
    set(USE_ML ON)
    add_subdirectory(SWMachineLearning)
    add_definitions(-DUSE_ML)
endif()

if(USE_GRAPHICS)
    set(USE_GRAPHICS ON)
    add_subdirectory(SWGraphics)
    add_definitions(-DUSE_GRAPHICS)
endif()

message(STATUS " --- USE_ML: ${USE_ML}")
message(STATUS " --- USE_LIB: ${USE_LIB}")
message(STATUS " --- USE_GRAPHICS: ${USE_GRAPHICS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

add_subdirectory(SWGeneral)
