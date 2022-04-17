cmake_minimum_required(VERSION 3.10)

project(SWTest)

include_directories(.)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

list(APPEND SOURCES
    ./Threads/Thread.cpp
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
)


add_library(SWTest SHARED ${SOURCES})

target_link_libraries(SWTest ${LIBRARIES})