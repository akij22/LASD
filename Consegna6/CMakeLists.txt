cmake_minimum_required(VERSION 3.10)

project(consegna6)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(consegna6
    src/alignment.cpp
    src/file_io.cpp
    src/legacy_support.cpp
    src/main.cpp
    src/scoring.cpp
    src/state.cpp
    src/tokenizer.cpp
)

target_include_directories(consegna6 PRIVATE include)
