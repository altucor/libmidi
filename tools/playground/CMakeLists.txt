cmake_minimum_required(VERSION 3.15.2)

set(TOP_LEVEL_PROJECT ${PROJECT_NAME})
project(playground C CXX)

add_executable(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)

target_sources(${PROJECT_NAME}
    PRIVATE
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE midi)
target_include_directories(${PROJECT_NAME} PRIVATE midi)