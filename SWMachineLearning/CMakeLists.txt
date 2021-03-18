cmake_minimum_required(VERSION 2.8)

project(SWMachineLearning)

#cmake_policy(SET CMP0016 NEW) #supress link warning

include_directories(.)
include_directories(../SWLib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(LIBRARIES SWLib)

list(APPEND SOURCES
    ML/LinearRegression.cpp
)

add_library(SWMachineLearning SHARED ${SOURCES})
target_link_libraries(SWMachineLearning ${LIBRARIES})


# find_library(OPENGL_LIB GL)
# find_library(PTHREAD_LIB pthread)
# include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
# list(APPEND LIBRARIES GL glut)
# add_library(SWMachineLearning SHARED ${SOURCES})
# target_link_libraries(SWMachineLearning ${LIBRARIES})