cmake_minimum_required(VERSION 3.10)

project(SWTest)

include_directories(.)

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_GetProperties(googletest)
if(NOT googletest)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

set(LIBRARIES pthread)

list(APPEND SOURCES
    ./Threads/Thread.cpp
    # ./General/Features.cpp
    # ./Patterns/Patterns.cpp
    # ./Patterns/Creational/Singleton/HeroCamp.cpp
    # ./Patterns/Creational/Singleton/HeroCampMultithread.cpp
    # ./Patterns/Creational/AbstractFactory/BritishArcher.cpp
    # ./Patterns/Creational/AbstractFactory/BritishArmy.cpp
    # ./Patterns/Creational/AbstractFactory/BritishCannon.cpp
    # ./Patterns/Creational/AbstractFactory/BritishSwordsMan.cpp
    # ./Patterns/Creational/AbstractFactory/FrenchArcher.cpp
    # ./Patterns/Creational/AbstractFactory/FrenchArmy.cpp
    # ./Patterns/Creational/AbstractFactory/FrenchCannon.cpp
    # ./Patterns/Creational/AbstractFactory/FrenchSwordsMan.cpp

)

add_subdirectory(test_camera)

add_library(SWTest SHARED ${SOURCES})

target_link_libraries(SWTest ${LIBRARIES})