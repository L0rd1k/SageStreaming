cmake_minimum_required(VERSION 3.10.0)

project(SageLib)
include_directories(.)

list(APPEND SOURCES
    ./definitions/local_definitions.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
