cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(horizons)

add_executable(horizonpull)
add_executable(horizonread)
add_subdirectory(src)