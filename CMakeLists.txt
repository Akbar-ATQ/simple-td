cmake_minimum_required(VERSION 3.12)
project(simple-TD-V2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-enum-compare")

set(SOURCES
  ${CMAKE_SOURCE_DIR}/src/main.cpp
  ${CMAKE_SOURCE_DIR}/src/tile.cpp
  ${CMAKE_SOURCE_DIR}/src/level_manager.cpp
)

add_executable(main ${SOURCES})

target_include_directories(main PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include_external
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(main ${CMAKE_SOURCE_DIR}/lib/libraylib.a winmm opengl32)

set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
