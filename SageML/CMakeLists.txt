cmake_minimum_required(VERSION 3.10)

project(SageML)

include_directories(.)

if(USE_LIB)
    include_directories(../SageLib)
    set(LIBRARIES SageLib)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

list(APPEND SOURCES
    ML/LinearRegression.cpp
    YOLO/YoloTranslator.cpp
)


add_library(SageML SHARED ${SOURCES})

target_link_libraries(SageML ${LIBRARIES})