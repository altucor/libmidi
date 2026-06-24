cmake_minimum_required(VERSION 3.15.2)

set(TOP_LEVEL_PROJECT ${PROJECT_NAME})
project(midi-tests C CXX)

include_directories(${CMAKE_SOURCE_DIR}/include)
include(GoogleTest)

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "./src/*.cpp")

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 23
    C_STANDARD_REQUIRED ON
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    LINKER_LANGUAGE C
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE GTest::gtest
    PRIVATE GTest::gtest_main
    PRIVATE ${TOP_LEVEL_PROJECT})

add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)

gtest_discover_tests(${PROJECT_NAME})
