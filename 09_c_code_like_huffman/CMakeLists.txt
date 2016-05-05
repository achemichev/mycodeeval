cmake_minimum_required(VERSION 3.3)
project(09_c_code_like_huffman)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(SOURCE_FILES main.c readbuf.c huffman.c)
add_executable(09_c_code_like_huffman ${SOURCE_FILES})