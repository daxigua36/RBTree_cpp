cmake_minimum_required(VERSION 3.5)
project(RBTree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Tree.h)
add_executable(RBTree ${SOURCE_FILES})