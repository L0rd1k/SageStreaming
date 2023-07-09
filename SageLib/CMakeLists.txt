cmake_minimum_required(VERSION 3.10.0)

project(SageLib)
include_directories(.)


if(USE_BOOST)
    set(Boost_USE_STATIC_LIBS OFF) 
    set(Boost_USE_MULTITHREADED ON)  
    set(Boost_USE_STATIC_RUNTIME OFF) 
    set(BOOST_ROOT "/usr/local/boost-1.74.0")
    set(BOOST_INCLUDE_DIRS "/usr/local/boost-1.74.0/include")
    set(BOOST_LIBRARY_DIRS "/usr/local/boost-1.74.0/lib")

    find_package(Boost 1.74.0 COMPONENTS filesystem system thread) 
    find_package(OpenSSL REQUIRED)

    if(Boost_FOUND)
        include_directories(${Boost_INCLUDE_DIRS})  
    endif()
    set(LIBRARIES ${Boost_LIBRARIES} OpenSSL::SSL)
endif()

list(APPEND SOURCES
    ./definitions/local_definitions.cpp
    ./config/ini_parser.cpp
    ./net/socket/unix_socket.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
