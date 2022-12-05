cmake_minimum_required(VERSION 3.19.0)

set(THIS test_static_queue)

include_directories(../../SageLib)

add_executable(${THIS})

target_sources(${THIS} PRIVATE
    static_queue_test.cpp
)

target_link_libraries(${THIS} PRIVATE gtest_main SageLib)

add_test(
    NAME ${THIS}
    COMMAND ${THIS}
)