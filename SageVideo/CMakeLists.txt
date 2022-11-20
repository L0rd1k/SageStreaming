cmake_minimum_required(VERSION 3.10)

project(SageVideo)
include_directories(.)

if(USE_LIB)
    include_directories(../SageLib)
    set(LIBRARIES SageLib)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

list(APPEND SOURCES
    ./ffmpeg/camera_ffmpeg.cpp
    ./ffmpeg/ffmpeg_utils.cpp
    ./opencv/camera_opencv.cpp

    ./decoder/decoder.cpp
    ./decoder/ffmpeg/ffmpeg_decoder.cpp
    ./decoder/opencv/opencv_decoder.cpp

    ./encoder/encoder.cpp
   
    ./cameras/cameras_handler.cpp
    ./cameras/cameras_creator.cpp

)

if (UNIX)
    message("Looking for FFmpeg...")
    set(FFMPEG_PATH /usr/local/ffmpeg_dev)
    set(FFMPEG_LIB_PATH ${FFMPEG_PATH}/lib)
    find_library(AVCODEC_LIB avcodec PATHS ${FFMPEG_LIB_PATH} NO_DEFAULT_PATH)
    find_library(AVFORMAT_LIB avformat PATHS ${FFMPEG_LIB_PATH} NO_DEFAULT_PATH)
    find_library(AVUTIL_LIB avutil PATHS ${FFMPEG_LIB_PATH} NO_DEFAULT_PATH)
    find_library(SWSCALE_LIB swscale PATHS ${FFMPEG_LIB_PATH} NO_DEFAULT_PATH)
    find_library(AVDEVICE_LIB avdevice PATHS ${FFMPEG_LIB_PATH} NO_DEFAULT_PATH)
        message(STATUS "    > FFMPEG_LIB_PATH: ${FFMPEG_LIB_PATH}")
        message(STATUS "    > AVCODEC_LIB: ${AVCODEC_LIB}")
        message(STATUS "    > AVFORMAT_LIB: ${AVFORMAT_LIB}")
        message(STATUS "    > AVUTIL_LIB: ${AVUTIL_LIB}")
        message(STATUS "    > SWSCALE_LIB: ${SWSCALE_LIB}")
        message(STATUS "    > AVDEVICE_LIB: ${AVDEVICE_LIB}")
    if(NOT AVCODEC_LIB OR NOT AVFORMAT_LIB OR NOT AVUTIL_LIB OR NOT SWSCALE_LIB OR NOT AVDEVICE_LIB)
	    message(FATAL_ERROR "Some of the required ffmpeg components not found")
    endif()
    include_directories(${FFMPEG_PATH}/include)
    link_directories(${FFMPEG_LIB_PATH})
    list(APPEND LIBRARIES ${AVCODEC_LIB} ${AVFORMAT_LIB} ${AVUTIL_LIB} ${SWSCALE_LIB} ${AVDEVICE_LIB})

    if(USE_GSTREAMER)
        find_package(PkgConfig)
        pkg_check_modules(GST REQUIRED 
            gstreamer-1.0>=1.0
            gstreamer-sdp-1.0>=1.0
            gstreamer-pbutils-1.0>=1.0
            gstreamer-video-1.0>=1.0
            gstreamer-app-1.0>=1.0
        )
        include_directories(${GST_INCLUDE_DIRS})
        message(STATUS "Gstreamer library staus:")  
        message(STATUS "    > libraries: ${GST_LIBRARIES}")         
        message(STATUS "    > include path: ${GST_INCLUDE_DIRS}")
        list(APPEND LIBRARIES ${GST_LIBRARIES})

        list(APPEND SOURCES 
        ./gstreamer/gstreamer_capture.cpp 
        ./gstreamer/gstreamer_initializer.cpp)

    endif()

endif(UNIX)

find_package(OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS})
link_directories( ${OpenCV_LIB_DIR} )

message(STATUS "OpenCV library status:")
message(STATUS "    > config: ${OpenCV_DIR}")
message(STATUS "    > version: ${OpenCV_VERSION}")
message(STATUS "    > libraries: ${OpenCV_LIBS}")
message(STATUS "    > include path: ${OpenCV_INCLUDE_DIRS}")

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES} ${OpenCV_LIBS})
