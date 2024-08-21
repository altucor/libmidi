cmake_minimum_required(VERSION 3.15.2)

project(midi C CXX)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
set(CMAKE_CXX_STANDARD 17)

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

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message("Debug build.")
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    message("Release build.")
ELSE()
    message(" ! ! ! Unknown build type.")
ENDIF()

file(GLOB_RECURSE ${PROJECT_NAME}_HEADERS ${CMAKE_SOURCE_DIR}/src "*.h" "*.hpp")
file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES ${CMAKE_SOURCE_DIR}/src "*.c" "*.cpp")

include_directories("./src")

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)
