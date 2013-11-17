cmake_minimum_required(VERSION 2.8)

#include(ExternalProject)

if(UNIX)

    set( CMAKE_CXX_FLAGS  "-std=c++11 -pedantic -pedantic-errors -W -Wall -Wextra -Wconversion" )
    #set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

endif(UNIX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

project(Task_3)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../task_2/lib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../task_2/lib ${CMAKE_CURRENT_BINARY_DIR}/lib)

add_subdirectory(bad_channel)
add_subdirectory(emulation_queues)
add_subdirectory(split_datafeed)

