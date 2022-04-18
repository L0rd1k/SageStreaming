cmake_minimum_required(VERSION 3.10.0)

project(SWGeneral)

cmake_policy(SET CMP0015 NEW) #supress link warning

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

if(USE_LIB)
    include_directories(../SWLib)
    list(APPEND SW_LIBRARIES SWLib)
endif()
if(USE_ML)
    include_directories(../SWMachineLearning)
    list(APPEND SW_LIBRARIES SWMachineLearning)
endif()

if(USE_GRAPHICS)
    include_directories(../SWGraphics)
    list(APPEND SW_LIBRARIES SWGraphics)
endif()

if(USE_VIDEO)
    include_directories(../SWVideo)
    list(APPEND SW_LIBRARIES SWVideo)
endif()

if(UNIX)
   set(THREAD_LIB pthread) 
   find_package(PythonLibs REQUIRED)
   include_directories(${PYTHON_INCLUDE_DIRS})
endif(UNIX)

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

if(PYTHONLIBS_FOUND)
        message("   > Path to where Python.h is found: ${OpenCV_INCLUDE_DIRS}")
        message("   > Path to the python library: ${PYTHON_LIBRARIES}")
        message("   > Version of the Python libs found: ${PYTHONLIBS_VERSION_STRING}")
        message("   > Path to the debug library (deprecated): ${PYTHON_DEBUG_LIBRARIES}")
endif()

set(SOURCES
    ./UltimateMod.cpp
    ./substance/UMSubstance.cpp
)


if(WIN32)
    find_package( OpenCV REQUIRED )
    include_directories( ${OpenCV_INCLUDE_DIRS})
    link_directories( ${OpenCV_LIB_DIR} )

    message(STATUS "OpenCV library status:")
    message(STATUS "    > config: ${OpenCV_DIR}")
    message(STATUS "    > version: ${OpenCV_VERSION}")
    message(STATUS "    > libraries: ${OpenCV_LIBS}")
    message(STATUS "    > include path: ${OpenCV_INCLUDE_DIRS}")
endif(WIN32)


add_executable (sw ${SOURCES})

target_link_libraries(sw ${SW_LIBRARIES} ${PYTHON_LIBRARIES}  ${THREAD_LIB} ${OpenCV_LIBS})


