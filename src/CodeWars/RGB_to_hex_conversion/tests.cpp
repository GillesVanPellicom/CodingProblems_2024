#include <gtest/gtest.h>
#include <string>
#include "main.cpp"

// Assuming rgb_to_hex is declared somewhere, e.g.:
// std::string rgb_to_hex(int r, int g, int b);

TEST(BasicTests, BasicConversionsOne) {
  EXPECT_EQ(rgb_to_hex(0, 0, 0), "000000") << "Expected: RGB(0, 0, 0) should be #000000";

}

TEST(BasicTests, BasicConversionsTwo) {
  EXPECT_EQ(rgb_to_hex(1, 2, 3), "010203") << "Expected: RGB(1, 2, 3) should be #010203";

}

TEST(BasicTests, BasicConversionsThree) {
  EXPECT_EQ(rgb_to_hex(255, 255, 255), "FFFFFF") << "Expected: RGB(255, 255, 255) should be #FFFFFF";

}

TEST(BasicTests, BasicConversionsFour) {
  EXPECT_EQ(rgb_to_hex(254, 253, 252), "FEFDFC") << "Expected: RGB(254, 253, 252) should be #FEFDFC";
}

TEST(BasicTests, BasicConversionsFive) {
  EXPECT_EQ(rgb_to_hex(-20, 275, 125), "00FF7D") << "Expected: RGB(-20, 275, 125) should be #00FF7D";

}

// Entry point for the test program
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
