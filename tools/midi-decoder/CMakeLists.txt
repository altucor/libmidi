cmake_minimum_required(VERSION 4.0.0)

set(TOP_LEVEL_PROJECT ${PROJECT_NAME})
project(midi-decoder C CXX)

add_executable(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)

target_sources(${PROJECT_NAME}
    PRIVATE
    src/main.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework CoreMIDI -framework CoreAudio -framework AudioToolbox -framework CoreFoundation")

target_link_libraries(${PROJECT_NAME} PRIVATE midi)
target_link_libraries(${PROJECT_NAME} PRIVATE common)

target_include_directories(${PROJECT_NAME} PRIVATE midi)