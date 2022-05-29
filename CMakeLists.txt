cmake_minimum_required(VERSION 3.22)
project(BFInterpreter)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE
    src/main.cpp
    src/BFMachine.cpp)

add_executable(BFInterpreter ${SOURCE})

target_include_directories(BFInterpreter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
