// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Project      : CodeWars - Snail sort                                             ║
// ║ File         : main.cpp                                                          ║
// ║ Author(s)    : Gilles Van pellicom                                               ║
// ║ Date         : 2024/08/21                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝
#include "../../utils/ArrayUtils.cpp"
#include <vector>

/**
 * Traverses a 2D-array in the snail pattern
* @example array = [[1,2,3],
                    [4,5,6],
                    [7,8,9]]
           snail(array) => [1,2,3,6,9,8,7,4,5]
 * @param snail_map 2D-vector to be traversed
 * @return Traversal result as vector
 */
std::vector<int> snail(const std::vector<std::vector<int> > &snail_map) {
  std::vector<int> res;

  if (snail_map.empty() || snail_map[0].empty()) {
    return res; // Handle empty matrix case
  }

  constexpr int dx[] = {0, 1, 0, -1}, dy[] = {1, 0, -1, 0};
  int offsetLeft = 0, offsetRight = 0, offsetTop = 0, offsetBottom = 0;
  int direction = 0; // 0 = E, 1 = S, 2=  W, 3 = N
  int index = 0; // Current iteration count
  const int target = static_cast<int>(snail_map.size() * snail_map.size()); // Target iteration count
  int x = 0;
  int y = 0;

  while (true) {
    // Add current element to result
    res.emplace_back(snail_map[x][y]);

    // Check if direction switch is required
    if ((direction == 0 && y == static_cast<int>(snail_map.size() - offsetRight - 1)) ||
      (direction == 1 && x == static_cast<int>(snail_map.size() - offsetBottom - 1)) ||
      (direction == 2 && y == offsetLeft) ||
      (direction == 3 && x == offsetTop)) {
      switch (direction) {
        case 0: offsetTop++; break;
        case 1: offsetRight++; break;
        case 2: offsetBottom++; break;
        case 3: offsetLeft++; break;
      }
      // Update direction
      direction = (direction + 1) % 4;
      }

    x += dx[direction];
    y += dy[direction];

    index++;
    if (index == target) {
      // Finished
      return res;
    }
  }
}

int main() {
  std::vector<std::vector<int> > arr = {
    {1, 2, 3, 10},
    {4, 5, 6, 11},
    {7, 8, 9, 12},
    {13, 14, 15, 16}

  };
  printArray(snail(arr));
  return 0;
}
