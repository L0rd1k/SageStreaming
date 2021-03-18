cmake_minimum_required(VERSION 2.8)

project(SWLib)
include_directories(.)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

list(APPEND SOURCES
    ./Utils/Log.cpp
)

add_library(SWLib SHARED ${SOURCES})
