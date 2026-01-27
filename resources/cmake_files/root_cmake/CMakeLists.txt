cmake_minimum_required(VERSION 3.16)
project(List VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra")

set(CMAKE_CXX_FLAGS_RELEASE_INIT "-O2")

add_subdirectory(src/core)
