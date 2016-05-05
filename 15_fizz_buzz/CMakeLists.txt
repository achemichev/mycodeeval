cmake_minimum_required(VERSION 3.3)
project(15_fizz_buzz)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

#set(SOURCE_FILES main.c readbuf.c challenge.c)

set(SOURCE_FILES codeeval_assembly.c)

add_executable(15_fizz_buzz ${SOURCE_FILES})