cmake_minimum_required(VERSION 2.8)

project(SWGeneral)
cmake_policy(SET CMP0015 NEW) #supress link warning

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package(PythonLibs REQUIRED)

include_directories(../SWMachineLearning)
include_directories(../SWLib)
include_directories(../SWGraphics)
include_directories(${PYTHON_INCLUDE_DIRS})

set(SOURCES
    ./UltimateMod.cpp
    ./General/Features.cpp
    ./Patterns/Patterns.cpp
    ./Patterns/Creational/Singleton/HeroCamp.cpp
    ./Patterns/Creational/Singleton/HeroCampMultithread.cpp
    ./Patterns/Creational/AbstractFactory/BritishArcher.cpp
    ./Patterns/Creational/AbstractFactory/BritishArmy.cpp
    ./Patterns/Creational/AbstractFactory/BritishCannon.cpp
    ./Patterns/Creational/AbstractFactory/BritishSwordsMan.cpp
    ./Patterns/Creational/AbstractFactory/FrenchArcher.cpp
    ./Patterns/Creational/AbstractFactory/FrenchArmy.cpp
    ./Patterns/Creational/AbstractFactory/FrenchCannon.cpp
    ./Patterns/Creational/AbstractFactory/FrenchSwordsMan.cpp
    ./Tasks/AdventOfCode.cpp
    ./Tasks/MainGeek.cpp
)


link_directories(.)
set(SW_LIBRARIES SWMachineLearning SWLib SWGraphics)

add_executable (sw ${SOURCES})
target_link_libraries(sw ${SW_LIBRARIES} ${PYTHON_LIBRARIES} pthread)


