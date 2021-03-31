cmake_minimum_required(VERSION 2.8)

project(SWGeneral)

cmake_policy(SET CMP0015 NEW) #supress link warning

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

if(USE_LIB)
    include_directories(../SWLib)
    set(SW_LIBRARIES SWLib)
endif()
if(USE_ML)
    include_directories(../SWMachineLearning)
    set(SW_LIBRARIES SWMachineLearning)
endif()

if(USE_GRAPHICS)
    include_directories(../SWGraphics)
    set(SW_LIBRARIES SWGraphics)
endif()

if(USE_VIDEO)
    include_directories(../SWVideo)
    set(SW_LIBRARIES SWVideo)
endif()

if(UNIX)
   set(THREAD_LIB pthread) 
   find_package(PythonLibs REQUIRED)
   include_directories(${PYTHON_INCLUDE_DIRS})
endif(UNIX)

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

#set(PYTHON_LIBRARIES "//usr//lib//python3.7")
#set(PYTHON_INCLUDE_DIR "//usr//include//python3.7")

if(PYTHONLIBS_FOUND)
	message("	> Path to where Python.h is found: ${OpenCV_INCLUDE_DIRS}")
	message("	> Path to the python library: ${PYTHON_LIBRARIES}")
	message("	> Version of the Python libs found: ${PYTHONLIBS_VERSION_STRING}")
	message("	> Path to the debug library (deprecated): ${PYTHON_DEBUG_LIBRARIES}")     
endif()

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
    ./Tasks/AdventOfCode/AdventOfCode.cpp
    ./Tasks/AdventOfCode/D1ReportRepair.cpp
    ./Tasks/AdventOfCode/D2PasswordPhilosophy.cpp
    ./Tasks/AdventOfCode/D3TobogganTrajectory.cpp
    ./Tasks/AdventOfCode/D4PassportProcessing.cpp
    ./Tasks/AdventOfCode/D5BinaryBoarding.cpp
    ./Tasks/AdventOfCode/D6CustomCustoms.cpp
    ./Tasks/AdventOfCode/D7HandyHaversacks.cpp
    ./Tasks/GeekForGeeks/MainGeek.cpp
    ./Tasks/LeetCode/LeetCode.cpp
    ./Tasks/LeetCode/TwoSum.cpp
    ./Tasks/LeetCode/AddTwoNumbers.cpp
    ./Tasks/LeetCode/RemoveDuplicates.cpp
    ./Tasks/LeetCode/RemoveElement.cpp
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

#link_directories(.)

add_executable (sw ${SOURCES})

target_link_libraries(sw ${SW_LIBRARIES} ${PYTHON_LIBRARIES}  ${THREAD_LIB} ${OpenCV_LIBS})


