cmake_minimum_required(VERSION 3.10.0)

project(SWLib)
include_directories(.)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

list(APPEND SOURCES
    ./test/Test.cpp
    ./definitions/LocalDefinitions.cpp
)

add_library(SWLib SHARED ${SOURCES})
