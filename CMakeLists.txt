cmake_minimum_required(VERSION 3.15.2)

project(midi C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    add_definitions(-DOS_WINDOWS)
    add_definitions(-DPLATFORM_X86_64)
    add_definitions(-D_USE_MATH_DEFINES)
elseif(APPLE)
    add_definitions(-DOS_OSX)
    add_definitions(-DPLATFORM_X86_64)
elseif(UNIX)
    add_definitions(-DOS_LINUX)
    add_definitions(-DPLATFORM_X86_64)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    message("Debug build.")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    message("Release build.")
else()
    message(" ! ! ! Unknown build type.")
endif()

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "src/**.c")

add_library(${PROJECT_NAME} STATIC)

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_sources(${PROJECT_NAME} PRIVATE ${${PROJECT_NAME}_SOURCES})
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-Werror> # GCC/Clang
    $<$<COMPILE_LANGUAGE:C>:-Werror> # GCC/Clang for C files
    $<$<CXX_COMPILER_ID:MSVC>:/WX> # MSVC equivalent
)

# Optionally enable testing
option(ENABLE_TESTING "Enable unit testing" ON)

if(ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

option(ENABLE_MEMORY_LEAK_TESTING "Enable memory leak testing" ON)

if(ENABLE_TESTING AND ENABLE_MEMORY_LEAK_TESTING)
    if(APPLE)
        add_custom_target(memory-leak-testing
            COMMAND leaks --atExit -- ./tests/midi-tests
        )
    endif()
endif()
