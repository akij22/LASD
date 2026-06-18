cmake_minimum_required(VERSION 3.10)

project(consegna4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(consegna4
    src/algorithms.cpp
    src/cli.cpp
    src/graph.cpp
    src/list.cpp
    src/main.cpp
    src/min_heap.cpp
    src/output.cpp
    src/state.cpp
    src/utilities.cpp
)

target_include_directories(consegna4 PRIVATE include)
