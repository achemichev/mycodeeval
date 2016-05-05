cmake_minimum_required(VERSION 3.3)
project(longest_common_subseq_v100)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c)
add_executable(longest_common_subseq_v100 ${SOURCE_FILES})