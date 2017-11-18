include_directories(${CLarity_SOURCE_DIR}/include 
                    ${CLarity_SOURCE_DIR}/third_party/googletest/googletest/include)

file(GLOB CLARITY_TEST_CASES "*.cc")
add_executable(clarity_test_suite ${CLARITY_TEST_CASES})

target_link_libraries(clarity_test_suite clarity gtest_main)
