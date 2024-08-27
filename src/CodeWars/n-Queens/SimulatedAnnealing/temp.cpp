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
  const int n = static_cast<int>(b.size())-1;
  int E = -1;
  for (int i = 0; i < n; ++i) {
    // Dont count i == j
    for (int j = 0; j < n; ++j) {
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


long lastIterationCount;

Board simulatedAnnealing(const int n, const std::pair<int, int>& queenPos) {
  double T = n * 10;
  constexpr double T_min = 0.01;
  constexpr double alpha = 0.95;

  Board b = generateBoard(n, queenPos);
  auto [qx, qy] = queenPos;
  std::uniform_real_distribution<> randZeroOne(0, 1); // Uniform double distribution [0; 1]
  std::uniform_int_distribution<> nRand(0, n - 1); // Uniform int distribution [0; 1]
  int E_boardCurrent = 0;
  int E_boardOld = calculateE(b);
  while (T > T_min) {
    if (E_boardOld == 0) {
      break; // Early exit if a solution is found
    }
    std::cout << "E = " << E_boardOld << std::endl;

    int row = 0;
    while (true) {
      row = nRand(gen);
      if (row != qx) {
        break;
      }
    }


    int colValOld = b[row];
    b[row] = nRand(gen);
    E_boardCurrent = calculateE(b);
    const int deltaE = E_boardCurrent - E_boardOld;
    if (deltaE < 0 || exp(-deltaE / T) > randZeroOne(gen)) {
      // Accept solution
      E_boardOld = E_boardCurrent; // Update the old energy to the new energy
    } else {
      // Decline solution
      b[row] = colValOld;
    }

    T *= alpha;
  }
  return b;
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
        res += "Q ";
        continue;
      }
      res += ". ";
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
  for (i = 0; i < epochs; ++i) {
    solveNQueens(n, mandatoryQueenCoordinates);
    total += lastIterationCount;
  }
  // End time
  const auto end = std::chrono::high_resolution_clock::now();
  // Calculate duration in milliseconds
  const std::chrono::duration<double, std::milli> duration = end - start;

  std::cout << "Iterations on average (n = " << n << ", " << epochs << " epochs) = " << total / i << std::endl;
  std::cout << "Time elapsed: " << duration.count() << " ms" << std::endl;
}
}


int main() {
  using namespace nQueens;
  // measurePerformance(6, {2, 1}, 1);

  std::cout << solveNQueens(6, {2, 1});
  return 0;
}
