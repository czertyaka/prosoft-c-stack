cmake_minimum_required(VERSION 3.15)
project(cstack)
enable_testing()

option(WITH_TEST "Build test (GTest library required)" OFF)

# build library
add_library(cstack STATIC cstack.c)

# build test
if (WITH_TEST)
    enable_testing()
    find_package(GTest REQUIRED)
    add_executable(cstack_test test.cpp)
    target_link_libraries(cstack_test cstack GTest::GTest GTest::Main)
    add_test(NAME cstack_test COMMAND cstack_test)
endif()

# compiler-specific project settings
if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set(MSVC_FLAGS /Wall /WX)
    target_compile_options(cstack PRIVATE ${MSVC_FLAGS})
    target_link_options(cstack PRIVATE ${MSVC_FLAGS})
else()
    target_compile_options(cstack PRIVATE -Wall -Wextra -Wformat -Wformat-security -Werror)
    set(SANITIZERS_FLAGS -fsanitize=undefined -fsanitize=address)
    if (WITH_TEST)
        target_compile_options(cstack_test PRIVATE ${SANITIZERS_FLAGS})
        target_link_options(cstack_test PRIVATE ${SANITIZERS_FLAGS})
    endif()
endif()

