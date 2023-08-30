cmake_minimum_required(VERSION 3.12)
project(simple-TD-V2)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
  ${PROJECT_SOURCE_DIR}/include/tile.cpp
  ${CMAKE_SOURCE_DIR}/src/main.cpp
  ${CMAKE_SOURCE_DIR}/src/level.cpp
  ${CMAKE_SOURCE_DIR}/src/unit/path_finding.cpp
)

add_executable(main ${SOURCES})

include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/include_external")

target_include_directories(main PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include_external
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/grid
    ${PROJECT_SOURCE_DIR}/src/terrain
    ${PROJECT_SOURCE_DIR}/src/unit
)

target_link_libraries(main
    ${CMAKE_SOURCE_DIR}/lib/libraylib.a
    ${CMAKE_SOURCE_DIR}/lib/liba-star.a
    ${CMAKE_SOURCE_DIR}/lib/libQuadTree.a
    winmm
    opengl32
)

set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
