cmake_minimum_required(VERSION 3.0)
project(lttb-cpp-tests)
set(CMAKE_CXX_STANDARD 11)

# Build the test executable
add_executable(testrunner
    test-main.cpp
    test-downsample.cpp
    test-empty.cpp
    test-small.cpp)
target_include_directories(testrunner PUBLIC 
    doctest/doctest
    ../include)

# test data
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/data/source.csv"
    "${CMAKE_CURRENT_BINARY_DIR}/data/source.csv"
    COPYONLY)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/data/sample.csv"
        "${CMAKE_CURRENT_BINARY_DIR}/data/sample.csv"
        COPYONLY)

# Register tests
include(doctest/scripts/cmake/doctest.cmake)
enable_testing()
doctest_discover_tests(testrunner)
