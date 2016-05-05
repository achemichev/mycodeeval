cmake_minimum_required(VERSION 3.3)
project(06_csortcols)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

set(SOURCE_FILES main.c)
add_executable(06_csortcols ${SOURCE_FILES})