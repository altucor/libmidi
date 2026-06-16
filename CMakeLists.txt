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

add_library(${PROJECT_NAME} STATIC)

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 23
    C_STANDARD_REQUIRED ON
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    LINKER_LANGUAGE C
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

    src/events/note.c
    src/events/pitch.c
    src/events/program_change.c
    src/events/control.c
    src/events/channel_pressure.c
    src/events/key_pressure.c

    src/events/meta/tempo.c
    src/events/meta/text.c
    src/events/meta/time_signature.c

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

    include/libmidi/events/note.h
    include/libmidi/events/pitch.h
    include/libmidi/events/program_change.h
    include/libmidi/events/meta.h
    include/libmidi/events/key_pressure.h
    include/libmidi/events/control.h
    include/libmidi/events/channel_pressure.h

    include/libmidi/events/meta/tempo.h
    include/libmidi/events/meta/text.h
    include/libmidi/events/meta/time_signature.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Optionally enable testing
option(ENABLE_TESTING "Enable unit testing" OFF)

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
