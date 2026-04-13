cmake_minimum_required(VERSION 3.16)
project(Consegna2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(consegna2 consegna2.cpp)
target_include_directories(consegna2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(consegna2 PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_custom_target(run
    COMMAND $<TARGET_FILE:consegna2>
    DEPENDS consegna2
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    USES_TERMINAL
)

add_custom_target(run_verbose
    COMMAND $<TARGET_FILE:consegna2> -verbose
    DEPENDS consegna2
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    USES_TERMINAL
)

add_custom_target(run_graph
    COMMAND $<TARGET_FILE:consegna2> -graph
    DEPENDS consegna2
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    USES_TERMINAL
)
