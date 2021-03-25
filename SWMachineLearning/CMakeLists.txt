cmake_minimum_required(VERSION 2.8)

project(SWMachineLearning)

#cmake_policy(SET CMP0016 NEW) #supress link warning

include_directories(.)

if(USE_LIB)
    include_directories(../SWLib)
    set(LIBRARIES SWLib)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

list(APPEND SOURCES
    ML/LinearRegression.cpp
    YOLO/YoloTranslator.cpp
)


add_library(SWMachineLearning SHARED ${SOURCES})

target_link_libraries(SWMachineLearning ${LIBRARIES})