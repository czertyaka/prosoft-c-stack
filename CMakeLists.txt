cmake_minimum_required(VERSION 3.5)
project(cstack)

include(ExternalProject)
include(cmake/gtest.cmake)
include_directories(${GTEST_INCLUDE_DIRS})

add_library(cstack STATIC cstack.c)
enable_testing()
add_executable(cstack_test test.cpp)
target_link_libraries(cstack_test cstack ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY})
add_test(NAME cstack_test COMMAND cstack_test)
