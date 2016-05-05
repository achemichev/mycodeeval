cmake_minimum_required(VERSION 3.3)
project(13_reverse_and_add)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(SOURCE_FILES main.c)
add_executable(13_reverse_and_add ${SOURCE_FILES})