cmake_minimum_required(VERSION 3.28)
project(CodingProblems_2024)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_C_FLAGS "-Wall -pedantic -Wsign-compare")

set(SANITIZER_FLAGS "-fsanitize=address,undefined")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SANITIZER_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZER_FLAGS}")

add_executable(RGB_to_hex_conversion src/CodeWars/RGB_to_hex_conversion/main.cpp)

add_executable(Snail_sort src/CodeWars/Snail_sort/main.cpp)

add_executable(n-Queens_SA src/CodeWars/n-Queens/SimulatedAnnealing/main.cpp)

add_executable(n-Queens_SA_new src/CodeWars/n-Queens/SimulatedAnnealing/temp.cpp)

add_executable(n-Queens_MC src/CodeWars/n-Queens/MinConflicts/main.cpp)
