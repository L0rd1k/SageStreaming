cmake_minimum_required(VERSION 2.8)

project(SWLib)
include_directories(.)

list(APPEND SOURCES
    ./Utils/Log.cpp
)

add_library(SWLib SHARED ${SOURCES})
