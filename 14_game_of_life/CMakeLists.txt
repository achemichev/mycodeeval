cmake_minimum_required(VERSION 3.3)
project(14_game_of_life)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(SOURCE_FILES main.c)
add_executable(14_game_of_life ${SOURCE_FILES})