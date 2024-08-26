// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Project      : CodeWars - n-Queens (minConflicts)                                ║
// ║ Version      : v0.3.0 "greedy-random board initialization (gan-init)"            ║
// ║ File         : main.cpp                                                          ║
// ║ Author(s)    : Gilles Van pellicom                                               ║
// ║ Date         : 2024/08/22                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <random>
#include <chrono>
#include <string>
// #include "../../utils/ArrayUtils.cpp"


namespace nQueens {
// Global variables
using Board = std::vector<int>;
std::mt19937 gen(std::random_device{}()); // Mersenne Twister random

// Profiling variables
long iterationCount = 0;
int stagnationCount = 0;

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
 * Generates starting board layout
 * One mandatory queen coordinate is required?
 * Only one queen can generate per row.
 * Uses a random-greedy initialization algorithm (gan-init).
 * gan-init tries to make the board as low in energy as possible,
 * but still introduces some randomness
 * so as to be able to explore the solution space effectively.
 *
 * @param n board size (n*n) and amount of queens
 * @param mandatoryQueenCoordinates
 * @return Randomised starting board
 */
Board generateBoard(const int n, const std::pair<int, int>& mandatoryQueenCoordinates) {
    Board res(n, -1);  // Initialize board with -1, indicating no queen placed yet.
    auto [my, mx] = mandatoryQueenCoordinates;

    // Mandatory queen
    res[mx] = my;

    std::uniform_int_distribution<> rand(0, n - 1); // Random distribution for perturbation

    // Place queens in remaining rows
    for (int i = 0; i < n; ++i) {
        if (i == mx) continue;  // Skip the mandatory queen's row

        int minConflicts = n + 1;  // Set to max possible conflicts + 1
        std::vector<int> bestCols;

        // Evaluate all columns in row `i` to find those with the fewest conflicts
        for (int col = 0; col < n; ++col) {
            res[i] = col;  // Tentatively place queen
            int conflicts = 0;

            // Calculate conflicts for tentative placement
            for (int j = 0; j < i; ++j) {
                if (res[j] == col || std::abs(res[j] - col) == std::abs(j - i)) {
                    conflicts++;
                }
            }

            // Update best columns if this placement is as good as the current best
            if (conflicts < minConflicts) {
                minConflicts = conflicts;
                bestCols.clear();
                bestCols.push_back(col);
            } else if (conflicts == minConflicts) {
                bestCols.push_back(col);
            }
        }

        // Randomly select one of the best columns to place the queen
        std::uniform_int_distribution<> bestColRand(0, static_cast<int>(bestCols.size()) - 1);
        res[i] = bestCols[bestColRand(gen)];
    }

    // Apply random perturbation to introduce additional variability
    for (int i = 0; i < n / 10; ++i) {

        if (const int row = rand(gen);
          row != mx) {  // Avoid perturbing the mandatory queen
            res[row] = rand(gen);
        }
    }
    return res;
}


Board minConflicts(const int n, const std::pair<int, int>& queenPos) {
  // Initialize _n
  _n = n;

  // Initialize board
  Board b = generateBoard(n, queenPos);
  const int E_begin = calculateFullE(b);
  std::uniform_int_distribution<> nGen(0, n - 1);


  // Stagnation detection variables
  // stagnationInterval = max(⌊c_1 ⋅ n⌋, c_2) where c = constant
  const int stagnationCheckInterval = static_cast<int>(floor(10 * n));
  // Precompute stagnation check intervals
  const int stagnationIntervalOne = static_cast<int>(std::floor(stagnationCheckInterval / 1.5));
  const int stagnationIntervalTwo = static_cast<int>(std::floor(stagnationCheckInterval / 3.0));
  const int lcmInterval = std::lcm(std::lcm(stagnationIntervalOne, stagnationIntervalTwo), stagnationCheckInterval);
  // Initialize stagnation check sample variables
  int stagnationSampleOne = E_begin;
  int stagnationSampleTwo = stagnationSampleOne;
  int stagnationSampleThree = stagnationSampleOne;


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

    const int E_board = calculateFullE(b);
    // std::cout << "E = " << E_board << std::endl;

    // Check if solution satisfies constraints
    if (E_best == 0) {
      // done
      std::cout << "Finished at iteration " << iterationCount << std::endl;
      std::cout << "E_begin = " << E_begin << std::endl << "lcm = " << lcmInterval << std::endl;
      return b;
    }


    // Take samples for stagnation check
    if (iterationCount % stagnationIntervalOne == 0) {
      stagnationSampleTwo = E_board;
    }
    if (iterationCount % stagnationIntervalTwo == 0) {
      stagnationSampleThree = E_board;
    }

    // Check stagnation every max(⌊c_1 ⋅ n⌋, c_2) iterations to combat local minima
    if (iterationCount % stagnationCheckInterval == 0) {
      std::cout << "Stagnation check: " << std::endl
          << "Current iteration = " << iterationCount << std::endl
          << "E = " << E_board << std::endl;
      // std::cout << "stagnationCheckInterval = " << stagnationCheckInterval << "\n"
      //     << "stagnationIntervalOne = " << stagnationIntervalOne << "\n"
      //     << "stagnationIntervalTwo = " << stagnationIntervalTwo << "\n"
      //     << "stagnationSampleOne = " << stagnationSampleOne << "\n"
      //     << "stagnationSampleTwo = " << stagnationSampleTwo << "\n"
      //     << "stagnationSampleThree = " << stagnationSampleThree << "\n"
      //     << "stagnationCount = " << stagnationCount << "\n"
      //     << "E_begin = " << E_begin << std::endl;

      // if all samples are equal
      if (stagnationSampleOne == stagnationSampleTwo &&
        stagnationSampleOne == stagnationSampleThree &&
        stagnationSampleOne == E_board) {
        // Stagnation detected.
        stagnationCount++;
        std::cout << "Stagnation count = " << stagnationCount << std::endl;

        // Attempt to escape local minima by perturbing the board
        // Perturbation amount calculated using modified variant of simulated annealing.
        // Perturbation = ⌈min_perbutation + (E_current / E_begin) * (max_perbutation - min_perbutation)⌉
        // Perturbation = ⌈n/8 + (E_board / E_begin) * (n/3.5 - n/8)⌉
        // Ceil so that Perturbation < 1 will always result in at least one change
        for (int i = 0; i < ceil(n / 8.0 + (static_cast<double>(E_board) / E_begin) * (n / 2 - n / 8.0)); ++i) {
          const int row = nGen(gen);
          // Ignore manditory queen row
          if (row == queenPos.second) {
            continue;
          }
          // perturbation
          b[row] = nGen(gen);
        }
      }
      // take new sampleOne for next stagnation check
      stagnationSampleOne = E_board;
    }


    std::uniform_int_distribution<> conflictGen(0, static_cast<int>(highestConflictRows.size()) - 1);

    if (const int row = highestConflictRows[conflictGen(gen)]; row == queenPos.second) {
      b[row] = queenPos.first;
    } else {
      b[row] = nGen(gen);
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
      << "\tStagnations total \t: " << stagnationCount << "\n"
      << "\tStagnations (avg) \t: " << stagnationCount / i << "\n"
      << "\tTime elapsed \t\t: " << duration.count() << " ms\n"
      << "\tTime (avg)   \t\t: " << (double) (duration.count() / (double) epochs) << " ms\n" << std::endl;
}
}

int main() {
  using namespace nQueens;
  constexpr int n = 200;

  // Board b = minConflicts(n, {0, 1});
  // for (int i = 0; i < n; ++i) {
  //   std::cout << calculateE(b, i) << " ";
  // }
  // std::cout << std::endl;
  // printBoard(b);

  measurePerformance(n, {0, 3}, 10);

  // std::cout << solveNQueens(n, {0, 3});
  return 0;
}
