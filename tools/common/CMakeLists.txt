cmake_minimum_required(VERSION 4.0.0)

set(TOP_LEVEL_PROJECT ${PROJECT_NAME})
project(common C CXX)

add_library(${PROJECT_NAME} INTERFACE)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)

target_sources(${PROJECT_NAME}
    INTERFACE
    FILE_SET HEADERS
    BASE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    FILES
    include/common/eventPrint.hpp
    include/common/driver.hpp
)

target_link_libraries(${PROJECT_NAME} INTERFACE midi)
target_include_directories(${PROJECT_NAME} INTERFACE midi)