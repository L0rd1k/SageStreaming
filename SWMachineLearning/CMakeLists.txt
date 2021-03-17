cmake_minimum_required(VERSION 2.8)

project(SWMachineLearning)
include_directories(.)
include_directories(../SWLib)

set(LIBRARIES SWLib)

list(APPEND SOURCES
    ML/LinearRegression.cpp
)

add_library(SWMachineLearning SHARED ${SOURCES})
