cmake_minimum_required(VERSION 2.8)
project(project2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES project2.cpp IODevice.h IODevice.cpp Cpu.h Cpu.cpp Process.h Process.cpp Event.h Event.cpp)

SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR} )

add_executable(project2 ${SOURCE_FILES})
