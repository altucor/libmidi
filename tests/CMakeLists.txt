cmake_minimum_required(VERSION 3.20)

project(midi-tests C CXX)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Release)

IF(MSVC)
    add_definitions(-DOS_WINDOWS)
    add_definitions(-DPLATFORM_X86_64)
    add_definitions(-D_USE_MATH_DEFINES)
ELSEIF(APPLE)
    add_definitions(-DOS_OSX)
    add_definitions(-DPLATFORM_X86_64)
ELSEIF(UNIX)
    add_definitions(-DOS_LINUX)
    add_definitions(-DPLATFORM_X86_64)
ENDIF()

find_package(GTest REQUIRED)
enable_testing()

include_directories("../include")
include(GoogleTest)

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "./src/*.cpp" "../src/**.c")

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

target_link_libraries(${PROJECT_NAME} GTest::gtest_main)

add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)

gtest_discover_tests(${PROJECT_NAME})
