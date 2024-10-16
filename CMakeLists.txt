cmake_minimum_required(VERSION 3.15.2)

project(midi C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

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

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "src/**.c")

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Optionally enable testing
option(ENABLE_TESTING "Enable unit testing" ON)

if(ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

if(CODE_COVERAGE)
    add_custom_target(coverage
        COMMAND ./my_tests
        COMMAND lcov --capture --directory . --output-file coverage.info
        COMMAND genhtml coverage.info --output-directory out
    )
endif()
