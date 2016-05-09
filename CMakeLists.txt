cmake_minimum_required(VERSION 2.8)
project(LightSpeed)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")
subdirs(src shader)

