cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)
project(cstack)

# choose runtime libs for MSVC
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# clone & build Google Test
include(cmake/gtest.cmake)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMOCK_INCLUDE_DIRS})

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

# compilation flags
if (MSVC)
    target_compile_options(cstack PRIVATE /Wall)
else()
    target_compile_options(cstack PRIVATE -Wall -Wextra -Wformat -Wformat-security -Werror)
    set(SANITIZERS_FLAGS -fsanitize=undefined -fsanitize=address)
    target_compile_options(cstack_test PRIVATE ${SANITIZERS_FLAGS})
    target_link_options(cstack_test PRIVATE ${SANITIZERS_FLAGS})
endif()
