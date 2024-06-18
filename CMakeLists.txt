cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(horizons)

FILE(GLOB_RECURSE SOURCES src/*.cpp src/*.h src/*.hpp)
add_executable(horizons ${SOURCES})