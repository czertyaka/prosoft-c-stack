cmake_minimum_required(VERSION 3.5)
project(cstack)

# clone & build Google Test
include(cmake/gtest.cmake)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMOCK_INCLUDE_DIRS})

# compilation flags
set(COMMON_FLAGS "-Wall -Wextra -Wpedantic -Wformat -Wformat-security -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS} -fsanitize=undefined -fsanitize=address")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 ${COMMON_FLAGS}")

# build library
add_library(cstack STATIC cstack.c)

# build test
enable_testing()
add_executable(cstack_test test.cpp)
target_link_libraries(cstack_test
    cstack
    ${GTEST_LIBRARY}
    ${GTEST_MAIN_LIBRARY}
    ${GMOCK_LIBRARY}
    ${GMOCK_MAIN_LIBRARY}
)
add_test(NAME cstack_test COMMAND cstack_test)
