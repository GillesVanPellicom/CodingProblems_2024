#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <random>
#include <set>
#include <chrono>
#include <string>
// #include "../../utils/ArrayUtils.cpp"


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
int calculateE(const Board& b, const int rowPos) {
  int E = 0;
  for (int i = 0; i < _n; ++i) {
    // if (not in same col) or (not in same diagonal)
    // row doesn't need to be checked since the code doesn't allow two queens to generate in one row.
    if (b[rowPos] == b[i] || std::abs(b[rowPos] - b[i]) == std::abs(rowPos - i)) {
      ++E;
    }
  }
  return E-1;
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

long iterationCount;


Board minConflicts(const int n, const std::pair<int, int>& queenPos) {
  _n = n;
  Board b = generateBoard(n, queenPos);
  while (true) {

    iterationCount++;
    int E_highest = 0;
    std::vector<int> highestConflictRows;

    for (int i = 0; i < n; ++i) {
      int E_current = calculateE(b, i);
      if (E_current > E_highest) {
        E_highest = E_current;
        highestConflictRows.clear();
        highestConflictRows.push_back(i);
      } else if (E_current == E_highest) {
        highestConflictRows.push_back(i);
      }
    }

    if (E_highest == 0) {
      // done
      return b;
    }

    std::uniform_int_distribution<> rowGen(0, static_cast<int>(highestConflictRows.size()) - 1);
    if (const int row = highestConflictRows[rowGen(gen)];
      row == queenPos.second) {
      b[row] = queenPos.first;
    } else {
      std::uniform_int_distribution<> colGen(0, _n - 1); // Uniform integer distribution [0; n-1]
      b[row] = colGen(gen);
    }
    std::cout << "E = " << E_highest << std::endl;

  }
  return b;
}

void printBoard(Board b) {
  std::string res;
  int n = b.size();
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


std::string solveNQueens(int n, std::pair<int, int> mandatoryQueenCoordinates) {
  const Board b = minConflicts(n, mandatoryQueenCoordinates);
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
  for (i = 0; i < epochs+1; ++i) {
    solveNQueens(n, mandatoryQueenCoordinates);
    total += iterationCount;
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
  int n = 80;
  // // measurePerformance(50, {0, 1}, 20);
  // Board b = minConflicts(n, {0, 1});
  // for (int i = 0; i < n; ++i) {
  //   std::cout << calculateE(b, i) << " ";
  // }
  // std::cout << std::endl;
  // printBoard(b);
      measurePerformance(n, {0, 3}, 1);
  // std::cout << solveNQueens(n, {0, 3});
  return 0;
}
