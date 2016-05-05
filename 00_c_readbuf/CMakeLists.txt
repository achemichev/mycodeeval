cmake_minimum_required(VERSION 3.3)
project(00_c_readbuf)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(SOURCE_FILES main.c)
add_executable(00_c_readbuf ${SOURCE_FILES})