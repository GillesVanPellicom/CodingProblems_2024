#include <gtest/gtest.h>
#include <vector>
#include <set>
#include <string>
#include <utility>
#include <algorithm>
#include <random>
#include <iomanip>

#include "main.cpp"

constexpr int UNINITIALISED = -1;

int rint(const int limit) {
  static std::default_random_engine rdEngine(std::random_device{}());
  std::uniform_int_distribution<int> nDistribution(0, limit - 1);
  return nDistribution(rdEngine);
}

bool boardHasCorrectSize(const std::string& board, const int n) {
  return static_cast<int>(board.size()) == n * n + n;
}

std::pair<std::string, std::vector<int> > parse(const std::string& board, const int n) {
  int numQueens = 0;
  int stringOffset = 0;
  std::vector<int> queenCoords(n, UNINITIALISED);

  for (int y = 0; y < n; ++y) {
    for (int x = 0; x < n; ++x) {
      if (const char field = board[stringOffset++]; field == 'Q') {
        if (queenCoords[y] != UNINITIALISED)
          return {"Board with horizontal attacks between Queens", {}};

        numQueens++;
        queenCoords[y] = x;
      } else if (field != '.') {
        return {"Board with invalid characters", {}};
      }
    }
    if (board[stringOffset++] != '\n')
      return {"Board with missing/incorrect characters", {}};
  }
  if (numQueens != n)
    return {"Board with wrong number of Queens", {}};

  return {"Board with correct characters", queenCoords};
}

std::string noAttacks(const std::vector<int>& queenCoords) {
  const size_t n = queenCoords.size();
  std::vector<bool> takenCols(n);
  std::vector<bool> takenDiag1(n * 2), takenDiag2(n * 2);

  for (int row = 0; row < n; ++row) {
    const int col = queenCoords[row];
    if (takenCols[col]) return "Board with vertical attacks between Queens";
    takenCols[col] = true;
    if (takenDiag1[col + row]) return "Board with diag1 attacks between Queens";
    takenDiag1[col + row] = true;
    if (takenDiag2[col - row + n - 1]) return "Board with diag2 attacks between Queens";
    takenDiag2[col - row + n - 1] = true;
  }
  return "Board with no attacks between Queens";
}

std::string mandatoryQueenInCorrectPosition(const std::vector<int>& queenCoords,
                                            const std::pair<int, int>& mandatoryCoordinates) {
  if (queenCoords[mandatoryCoordinates.second] == mandatoryCoordinates.first)
    return "Mandatory queen in required position";
  return "Mandatory queen not in required position";
}

void checkBoard(const std::string& board, const int size, const std::pair<int, int>& mandatoryCoordinates) {
  EXPECT_EQ(boardHasCorrectSize(board, size), true) << "Board string has incorrect size";

  auto [fst, snd] = parse(board, size);
  EXPECT_EQ(fst, "Board with correct characters") << fst;

  const auto queens = snd;
  EXPECT_EQ(noAttacks(queens), "Board with no attacks between Queens");
  EXPECT_EQ(mandatoryQueenInCorrectPosition(queens, mandatoryCoordinates), "Mandatory queen in required position");
}

void testSolution(const int n, const std::pair<int, int>& mandatoryCoordinates) {
  const std::string& board = nQueens::solveNQueens(n, mandatoryCoordinates);
  checkBoard(board, n, mandatoryCoordinates);
}

void testNoSolution(const int n, const std::pair<int, int>& mandatoryCoordinates) {
  const std::string& board = nQueens::solveNQueens(n, mandatoryCoordinates);
  EXPECT_EQ(board.empty(), true) << "Expected empty solution string, but got: " << board;
}

class NQueensTest : public ::testing::Test {
};

TEST_F(NQueensTest, BasicTests) {
  testSolution(4, {2, 0});
  testSolution(8, {3, 0});
  testSolution(1, {0, 0});
}

TEST_F(NQueensTest, NoSolution) {
  testNoSolution(2, {0, 0});
  testNoSolution(3, {2, 0});
  testNoSolution(6, {0, 0});
}

TEST_F(NQueensTest, RandomTestsSmallN) {
  std::default_random_engine rdEngine(std::random_device{}());
  std::vector<std::string> noSolutions = {
    "200", "201", "210", "211", "300", "301", "302", "310", "311", "312",
    "320", "321", "322", "400", "403", "411", "412", "421", "422",
    "430", "433", "600", "605", "611", "614", "622", "623", "632",
    "633", "641", "644", "650", "655"
  };
  std::set<std::string> noSolutionsSet(noSolutions.begin(), noSolutions.end());
  std::shuffle(noSolutions.begin(), noSolutions.end(), rdEngine);

  std::string specs = std::string(5, '+') + std::string(5, '-');
  std::shuffle(specs.begin(), specs.end(), rdEngine);

  for (const char spec : specs) {
    if (spec == '-') {
      auto testCaseDescription = noSolutions.back();
      noSolutions.pop_back();
      const int size = testCaseDescription[0] - '0';
      const int col = testCaseDescription[1] - '0';
      const int row = testCaseDescription[2] - '0';
      testNoSolution(size, {col, row});
    } else {
      int size = 0, row, col;
      std::string input;

      while (input.empty()) {
        size = rint(7) + 4;
        row = rint(size);
        col = rint(size);
        input = "---";
        input[0] = static_cast<char>(size + '0');
        input[1] = static_cast<char>(col + '0');
        input[2] = static_cast<char>(row + '0');
        if (noSolutionsSet.find(input) != noSolutionsSet.end()) {
          input = "";
        }
      }
      testSolution(size, {col, row});
    }
  }
}

TEST_F(NQueensTest, RandomTestsLargerN) {
  testSolution(rint(10) + 10, {rint(10), rint(10)});
  testSolution(rint(5) + 15, {rint(15), rint(15)});
  testSolution(rint(5) + 20, {rint(20), rint(20)});
}

TEST_F(NQueensTest, RandomTestsLargeN) {
  testSolution(rint(50) + 100, {rint(100), rint(100)});
  testSolution(rint(50) + 200, {rint(200), rint(200)});
  testSolution(rint(50) + 300, {rint(300), rint(300)});
}

TEST_F(NQueensTest, RandomTestsEvenLargerN) {
  testSolution(rint(100) + 500, {rint(500), rint(500)});
  testSolution(rint(100) + 650, {rint(650), rint(650)});
  testSolution(rint(100) + 750, {rint(750), rint(750)});
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
