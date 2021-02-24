cmake_minimum_required(VERSION 2.8)
project(UlimateMode)

include_directories(.)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 


#IF(UNIX)
#  set(OpenCV_DIR "/usr/local/opencv_3.4.7/share/OpenCV")
#  find_package(OpenCV REQUIRED)
#  if (NOT ${OPENCV_FOUND})
#     message(SEND_ERROR "Failed to find OpenCV")
#     return()
#ENDIF(UNIX)


set ( SOURCES
UltimateMod.cpp
General/Features.cpp
Patterns/Patterns.cpp
Patterns/Creational/Singleton/HeroCamp.cpp
Patterns/Creational/Singleton/HeroCampMultithread.cpp
Patterns/Creational/AbstractFactory/BritishArcher.cpp
Patterns/Creational/AbstractFactory/BritishArmy.cpp
Patterns/Creational/AbstractFactory/BritishCannon.cpp
Patterns/Creational/AbstractFactory/BritishSwordsMan.cpp
Patterns/Creational/AbstractFactory/FrenchArcher.cpp
Patterns/Creational/AbstractFactory/FrenchArmy.cpp
Patterns/Creational/AbstractFactory/FrenchCannon.cpp
Patterns/Creational/AbstractFactory/FrenchSwordsMan.cpp
Tasks/AdventOfCode.cpp
Tasks/MainGeek.cpp
)

set ( HEADERS
General/Features.h
Patterns/Patterns.h
Creational/Singleton/HeroCamp.h
Creational/Singleton/HeroCampMultithread.h
Creational/AbstractFactory/Archer.h
Creational/AbstractFactory/Army.h
Creational/AbstractFactory/Cannon.h
Creational/AbstractFactory/SwordsMan.h
Creational/AbstractFactory/BritishArcher.h
Creational/AbstractFactory/BritishArmy.h
Creational/AbstractFactory/BritishCannon.h
Creational/AbstractFactory/BritishSwordsMan.h
Creational/AbstractFactory/FrenchArcher.h
Creational/AbstractFactory/FrenchArmy.h
Creational/AbstractFactory/FrenchCannon.h
Creational/AbstractFactory/FrenchSwordsMan.h
)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()
link_directories(.)
add_executable (UlimateMode ${SOURCES})
target_link_libraries(UlimateMode ${QT_LIBRARIES} ${OpenCV_LIBS} pthread)
