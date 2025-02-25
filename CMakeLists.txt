cmake_minimum_required(VERSION 3.28)
project(CodingProblems_2024)

set(CMAKE_CXX_STANDARD 17)
# Profiling flags
set(CMAKE_C_FLAGS "-Wall -pedantic -Wsign-compare -O0 -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer")
set(CMAKE_CXX_FLAGS "-Wall -pedantic -Wsign-compare -O0 -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer")

# Default flags
#set(CMAKE_C_FLAGS "-Wall -pedantic -Wsign-compare -Ofast")
#set(CMAKE_CXX_FLAGS "-Wall -pedantic -Wsign-compare -Ofast")

#set(SANITIZER_FLAGS "-fsanitize=address -fsanitize=undefined")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SANITIZER_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZER_FLAGS}")

find_package(GTest REQUIRED)

add_executable(RGB_to_hex_conversion src/CodeWars/RGB_to_hex_conversion/main.cpp)
add_executable(RGB_to_hex_conversion_tests src/CodeWars/RGB_to_hex_conversion/tests.cpp)
target_link_libraries(RGB_to_hex_conversion_tests ${GTEST_LIBRARIES} pthread)


add_executable(Snail_sort src/CodeWars/Snail_sort/main.cpp)

add_executable(n-Queens_SA src/CodeWars/n-Queens/SimulatedAnnealing/main.cpp)

add_executable(n-Queens_MC src/CodeWars/n-Queens/MinConflicts/main.cpp)
add_executable(n-Queens_MC_tests src/CodeWars/n-Queens/MinConflicts/test.cpp)
target_link_libraries(n-Queens_MC_tests ${GTEST_LIBRARIES} pthread)

add_executable(REPL src/CodeWars/InteractiveInterpreter/main.cpp)
add_executable(REPL_tests src/CodeWars/InteractiveInterpreter/tests.cpp)
target_link_libraries(REPL_tests ${GTEST_LIBRARIES} pthread)

include_directories(${GTEST_INCLUDE_DIRS})