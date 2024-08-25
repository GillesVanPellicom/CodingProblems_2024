// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Project      : CodeWars - n-Queens (minConflicts)                                ║
// ║ Version      : v0.2.0 "local minima detection/resolution"                        ║
// ║ File         : main.cpp                                                          ║
// ║ Author(s)    : Gilles Van pellicom                                               ║
// ║ Date         : 2024/08/22                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <random>
#include <set>
#include <chrono>
#include <string>
#include <__algorithm/ranges_max.h>
// #include "../../utils/ArrayUtils.cpp"


namespace nQueens {
using Board = std::vector<int>;
std::mt19937 gen(std::random_device{}()); // Mersenne Twister random
int _n = -1;
/**
 * Calculates the energy (E) of a specified queen.
 * The higher E, the more conflicts.
 * No conflicts is E = 0
 * @param b Board from which to calculate E
 * @param row Row of queen to check
 * @return energy as 𝕫⁺
 */
int calculateE(const Board& b, const int row) {
  int E = 0;
  for (int i = 0; i < _n; ++i) {
    // if (not in same col) or (not in same diagonal)
    // row doesn't need to be checked since the code doesn't allow two queens to generate in one row.
    if (b[row] == b[i] || std::abs(b[row] - b[i]) == std::abs(row - i)) {
      ++E;
    }
  }
  return E - 1;
}

int calculateFullE(const Board& b) {
  int E = 0;
  for (int i = 0; i < _n; ++i) {
    // Dont count i == j
    for (int j = i + 1; j < _n; ++j) {
      // if (not in same col) or (not in same diagonal)
      // row doesn't need to be checked since the code doesn't allow two queens to generate in one row.
      if (b[i] == b[j] || std::abs(b[i] - b[j]) == std::abs(i - j)) {
        ++E;
      }
    }
  }
  return E;
}


/**
 * Generates a random starting board layout.
 * One mandatory queen coordinate is required
 * Only one queen can generate per row.
 *
 * @param n board size (n*n) and amount of queens
 * @param mandatoryQueenCoordinates
 * @return Randomised starting board
 */
Board generateBoard(const int n, const std::pair<int, int>& mandatoryQueenCoordinates) {
  std::uniform_int_distribution<> rand(0, n - 1); // Uniform integer distribution [0; n]

  Board res;
  auto [my, mx] = mandatoryQueenCoordinates;
  // For each row, choose a pseudo-random col
  for (int i = 0; i < n; ++i) {
    res.emplace_back(rand(gen));
  }
  // Ensure mandatory queen is correct
  res[mx] = my;
  return res;
}

long iterationCount = 0;
int stagnationCount = 0;


Board minConflicts(const int n, const std::pair<int, int>& queenPos) {
  // Initialize _n
  _n = n;

  // Initialize board
  Board b = generateBoard(n, queenPos);

  // Stagnation detection variables
  // stagnationInterval = max(⌊c_1 ⋅ n⌋, c_2) where c = constant
  const int stagnationInterval = std::ranges::max(static_cast<int>(floor(10 * n)), 30);
  int stagnationSample = calculateFullE(b);

  while (true) {
    iterationCount++;
    int E_best = 0;
    std::vector<int> highestConflictRows;

    for (int i = 0; i < n; ++i) {
      // If E_current > E_best, E_current is the new best
      if (const int E_current = calculateE(b, i);
        E_current > E_best) {
        E_best = E_current;
        // Clear highest conflict list since it's populated only by lower E's.
        highestConflictRows.clear();
        highestConflictRows.push_back(i);
      } else if (E_current == E_best) {
        // If E_current == E_best, add to list
        highestConflictRows.push_back(i);
      }
    }

    // Check if solution satisfies constraints
    const int E_board = calculateFullE(b);
    if (E_best == 0) {
      // done
      std::cout << "Finished at iteration " << iterationCount << std::endl;

      return b;
    }

    // Check stagnation every max(⌊c ⋅ n⌋, 100) iterations to combat local minima
    if (iterationCount % stagnationInterval == 0) {
      std::cout << "Stagnation check: " << std::endl
          << "Current iteration = " << iterationCount << std::endl
          << "E = " << E_board << std::endl;

      // if |E_current - stagnationSample| <= tolerance
      if (std::abs(E_board - stagnationSample) <= 0) {
        // Stagnation detected.
        stagnationCount++;
        std::cout << "Stagnation count = " << stagnationCount << std::endl;

        for (int i = 0; i < floor(n / 3.5); ++i) {
          std::uniform_int_distribution<> rowGen(0, n - 1);
          std::uniform_int_distribution<> colGen(0, n - 1);
          const int row = rowGen(gen);
          if (row == queenPos.second) {
            continue;
          }
          b[row] = colGen(gen);
        }
      }
      // take new sample for next stagnation check
      stagnationSample = E_board;
    }

    std::uniform_int_distribution<> rowGen(0, static_cast<int>(highestConflictRows.size()) - 1);
    if (const int row = highestConflictRows[rowGen(gen)];
      row == queenPos.second) {
      b[row] = queenPos.first;
    } else {
      std::uniform_int_distribution<> colGen(0, _n - 1); // Uniform integer distribution [0; n-1]
      b[row] = colGen(gen);
    }
  }
  // Unreachable but I like it so it can stay
  return b;
}

void printBoard(const Board& b) {
  std::string res;
  const int n = static_cast<int>(b.size());
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (b[j] == i) {
        res += "Q ";
        continue;
      }
      res += ". ";
    }
    res += "\n";
  }
  std::cout << res << std::endl;
}


std::string solveNQueens(const int n, const std::pair<int, int>& mandatoryQueenCoordinates) {
  std::string res;

  // Base cases
  if (n == 1) {
    // Trivial solution
    return "Q";
  } else if (n < 4) {
    // No solution
    return res;
  }

  const Board b = minConflicts(n, mandatoryQueenCoordinates);

  if (b.empty()) {
    return res;
  }

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (b[j] == i) {
        res += "Q ";
        continue;
      }
      res += ". ";
    }
    res += "\n";
  }
  return res;
}

void measurePerformance(const int n, const std::pair<int, int>& mandatoryQueenCoordinates, const int epochs) {
  int i;
  // Start time
  const auto start = std::chrono::high_resolution_clock::now();
  for (i = 0; i < epochs; ++i) {
    solveNQueens(n, mandatoryQueenCoordinates);
  }
  // End time
  const auto end = std::chrono::high_resolution_clock::now();
  // Calculate duration in milliseconds
  const std::chrono::duration<double, std::milli> duration = end - start;
  std::cout
      << "Test suite results:\n"
      << "\tTest settings: n = " << n << ", " << epochs << " epochs\n"
      << "\tIterations (avg) \t: " << iterationCount / i << "\n"
      << "\tStagnations (avg) \t: " << stagnationCount / i << "\n"
      << "\tTime elapsed \t\t: " << duration.count() << " ms\n"
      << "\tTime (avg)   \t\t: " << (double) (duration.count() / (double) epochs) << " ms\n" << std::endl;
}
}

int main() {
  using namespace nQueens;
  constexpr int n = 500;

  // Board b = minConflicts(n, {0, 3});
  // for (int i = 0; i < n; ++i) {
  //   std::cout << calculateE(b, i) << " ";
  // }
  // std::cout << std::endl;
  // printBoard(b);

  measurePerformance(n, {0, 3}, 1);

  // std::cout << solveNQueens(n, {0, 3});
  return 0;
}
