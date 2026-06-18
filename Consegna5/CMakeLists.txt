cmake_minimum_required(VERSION 3.10)

project(consegna5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(consegna5
    src/csv_loader.cpp
    src/edge_sort.cpp
    src/kruskal.cpp
    src/main.cpp
    src/union_find.cpp
)

target_include_directories(consegna5 PRIVATE include)
