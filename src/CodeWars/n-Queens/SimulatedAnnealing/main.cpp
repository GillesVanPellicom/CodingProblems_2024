#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <random>
#include <set>
#include <chrono>
#include <string>
// #include "../../utils/ArrayUtils.cpp"

// Board datatype. Index = row, value = col.
// Only one piece per row possible so no need for 2D array.
namespace nQueens {
using Board = std::vector<int>;
std::mt19937 gen(std::random_device{}()); // Mersenne Twister random
int _n = -1;


/**
 * Calculates the energy (E) of a specified board.
 * The higher E, the more conflicts.
 * A solved board is E = 0
 * @param b Board from which to calculate E
 * @return energy as 𝕫⁺
 */
int calculateE(const Board& b) {
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
  auto [mx, my] = mandatoryQueenCoordinates;
  // For each row, choose a pseudo-random col
  for (int i = 0; i < n; ++i) {
    res.emplace_back(rand(gen));
  }
  // Ensure mandatory queen is correct
  res[mx] = my;
  return res;
}


/**
 * Generates a neighbor of the given board.
 *
 * @param current Current board
 * @param mandatoryQueenCoordinates
 * @return Neighbor board
 */
Board generateNeighbor(const Board& current, const std::pair<int, int>& mandatoryQueenCoordinates) {
  std::uniform_int_distribution<> rowGen(0, _n - 1); // Uniform integer distribution [0; n-1]
  std::uniform_int_distribution<> colGen(0, _n - 1); // Uniform integer distribution [0; n-1]

  Board neighbor = current;
  auto [mx, my] = mandatoryQueenCoordinates;

  int newRow = rowGen(gen);
  int newCol = colGen(gen);
  if (newRow == mx) {
    if (newRow < _n-1) {
      newRow++;
    } else {
      newCol = my;
    }
  }
  neighbor[newRow] = newCol; // Change column of the chosen row



  return neighbor;
}


long lastIterationCount;

Board simulatedAnnealing(const int n, const std::pair<int, int>& queenPos) {
  _n = n;
  std::uniform_real_distribution<> randZeroOne(0, 1); // Uniform double distribution [0; 1]
  // Set starting values
  Board b_best = generateBoard(n, queenPos);
  int E_best = calculateE(b_best);

  Board b_current = b_best;
  int E_current = E_best;

  std::set<Board> tabuList;

  long iterationCount = 0;

  // Annealing variables
  double T = 100*n; // Temperature T, controls likelyhood of worse solutions being accepted.
  constexpr double alpha = 0.95; // Cooling rate α, rate of cooling of T each iteration.
  constexpr double T_min = 0.00; // T_min, annealing will stop when reaching this temperature.
  const long iteration_max = 10 * pow(2, n) * log(n); // IterationMax = ?, annealing failed when reached.

  while (T > T_min) {

    if (iterationCount > iteration_max) {
      // No valid solution has been found
      return std::vector<int>{};
    }

    Board b_neighbor = generateNeighbor(b_current, queenPos);
    const int E_neighbor = calculateE(b_neighbor);

    const int deltaE = E_neighbor - E_current; // ΔE = E_current - E_neighbor

    // ΔE is negative ⟹ solution is better.
    // ΔE is positive ⟹ compute probability of accepting worse solution based on temperature
    // Probability = e^(-ΔE/T) > x, x ∈ [0; 1]
    if (deltaE < 0 || exp(-deltaE / T) > randZeroOne(gen)) {
      b_current = b_neighbor;
      E_current = E_neighbor;

      // If there are less conflicts compared to the best solution, new best found
      if (E_current < E_best) {
        b_best = b_current;
        E_best = E_current;
      }

      // If best solution has zero energy, solution is correct. Stop annealing.
      if (E_best == 0) {
        // Solution found
        std::cout << "max = " << iteration_max << std::endl;
        std::cout << "iteration = " << iterationCount << std::endl;
        std::cout << "E_final = " << E_best << std::endl;
        lastIterationCount = iterationCount;
        return b_best;
      }
    }
    T *= alpha; // Cool temperature

    iterationCount++;
    std::cout << "E = " << E_best << std::endl;
  }
  std::cout << "E_final = " << E_best << std::endl;

  return b_best;
}


std::string solveNQueens(int n, std::pair<int, int> mandatoryQueenCoordinates) {
  const Board b = simulatedAnnealing(n, mandatoryQueenCoordinates);
  std::string res;

  if (b.empty()) {
    return res;
  }

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (b[j] == i) {
        res += 'Q';
        continue;
      }
      res += '.';
    }
    res += "\n";
  }
  return res;
}


void measurePerformance(const int n, const std::pair<int, int>& mandatoryQueenCoordinates, int epochs) {
  long total = 0;
  int i;
  // Start time
  const auto start = std::chrono::high_resolution_clock::now();
  for (i = 0; i < epochs+1; ++i) {
    solveNQueens(n, mandatoryQueenCoordinates);
    total += lastIterationCount;
  }
  // End time
  const auto end = std::chrono::high_resolution_clock::now();
  // Calculate duration in milliseconds
  const std::chrono::duration<double, std::milli> duration = end - start;

  std::cout << "Iterations on average (n = " << n << ", " << epochs << " epochs) = " << total/i << std::endl;
  std::cout << "Time elapsed: " << duration.count() << " ms" << std::endl;

}
}


int main() {
  using namespace nQueens;
  measurePerformance(200, {0, 1}, 10);

  // std::cout << solveNQueens(100, {0, 1});
  return 0;
}