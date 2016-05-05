cmake_minimum_required(VERSION 3.3)
project(08_c_meet_comb_sort)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(SOURCE_FILES main.c)
add_executable(08_c_meet_comb_sort ${SOURCE_FILES})