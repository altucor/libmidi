cmake_minimum_required(VERSION 3.15.2)

project(midi C)

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
    message("[libmidi] Debug build.")
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    message("[libmidi] Release build.")
ELSE()
    message(" ! ! ! Unknown build type.")
ENDIF()

add_library(${PROJECT_NAME} STATIC)

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 23
    C_STANDARD_REQUIRED ON
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    LINKER_LANGUAGE C
    CMAKE_COMPILE_WARNING_AS_ERROR ON
)

target_sources(${PROJECT_NAME}
    PUBLIC

    src/buffer.c
    src/event.c
    src/file.c
    src/midi_input_device.c
    src/mthd.c
    src/mtrk.c
    src/util.c
    src/vlv.c

    src/event/note.c
    src/event/pitch.c
    src/event/program_change.c
    src/event/control.c
    src/event/channel_pressure.c
    src/event/key_pressure.c

    src/event/meta/tempo.c
    src/event/meta/text.c
    src/event/meta/time_signature.c

    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    FILES

    include/libmidi/protocol.h
    include/libmidi/errors.h
    include/libmidi/buffer.h
    include/libmidi/util.h
    include/libmidi/vlv.h
    include/libmidi/event.h
    include/libmidi/mtrk.h
    include/libmidi/mthd.h
    include/libmidi/file.h
    include/libmidi/midi_input_device.h

    include/libmidi/event/note.h
    include/libmidi/event/pitch.h
    include/libmidi/event/program_change.h
    include/libmidi/event/meta.h
    include/libmidi/event/key_pressure.h
    include/libmidi/event/control.h
    include/libmidi/event/channel_pressure.h

    include/libmidi/event/meta/tempo.h
    include/libmidi/event/meta/text.h
    include/libmidi/event/meta/time_signature.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Optionally enable testing
option(ENABLE_TESTING "Enable unit testing" OFF)

if(ENABLE_TESTING)
    message(STATUS "[libmidi] Building with unit-tests")
    enable_testing()
    include(FetchContent)

    # Declare the content to fetch (Google Test)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.x
    )

    FetchContent_MakeAvailable(googletest)
    add_subdirectory(tests)
endif()

option(ENABLE_CODE_COVERAGE "Enable code coverage" OFF)

if(ENABLE_CODE_COVERAGE)
    message(STATUS "[libmidi] Building with code coverage")
    add_custom_target(coverage
        COMMAND ./my_tests
        COMMAND lcov --capture --directory . --output-file coverage.info
        COMMAND genhtml coverage.info --output-directory out
    )
endif()

option(ENABLE_TOOLS_BUILD "Enable tools build" OFF)

if(ENABLE_TOOLS_BUILD)
    message(STATUS "[libmidi] Building with tools")
    add_subdirectory(tools)
endif()
