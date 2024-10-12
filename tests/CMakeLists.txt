cmake_minimum_required(VERSION 3.20)

set(TOP_LEVEL_PROJECT ${PROJECT_NAME})
project(midi-tests C CXX)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Release)

find_package(GTest REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)
include(GoogleTest)

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "./src/*.cpp")

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

target_link_libraries(${PROJECT_NAME}
    PRIVATE GTest::gtest_main
    PRIVATE ${TOP_LEVEL_PROJECT})

add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)
gtest_discover_tests(${PROJECT_NAME})
