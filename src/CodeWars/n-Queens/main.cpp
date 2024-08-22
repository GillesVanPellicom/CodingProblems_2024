#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <random>
#include <set>
#include <string>
// #include "../../utils/ArrayUtils.cpp"

// Board datatype. Index = row, value = col.
// Only one piece per row possible so no need for 2D array.
namespace nQueens {
using Board = std::vector<int>;
std::mt19937 gen(std::random_device{}()); // Mersenne Twister random


/**
 * Calculates the energy (E) of a specified board.
 * The higher E, the more conflicts.
 * A solved board is E = 0
 * @param b Board from which to calculate E
 * @return energy as 𝕫⁺
 */
int calculateE(const Board& b) {
  const int n = static_cast<int>(b.size());
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
  const int n = static_cast<int>(current.size());

  std::uniform_int_distribution<> rowGen(0, n - 1); // Uniform integer distribution [0; n-1]
  std::uniform_int_distribution<> colGen(0, n - 1); // Uniform integer distribution [0; n-1]

  Board neighbor = current;
  auto [mx, my] = mandatoryQueenCoordinates;

  int newRow = rowGen(gen);
  // Ensure that we are not modifying the row of the mandatory queen
  while (newRow == mx) {
    newRow = rowGen(gen);
  }
  // Generate a new column for the chosen row
  const int newCol = colGen(gen);

  neighbor[newRow] = newCol; // Change column of the chosen row

  return neighbor;
}

Board simulatedAnnealing(const int n, const std::pair<int, int>& queenPos) {
  std::uniform_real_distribution<> randZeroOne(0, 1); // Uniform double distribution [0; 1]
  // Set starting values
  Board b_best = generateBoard(n, queenPos);
  int E_best = calculateE(b_best);

  Board b_current = b_best;
  int E_current = E_best;

  std::set<Board> tabuList;

  long iterationCount = 0;

  // Annealing variables
  double T = 10*n; // Temperature T, controls likelyhood of worse solutions being accepted.
  constexpr double alpha = 0.8; // Cooling rate α, rate of cooling of T each iteration.
  constexpr double T_min = 0.0; // T_min, annealing will stop when reaching this temperature.
  const long iteration_max = 5 * pow(2, n) * log(n); // IterationMax = ?, annealing failed when reached.
  const long tabu_listSize = n*2;

  while (T > T_min) {

    if (iterationCount > iteration_max) {
      // No valid solution has been found
      return std::vector<int>{};
    }

    Board b_neighbor;
    int E_neighbor;
    // Generate valid neighbor using tabu list
    while (true) {
      // Neighbor must not have been visited before
      b_neighbor = generateNeighbor(b_current, queenPos);
      if (!tabuList.contains(b_neighbor)) {
        // If neighbor is unique, calculate E
        E_neighbor = calculateE(b_neighbor);
        break;
      }
    }

    int deltaE = E_neighbor - E_current; // ΔE = E_current - E_neighbor

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

      // Update tabu list
      if (tabuList.size() >= tabu_listSize) {
        tabuList.erase(tabuList.begin()); // Remove oldest entry
      }
      tabuList.insert(b_current);

      // If best solution has zero energy, solution is correct. Stop annealing.
      if (E_best == 0) {
        // Solution found
        std::cout << "max = " << iteration_max << std::endl;
        std::cout << "iteration = " << iterationCount << std::endl;
        std::cout << "E_final = " << E_best << std::endl;
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
}

int main() {
  using namespace nQueens;
  // Board b = {2, 0, 3, 1};
  // Board b = simulatedAnnealing(4, {0, 0});
  // printArray(b);
  std::cout << solveNQueens(4, {0, 1});
  // std::cout << "E = " << calculateE(b) << std::endl;
  // printBoard(b);
  // std::cout << solveNQueens(8, {3, 0});
  return 0;
}