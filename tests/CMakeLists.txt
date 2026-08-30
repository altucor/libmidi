cmake_minimum_required(VERSION 3.15.2)

set(TOP_LEVEL_PROJECT ${PROJECT_NAME})
project(midi-tests C CXX)

include_directories(${CMAKE_SOURCE_DIR}/include)
include(GoogleTest)

add_executable(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 23
    C_STANDARD_REQUIRED ON
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)

target_sources(${PROJECT_NAME}
    PRIVATE

    src/protocol.cpp
    src/util.cpp
    src/vlv.cpp
    src/mthd.cpp
    src/mtrk.cpp
    src/midi_input_device.cpp

    src/event/note.cpp
    src/event/pitch.cpp
    src/event/control.cpp
    src/event/key_pressure.cpp
    src/event/channel_pressure.cpp
    src/event/program_change.cpp

    src/event/meta/tempo.cpp
    src/event/meta/text.cpp
    src/event/meta/time_signature.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE GTest::gtest
    PRIVATE GTest::gtest_main
    PRIVATE ${TOP_LEVEL_PROJECT})

add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)

gtest_discover_tests(${PROJECT_NAME})
