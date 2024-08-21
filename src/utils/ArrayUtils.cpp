// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : ArrayUtils.cpp                                                    ║
// ║ Description  : Contains frequently used functions                                ║
// ║              : for working with array-type datatypes                             ║
// ║ Author(s)    : Gilles Van pellicom                                               ║
// ║ Date         : 2024/08/21                                                        ║                
// ║ Version      : 1.0                                                               ║
// ║ License      : GPL-3.0                                                           ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#include <iostream>
#include <vector>
/**
 * Prints all elements of a vector to the standard output.
 * Elements are separated by commas and enclosed in square brackets for readability.
 *
 * @tparam A Type of elements in array.
 * @param arr Array
 */
template<typename A>
void printArray(const std::vector<A>& arr) {
  std::cout << "[" << arr[0];
  for (size_t i = 1; i < arr.size(); ++i) {
    std::cout << ", " << arr[i];
  }
  std::cout << "]" << std::endl;
}