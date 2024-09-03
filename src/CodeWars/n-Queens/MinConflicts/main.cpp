// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Project      : CodeWars - n-Queens (minConflicts)                                ║
// ║ Version      : v0.3.1 "general optimizations & cleanup"                          ║
// ║ File         : main.cpp                                                          ║
// ║ Author(s)    : Gilles Van pellicom                                               ║
// ║ Date         : 2024/08/22                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <random>
#include <string>
// #include <chrono>
// #include "../../utils/ArrayUtils.cpp"


namespace nQueens {
// Global variables
using Board = std::vector<int>;
std::mt19937 gen(std::random_device{}()); // Mersenne Twister random

// Profiling variables
long iterationCount = 0;
int stagnationCount = 0;

/**
 * Calculates the energy (E) of the entire board.
 * The higher E, the more conflicts.
 * No conflicts is E = 0
 * @param b Board from which to calculate E
 * @return energy as 𝕫⁺
 */
inline int calculateFullE(const Board& b) {
  const int n = static_cast<int>(b.size())-1;
  int E = 0;
  for (int i = 0; i < n; ++i) {
    // Dont count i == j
    for (int j = i + 1; j < n; ++j) {
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
    auto [mx, my] = mandatoryQueenCoordinates;

    // Mandatory queen
    res[mx] = my;

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
    return res;
}


/**
 * Utilises heuristics to calculate a valid solution for the n-Queens problem.
 * One queen position is mandatory and given via argument
 * @param n n ⋅ n board with n queens
 * @param queenPos Coordinates for the mandatory queen
 * @return Board with solution or empty if no solution found.
 */
Board minConflicts(const int n, const std::pair<int, int>& queenPos) {
  // Reset iterationcount in case of multiple runs
  iterationCount = 0;
  auto [mx, my] = queenPos;

  // Initialize board
  Board b = generateBoard(n, queenPos);
  const int E_begin = calculateFullE(b);
  int E_board = 0;
  std::uniform_int_distribution<> nGen(0, n - 1);

  // Max iterations before algorithm gives up
  // Set to static 5000 since this is the upper limit for n ≤ 1000
  constexpr int iterationMax = 4000;


  // Stagnation detection variables
  const int stagnationCheckInterval = 10 * n;
  // Precompute stagnation check intervals
  const int stagnationIntervalOne = static_cast<int>(std::floor(stagnationCheckInterval / 1.5));
  const int stagnationIntervalTwo = static_cast<int>(std::floor(stagnationCheckInterval / 3.0));
  // Initialize stagnation check sample variables
  int stagnationSampleOne = E_begin;
  int stagnationSampleTwo = stagnationSampleOne;
  int stagnationSampleThree = stagnationSampleOne;

  // Main loop
  while (true) {
    if (iterationCount >= iterationMax) {
      // Fail
      return {};
    }

    iterationCount++;

    std::vector<int> highestConflictRows;
    int E_best = 0;
    int E_total = 0;

    // STEP 1: calculate E for all rows, find all rows with highest E
    for (int i = 0; i < n; ++i) {
      // Skip queen
      if (i == mx) continue;

      // Calculate energy for the row
      int E_current = -1;
      for (int j = 0; j < n; ++j) {
        if (b[i] == b[j] || std::abs(b[i] - b[j]) == std::abs(i - j)) {
          E_current++;
        }
      }
      // Sum all rows E to avoid having to re-calculate when board total is needed
      E_total += E_current;

      // If E_current > E_best, E_current is the new best
      if (E_current > E_best) {
        E_best = E_current;
        // Clear highest conflict list since it's populated only by lower E's.
        highestConflictRows.clear();
        highestConflictRows.push_back(i);
      } else if (E_current == E_best) {
        // If E_current == E_best, add to list
        highestConflictRows.push_back(i);
      }
    }
    // Set board total to calculated total
    E_board = E_total;

    // Check if solution satisfies constraints
    if (E_board == 0) {
      // Solution found
      return b;
    }


    // STEP 2: check/resolve stagnation
    // Take samples for stagnation check
    if (iterationCount % stagnationIntervalOne == 0) {
      stagnationSampleTwo = E_board;
    }
    if (iterationCount % stagnationIntervalTwo == 0) {
      stagnationSampleThree = E_board;
    }

    // Check stagnation every stagnationCheckInterval iterations to combat local minima
    if (iterationCount % stagnationCheckInterval == 0) {

      // if all samples are equal
      if (stagnationSampleOne == stagnationSampleTwo &&
        stagnationSampleOne == stagnationSampleThree &&
        stagnationSampleOne == E_board) {
        // Stagnation detected.
        stagnationCount++;
        // std::cout << "Stagnation count = " << stagnationCount << std::endl;

        // Attempt to escape local minima by perturbing the board
        // Perturbation amount calculated using modified variant of simulated annealing.
        // Perturbation = ⌈min_perbutation + (E_current / E_begin) * (max_perbutation - min_perbutation)⌉
        // Perturbation = ⌈1 + (E_board / E_begin) * (n / 5.0 - 1)⌉
        // Ceil so that Perturbation < 1 will always result in at least one change
        for (int i = 0; i < ceil(1 + (static_cast<double>(E_board) / E_begin) * (n / 10.0 - 1)); ++i) {
          const int row = nGen(gen);
          // Ignore manditory queen row
          if (row == queenPos.first) {
            continue;
          }
          // perturbation
          b[row] = nGen(gen);
        }
      }
      // take new sampleOne for next stagnation check
      stagnationSampleOne = E_board;
    }


    // STEP 3: for a random highestConflictRow, find lowest energy location to move that queen to on the same row
    std::uniform_int_distribution<> conflictGen(0, static_cast<int>(highestConflictRows.size()) - 1);
    const int row = highestConflictRows[conflictGen(gen)];

    // Evaluate all columns in the selected row to find the best positions
    int E_min = n + 1;
    Board bestCols;

    for (int col = 0; col < n; ++col) {
      b[row] = col; // Tentatively place queen
      int E_current = -1;

      // Calculate E for the tentative placement
      for (int i = 0; i < n; ++i) {
        if (b[i] == col || std::abs(b[i] - col) == std::abs(i - row)) {
          E_current++;
        }
      }

      // Update best columns if this placement is as good as the current best
      if (E_current < E_min) {
        E_min = E_current;
        bestCols.clear();
        bestCols.push_back(col);
      } else if (E_current == E_min) {
        bestCols.push_back(col);
      }
    }

    // Randomly select one of the best columns
    std::uniform_int_distribution<> bestColRand(0, static_cast<int>(bestCols.size()) - 1);
    b[row] = bestCols[bestColRand(gen)];

  }
  // Unreachable but I like it so it can stay
  return b;
}



std::string solveNQueens(const int n, const std::pair<int, int>& mandatoryQueenCoordinates) {
  std::string res;

  // Base cases
  if (n == 1) {
    // Trivial solution
    return "Q\n";
  } else if (n < 4) {
    // No solution
    return res;
  }

  const Board b = minConflicts(n, mandatoryQueenCoordinates);

  if (b.empty()) {
    // std::cout << "No solution possible" << std::endl;
    return res;
  }

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (b[j] == i) {
        res += "Q";
        continue;
      }
      res += ".";
    }
    res += "\n";
  }
  return res;
}
}

int main() {
  using namespace nQueens;
  constexpr int n = 6;

  // Board b = minConflicts(n, {2, 1});
  // for (int i = 0; i < n; ++i) {
  //   std::cout << calculateE(b, i) << " ";
  // }
  // std::cout << std::endl;
  // printBoard(b);

  // std::cout << std::endl << ". . . . Q .\n. . Q . . .\nQ . . . . .\n. . . . . Q\n. . . Q . .\n. Q . . . .\n";
  // measurePerformance(n, {2, 1}, 10);

  const std::string res = solveNQueens(n, {5, 5});
  std::cout << res << std::endl;
  return 0;
}