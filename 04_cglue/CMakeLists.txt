cmake_minimum_required(VERSION 3.3)
project(cglue)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

set(SOURCE_FILES main.c)
add_executable(cglue ${SOURCE_FILES})