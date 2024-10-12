cmake_minimum_required(VERSION 3.28)
project(CodingProblems_2024)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "-Wall -pedantic -Wsign-compare")
set(CMAKE_CXX_FLAGS "-Wall -pedantic -Wsign-compare")  # Include warnings for C++ as well

set(SANITIZER_FLAGS "-fsanitize=address -fsanitize=undefined")  # Separate flags for clarity

# Add sanitizer flags for both C and C++
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SANITIZER_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZER_FLAGS}")

find_package(GTest REQUIRED)

add_executable(RGB_to_hex_conversion src/CodeWars/RGB_to_hex_conversion/main.cpp)

add_executable(Snail_sort src/CodeWars/Snail_sort/main.cpp)

add_executable(n-Queens_SA src/CodeWars/n-Queens/SimulatedAnnealing/main.cpp)

add_executable(n-Queens_SA_new src/CodeWars/n-Queens/SimulatedAnnealing/temp.cpp)

add_executable(n-Queens_MC src/CodeWars/n-Queens/MinConflicts/main.cpp)

add_executable(REPL src/CodeWars/InteractiveInterpreter/main.cpp)

add_executable(REPL_tests src/CodeWars/InteractiveInterpreter/tests.cpp)
target_link_libraries(REPL_tests ${GTEST_LIBRARIES} pthread)

include_directories(${GTEST_INCLUDE_DIRS})
