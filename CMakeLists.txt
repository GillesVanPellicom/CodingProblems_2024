cmake_minimum_required(VERSION 3.28)
project(CodingProblems_2024)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_C_FLAGS "-Wall -pedantic -Wsign-compare")

add_executable(RGB_to_hex_conversion src/CodeWars/RGB_to_hex_conversion/main.cpp
        src/utils/ArrayUtils.cpp)

add_executable(Snail_sort src/CodeWars/Snail_sort/main.cpp)

