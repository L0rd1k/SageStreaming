cmake_minimum_required(VERSION 2.8)

project(SWVideo)
include_directories(.)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

list(APPEND SOURCES
    VideoProcessing.cpp
)

if (UNIX)
    message("Looking for FFmpeg...")
    set(FFMPEG_PATH /usr/local/ffmpeg_2.8.1)
    set(FFMPEG_LIB_PATH ${FFMPEG_PATH}/lib)
    find_library(AVCODEC_LIB avcodec PATHS ${FFMPEG_LIB_PATH} NO_DEFAULT_PATH)
    find_library(AVFORMAT_LIB avformat PATHS ${FFMPEG_LIB_PATH} NO_DEFAULT_PATH)
    find_library(AVUTIL_LIB avutil PATHS ${FFMPEG_LIB_PATH} NO_DEFAULT_PATH)
    find_library(SWSCALE_LIB swscale PATHS ${FFMPEG_LIB_PATH} NO_DEFAULT_PATH)
        message(STATUS "    > FFMPEG_LIB_PATH: ${FFMPEG_LIB_PATH}")
        message(STATUS "    > AVCODEC_LIB: ${AVCODEC_LIB}")
        message(STATUS "    > AVFORMAT_LIB: ${AVFORMAT_LIB}")
        message(STATUS "    > AVUTIL_LIB: ${AVUTIL_LIB}")
        message(STATUS "    > SWSCALE_LIB: ${SWSCALE_LIB}")
    if(NOT AVCODEC_LIB OR NOT AVFORMAT_LIB OR NOT AVUTIL_LIB OR NOT SWSCALE_LIB)
	    message(FATAL_ERROR "Some of the required ffmpeg components not found")
    endif()
    include_directories(${FFMPEG_PATH}/include)
    link_directories(${FFMPEG_LIB_PATH})
    list(APPEND LIBRARIES ${AVCODEC_LIB} ${AVFORMAT_LIB} ${AVUTIL_LIB} ${SWSCALE_LIB})
endif(UNIX)

add_library(SWVideo SHARED ${SOURCES})
target_link_libraries(SWVideo ${LIBRARIES})
