cmake_minimum_required(VERSION 2.6)

project(CLarity)

# OpenCL resources
find_package(OpenCL REQUIRED)
set(BUILD_TESTS OFF CACHE BOOL "Build Unit Tests" FORCE)

# Project sources
configure_file(${CLarity_SOURCE_DIR}/include/clarity_config.h.in 
               ${CLarity_SOURCE_DIR}/include/clarity_config.h)
file(GLOB CLARITY_SOURCES "src/*.cc")
add_library(clarity SHARED ${CLARITY_SOURCES})
message(STATUS ${CLarity_SOURCE_DIR})
target_include_directories(clarity PUBLIC 
                           ${CLarity_SOURCE_DIR}/include 
                           ${OpenCL_INCLUDE_DIRS} 
                           ${CLarity_SOURCE_DIR}/third_party/khronos)
target_compile_options(clarity PUBLIC "-std=c++14" "-Werror" "-Wall" "-Wextra" "-Wpedantic")
target_link_libraries(clarity ${OpenCL_LIBRARY})

option(BUILD_UNITTESTS "Builds the unitests for clarity" ON)
option(BUILD_DEMO "Builds the demo for CLarity" ON)

if(BUILD_UNITTESTS)
    enable_testing()

    set(INSTALL_GTEST OFF CACHE BOOL "Install googletest")
    set(INSTALL_GMOCK OFF CACHE BOOL "Install googlemock")
    add_subdirectory(third_party/googletest)
    add_subdirectory(test)

    add_test(NAME CLarity_Tests COMMAND clarity_test_suite)
endif()

if(BUILD_DEMO)
    add_subdirectory(demo)
endif()
