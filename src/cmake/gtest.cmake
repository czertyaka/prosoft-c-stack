find_package(Threads REQUIRED)

include(ExternalProject)

ExternalProject_Add(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.12.0
    UPDATE_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
)

ExternalProject_Get_Property(googletest source_dir)
set(GTEST_INCLUDE_DIRS ${source_dir}/googletest/include)
set(GMOCK_INCLUDE_DIRS ${source_dir}/googlemock/include)

ExternalProject_Get_Property(googletest binary_dir)

function(add_gtest_library library)
    set(LIBRARY_PATH ${binary_dir}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}${library}${CMAKE_STATIC_LIBRARY_SUFFIX})
    add_library(${library} UNKNOWN IMPORTED)
    set_target_properties(
        ${library} PROPERTIES
        "IMPORTED_LOCATION" "${LIBRARY_PATH}"
        "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
    )
    add_dependencies(${library} googletest)
endfunction()

add_gtest_library(gtest)
set(GTEST_LIBRARY gtest)
add_gtest_library(gtest_main)
set(GTEST_MAIN_LIBRARY gtest_main)
add_gtest_library(gmock)
set(GMOCK_LIBRARY gmock)
add_gtest_library(gmock_main)
set(GMOCK_MAIN_LIBRARY gmock_main)

