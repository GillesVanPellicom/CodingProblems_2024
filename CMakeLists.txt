cmake_minimum_required(VERSION 3.28)
project(CodingProblems_2024)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_C_FLAGS "-Wall -pedantic -Wsign-compare")

set(SANITIZER_FLAGS "-fsanitize=address,undefined")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SANITIZER_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZER_FLAGS}")

add_executable(RGB_to_hex_conversion src/CodeWars/RGB_to_hex_conversion/main.cpp
        src/utils/ArrayUtils.cpp
        src/CodeWars/n-Queens/main.cpp
)
target_compile_options(RGB_to_hex_conversion PRIVATE ${SANITIZER_FLAGS})

add_executable(Snail_sort src/CodeWars/Snail_sort/main.cpp)
target_compile_options(Snail_sort PRIVATE ${SANITIZER_FLAGS})

add_executable(n-Queens src/CodeWars/n-Queens/main.cpp)
target_compile_options(n-Queens PRIVATE ${SANITIZER_FLAGS})
