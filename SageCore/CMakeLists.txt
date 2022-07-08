cmake_minimum_required(VERSION 3.10.0)

project(SageCore)

include_directories(.)

if(USE_LIB)
    include_directories(../SageLib)
    list(APPEND LIBRARIES SageLib)
endif()
if(USE_ML)
    include_directories(../SageML)
    list(APPEND LIBRARIES SageML)
endif()

if(USE_GRAPHICS)
    include_directories(../SageGraphics)
    list(APPEND LIBRARIES SageGraphics)
endif()

if(USE_VIDEO)
    include_directories(../SageVideo)
    list(APPEND LIBRARIES SageVideo)
endif()

set(SOURCES
    ./main.cpp
    ./substance/substance.cpp
    ./core/core.cpp
)

add_executable (${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES} ${PYTHON_LIBRARIES})


