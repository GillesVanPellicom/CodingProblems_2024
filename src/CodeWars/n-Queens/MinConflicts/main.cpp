// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Project      : CodeWars - n-Queens (minConflicts)                                ║
// ║ Version      : v0.3.2 "Vector & general optimizations"                           ║
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
#include <chrono>


namespace nQueens {
// Global variables
using Board = std::vector<int>;
std::mt19937 gen(std::random_device{}()); // Mersenne Twister random

long iterationCount = 0;
int stagnationCount = 0;

/**
 * Calculates the energy (E) of the entire board.
 * The higher E, the more conflicts.
 * No conflicts ⇒ E = 0
 * @param b Board from which to calculate E
 * @return energy as ℤ⁺
 */
inline int calculateFullE(const Board& b) {
  const int n = static_cast<int>(b.size()) - 1;
  int E = 0;
  for (int i = 0; i < n; ++i) {
    // Don't count i == j
    const int b_i = b[i];
    for (int j = i + 1; j < n; ++j) {
      // If (not in same col) or (not in same diagonal),
      // row doesn't need to be checked since the code doesn't allow two queens to generate in one row.
      if (const int b_j = b[i]; b_i == b_j || std::abs(b_i - b_j) == std::abs(i - j)) {
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
 * as to be able to explore the solution space effectively.
 *
 * @param n board size (n*n) and amount of queens
 * @param mandatoryQueenCoordinates
 * @return Randomised starting board
 */
Board generateBoard(const int n, const std::pair<int, int>& mandatoryQueenCoordinates) {
  Board res(n, -1); // Initialize board with -1, indicating no queen placed yet.
  auto [mx, my] = mandatoryQueenCoordinates;

  // Mandatory queen placement
  res[mx] = my;

  // Track conflicts in columns and diagonals
  std::vector<int> colConflicts(n, 0);
  std::vector<int> diagConflicts(2 * n - 1, 0); // diagonal from top-left to bottom-right
  std::vector<int> antiDiagConflicts(2 * n - 1, 0); // diagonal from top-right to bottom-left

  // Declare random devices
  static std::uniform_int_distribution<> bestColRand;

  // Declare vector to store best columns
  static std::vector<int> bestCols;
  bestCols.reserve(n);

  // Set conflicts for the mandatory queen
  colConflicts[my]++;
  diagConflicts[mx - my + (n - 1)]++;
  antiDiagConflicts[mx + my]++;

  // Place queens in remaining rows
  for (int i = 0; i < n; ++i) {
    if (i == mx) continue; // Skip the mandatory queen's row

    int minConflicts = n + 1; // Max possible conflicts + 1
    bestCols.clear();

    // Evaluate all columns in row `i` to find those with the fewest conflicts
    for (int col = 0; col < n; ++col) {
      // Calculate conflicts for the column

      // Update the best columns if this placement is as good or better as the current best
      if (const int conflicts = colConflicts[col] + diagConflicts[i - col + (n - 1)] + antiDiagConflicts[i + col];
        conflicts < minConflicts) {
        minConflicts = conflicts;
        bestCols.clear();
        bestCols.push_back(col);
      } else if (conflicts == minConflicts) {
        bestCols.push_back(col);
      }
    }

    // Randomly select one of the best columns to place the queen
    bestColRand.param(std::uniform_int_distribution<>::param_type(0, static_cast<int>(bestCols.size()) - 1));
    const int chosenCol = bestCols[bestColRand(gen)];

    // Place queen in the selected column
    res[i] = chosenCol;

    // Update conflict trackers for the newly placed queen
    colConflicts[chosenCol]++;
    diagConflicts[i - chosenCol + (n - 1)]++;
    antiDiagConflicts[i + chosenCol]++;
  }
  return move(res);
}


/**
 * Utilises heuristics to calculate a valid solution for the n-Queens problem.
 * One queen position is mandatory and given via argument
 * @param n n ⋅ n board with n queens
 * @param queenPos Coordinates for the mandatory queen
 * @return Board with solution or empty if no solution found.
 */
Board minConflicts(const int n, const std::pair<int, int>& queenPos) {
  // Reset iteration count in case of multiple runs
  iterationCount = 0;
  auto [mx, my] = queenPos;

  // Initialize board
  Board b = generateBoard(n, queenPos);
  const int E_begin = calculateFullE(b);
  int E_board = 0;

  // Max iterations before algorithm gives up
  constexpr int iterationMax = 8000;

  // Stagnation detection variables
  const int stagnationCheckInterval = 10 * n;
  const int stagnationIntervalOne = static_cast<int>(std::floor(stagnationCheckInterval / 1.5));
  const int stagnationIntervalTwo = static_cast<int>(std::floor(stagnationCheckInterval / 3.0));
  int stagnationSampleOne = E_begin;
  int stagnationSampleTwo = stagnationSampleOne;
  int stagnationSampleThree = stagnationSampleOne;

  // Declare random devices
  static std::uniform_int_distribution nGen(0, n - 1);
  static std::uniform_int_distribution<> conflictGen;
  static std::uniform_int_distribution<> bestColRand;

  // Declare vector to store rows with highest conflicts
  static std::vector<int> highestConflictRows;
  highestConflictRows.reserve(n);

  // Main loop
  while (true) {
    if (iterationCount >= iterationMax) {
      // Fail
      return {};
    }

    iterationCount++;

    // Clear highest conflict rows from potential previous iterations
    highestConflictRows.clear();

    // Energy variables
    int E_best = 0;
    int E_total = 0;

    // STEP 1: calculate E for all rows, find all rows with highest E
    for (int i = 0; i < n; ++i) {
      if (i == mx) continue;

      // Calculate energy for the row
      int E_current = -1;
      const int b_i = b[i]; // Cache b[i] for performance
      for (int j = 0; j < n; ++j) {
        if (const int b_j = b[j];
          b_i == b_j || std::abs(b_i - b_j) == std::abs(i - j)) {
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
        // If E_current == E_best, add to list
        highestConflictRows.push_back(i);
      } else if (E_current == E_best) {
        highestConflictRows.push_back(i);
      }
    }
    // Set board total to calculated total
    E_board = E_total;

    // Check if solution satisfies constraints
    if (E_board == 0) {
      // Solution found
      return move(b);
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

        // Attempt to escape local minima by perturbing the board
        // Perturbation amount calculated using modified variant of simulated annealing.
        // Perturbation = ⌈min_perbutation + (E_current / E_begin) * (max_perbutation - min_perbutation)⌉
        // Perturbation = ⌈1 + (E_board / E_begin) * (n / 5.0 - 1)⌉
        // Ceil so that Perturbation < 1 will always result in at least one change
        for (int i = 0; i < ceil(1 + (static_cast<double>(E_board) / E_begin) * (n / 10.0 - 1)); ++i) {
          const int row = nGen(gen);
          // Ignore mandatory queen row
          if (row == queenPos.first) {
            continue;
          }
          // Perturbation
          b[row] = nGen(gen);
        }
      }
      // Take new sampleOne for next stagnation check
      stagnationSampleOne = E_board;
    }

    // STEP 3: for a random highestConflictRow, find the lowest energy location to move that queen to on the same row
    conflictGen.param(std::uniform_int_distribution<>::param_type(0, static_cast<int>(highestConflictRows.size()) - 1));
    const int row = highestConflictRows[conflictGen(gen)];

    // Evaluate all columns in the selected row to find the best positions
    int E_min = n + 1;
    Board bestCols;

    for (int col = 0; col < n; ++col) {
      b[row] = col; // Tentatively place queen
      int E_current = -1;

      // Calculate E for the tentative placement
      for (int i = 0; i < n; ++i) {
        if (const int b_i = b[i]; b_i == col || std::abs(b_i - col) == std::abs(i - row)) {
          E_current++;
        }
      }

      // Update the best columns if this placement is as good as the current best
      if (E_current < E_min) {
        E_min = E_current;
        bestCols.clear();
        bestCols.push_back(col);
      } else if (E_current == E_min) {
        bestCols.push_back(col);
      }
    }

    // Randomly select one of the best columns
    bestColRand.param(std::uniform_int_distribution<>::param_type(0, static_cast<int>(bestCols.size()) - 1));
    b[row] = bestCols[bestColRand(gen)];
  }
  // Unreachable but I like it so it can stay
  return move(b);
}


std::string formatBoard(const Board& b, const int n, const std::string& delimiter) {
  std::string res;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (b[j] == i) {
        res += "Q" + delimiter;
        continue;
      }
      res += "." + delimiter;
    }
    res += "\n";
  }
  return move(res);
}


std::string solveNQueens(const int n, const std::pair<int, int>& mandatoryQueenCoordinates) {
  std::string res;

  // Base cases {n ∈ ℤ | n < 4}
  if (n == 1) return "Q\n"; // Trivial solution
  if (n < 4) return move(res); // No solution

  const Board b = minConflicts(n, mandatoryQueenCoordinates);
  if (b.empty()) return res;

  return formatBoard(b, n, "");
}

std::string solveNQueensPretty(const int n, const std::pair<int, int>& mandatoryQueenCoordinates) {
  std::string res;

  // Base cases {n ∈ ℤ | n < 4}
  if (n == 1) return "Q\n"; // Trivial solution
  if (n < 4) return move(res); // No solution

  const Board b = minConflicts(n, mandatoryQueenCoordinates);
  if (b.empty()) return res;

  return formatBoard(b, n, " ");
}
}


// int main() {
//   using namespace nQueens;
//   constexpr int n = 10000;
//
//   // Start time measurement
//   const auto start = std::chrono::high_resolution_clock::now();
//
//   const std::string res = solveNQueensPretty(n, {1, 0});
//
//   // End time measurement
//   const auto end = std::chrono::high_resolution_clock::now();
//   const std::chrono::duration<double> duration = end - start;
//
//   // Output the result and the time taken
//   std::cout << res << std::endl;
//   std::cout << "Time elapsed: " << duration.count() << " seconds" << std::endl;
//
//   return 0;
// }
