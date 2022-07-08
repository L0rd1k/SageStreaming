cmake_minimum_required(VERSION 3.10)

project(SageTest)

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
  ./General/Features.cpp
)

add_subdirectory(test_camera)

add_library(SageTest SHARED ${SOURCES})

target_link_libraries(SageTest ${LIBRARIES})