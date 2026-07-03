cmake_minimum_required(VERSION 4.0.0)

set(TOP_LEVEL_PROJECT ${PROJECT_NAME})
project(common C CXX)

add_library(${PROJECT_NAME} STATIC)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)

if(APPLE)
    find_library(COREMIDI_FRAMEWORK CoreMIDI)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${COREMIDI_FRAMEWORK})

    find_library(COREFOUNDATION_FRAMEWORK CoreFoundation)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${COREFOUNDATION_FRAMEWORK})
elseif(WINDOWS)
    message(FATAL_ERROR, "not implemented")
endif()

target_sources(${PROJECT_NAME}
    PRIVATE

    $<$<PLATFORM_ID:Darwin>:src/macos.cpp>
    $<$<PLATFORM_ID:Windows>:src/windows.cpp>

    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    FILES

    include/common/eventPrint.hpp
    include/common/driver.hpp
)

target_link_libraries(${PROJECT_NAME} INTERFACE midi)
target_include_directories(${PROJECT_NAME} INTERFACE midi)