cmake_minimum_required(VERSION 2.8)

if(UNIX)

    set( CMAKE_CXX_FLAGS  "-std=c++11 -pedantic -pedantic-errors -W -Wall -Wextra -Wconversion" )
    #set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

endif(UNIX)

project(Task_4)


#add_subdirectory(4_4)

add_subdirectory(tests)

enable_testing()