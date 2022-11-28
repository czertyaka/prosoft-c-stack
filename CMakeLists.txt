cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)
project(cstack)
enable_testing()

# choose runtime libs for MSVC
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# clone & build Google Test
include(cmake/gtest.cmake)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMOCK_INCLUDE_DIRS})

# make library and corresponding test for specific implementation
function(add_homework name)

    # set directory with cstack.c variable
    if (${ARGC} LESS 2)
        set(directory ${name})
    else()
        set(directory ${ARGV1})
    endif()

    # build library
    set(CSTACK_LIB "cstack_${name}")
    add_library(${CSTACK_LIB} STATIC ${directory}/cstack.c)
    target_include_directories(${CSTACK_LIB} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

    # build test
    set(TEST_NAME "test_${name}")
    add_executable(${TEST_NAME} test.cpp)
    target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(${TEST_NAME}
        ${CSTACK_LIB}
        ${GTEST_LIBRARY}
        ${GTEST_MAIN_LIBRARY}
        ${GMOCK_LIBRARY}
        ${GMOCK_MAIN_LIBRARY}
    )
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

    # compilation flags
    if (MSVC)
        set(MSVC_FLAGS /Wall /WX)
        target_compile_options(${CSTACK_LIB} PRIVATE ${MSVC_FLAGS})
        target_link_options(${CSTACK_LIB} PRIVATE ${MSVC_FLAGS})
    else()
        target_compile_options(${CSTACK_LIB} PRIVATE -Wall -Wextra -Wformat -Wformat-security -Werror)
        set(SANITIZERS_FLAGS -fsanitize=undefined -fsanitize=address)
        target_compile_options(${TEST_NAME} PRIVATE ${SANITIZERS_FLAGS})
        target_link_options(${TEST_NAME} PRIVATE ${SANITIZERS_FLAGS})
    endif()

endfunction()

add_homework(default .)
add_homework(MyxaWanyuha)

