cmake_minimum_required(VERSION 3.3)
project(dna_align_v100)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(SOURCE_FILES main.c)
add_executable(dna_align_v100 ${SOURCE_FILES})