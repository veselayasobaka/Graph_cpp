project(graph_qt)
cmake_minimum_required(VERSION 2.8)
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -pedantic -ggdb -std=c++11")
set(SOURCE_FILES graph.cpp graphIO.cpp main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})


