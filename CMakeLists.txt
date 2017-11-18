cmake_minimum_required(VERSION 2.6)

project(CLarity)

file(GLOB CLARITY_SOURCES "src/*.cc")

add_library(clarity SHARED ${CLARITY_SOURCES})
target_include_directories(clarity PUBLIC ${CLarity_SOURCE_DIR}/include)
target_compile_options(clarity PUBLIC "-std=c++14" "-Werror" "-Wall" "-Wextra" "-Wpedantic")

option(BUILD_UNITTESTS "Builds the unitests for clarity" ON)

if(BUILD_UNITTESTS)
    enable_testing()

    set(INSTALL_GTEST OFF CACHE BOOL "Install googletest")
    set(INSTALL_GMOCK OFF CACHE BOOL "Install googlemock")
    add_subdirectory(third_party/googletest)
    add_subdirectory(test)

    add_test(NAME CLarity_Tests COMMAND clarity_test_suite)
endif()

