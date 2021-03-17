cmake_minimum_required(VERSION 2.8)

project(SW)

option(USE_ML "Machine learning support" ON)
option(USE_LIB "Additional Library support" ON)

if(USE_ML)
    set(USE_ML ON)
endif()
if(USE_LIB)
    set(USE_LIB ON)
endif()

message(STATUS " ---> USE_ML: ${USE_ML}")
message(STATUS " ---> USE_LIB: ${USE_LIB}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

add_subdirectory(SWMachineLearning)
add_subdirectory(SWLib)
add_subdirectory(SWGeneral)
