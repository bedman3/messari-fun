cmake_minimum_required(VERSION 3.10)
project(messari)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(messari
        src/main.cpp
        )
