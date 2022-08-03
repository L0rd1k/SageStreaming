cmake_minimum_required(VERSION 3.19.0)

set(THIS test_camera)

add_executable(${THIS})

target_sources(${THIS} PRIVATE
    ffmpeg_test.cpp
)

target_link_libraries(${THIS} PRIVATE gtest_main)

add_test(
    NAME ${THIS}
    COMMAND ${THIS}
)