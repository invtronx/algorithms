cmake_minimum_required(VERSION 3.14)
project(algorithm)

## Configure C++ version and Build/Release settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

## Include GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/master.zip
)
FetchContent_MakeAvailable(googletest)

## Set the executable output directory
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")

## Change the variable SOURCES to compile different files
set(SOURCES tests/String_Matcher.cpp src/KMP.cpp)
add_executable(exec ${SOURCES})

## Register the include directory for target 'exec'
target_include_directories(exec PUBLIC include)

## Link our executable exec with GoogleTest
enable_testing()
target_link_libraries(exec gtest_main)

## Enables CMake's test runner to discover tests in the binary
include(GoogleTest)
gtest_discover_tests(exec)

