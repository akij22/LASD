cmake_minimum_required(VERSION 3.16)

project(consegna3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(consegna3
    main.cpp
    options.cpp
    linked_list.cpp
    graph.cpp
    dot_output.cpp
    dfs.cpp
)

target_compile_options(consegna3 PRIVATE
    -Wall
    -Wextra
    -pedantic
)
