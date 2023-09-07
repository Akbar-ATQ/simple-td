cmake_minimum_required(VERSION 3.12)
project(simple-TD-V2)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
  ${CMAKE_SOURCE_DIR}/src/main.cpp
  ${CMAKE_SOURCE_DIR}/src/level.cpp
  ${CMAKE_SOURCE_DIR}/src/grid/grid_helper.cpp
  ${CMAKE_SOURCE_DIR}/src/grid/update_unit.cpp
  ${CMAKE_SOURCE_DIR}/src/grid/update_terrain.cpp
  ${CMAKE_SOURCE_DIR}/src/terrain/spawner.cpp
  ${CMAKE_SOURCE_DIR}/src/terrain/platform.cpp
  ${CMAKE_SOURCE_DIR}/src/unit/tower/tower.cpp
  ${CMAKE_SOURCE_DIR}/src/unit/tower/bullet.cpp
  ${CMAKE_SOURCE_DIR}/src/unit/enemy/enemy.cpp
  ${CMAKE_SOURCE_DIR}/src/unit/enemy/path_finding.cpp
)

add_executable(main ${SOURCES})

include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/third_party")

target_include_directories(main PUBLIC
    ${PROJECT_SOURCE_DIR}/lib
    ${PROJECT_SOURCE_DIR}/third_party
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/grid
    ${PROJECT_SOURCE_DIR}/src/terrain
    ${PROJECT_SOURCE_DIR}/src/unit/tower
    ${PROJECT_SOURCE_DIR}/src/unit/enemy
)

target_link_libraries(main
    ${CMAKE_SOURCE_DIR}/static_lib/libraylib.a
    ${CMAKE_SOURCE_DIR}/static_lib/liba-star.a
    winmm
    opengl32
)

set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
