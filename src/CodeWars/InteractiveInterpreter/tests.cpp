#include <gtest/gtest.h>
#include "main.cpp"


class InterpreterTest : public ::testing::Test {
protected:
    // Optional: Setup and teardown methods if needed
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST(ArithmeticTests, BasicAddition) {
    EXPECT_EQ(interpret("5 + 5"), 10);
    EXPECT_EQ(interpret("5+ 5"), 10);
    EXPECT_EQ(interpret("5 +5"), 10);
    EXPECT_EQ(interpret("5+5"), 10);
}

TEST(ArithmeticTests, BasicSubtraction) {
    EXPECT_EQ(interpret("5 - 2"), 3);
    EXPECT_EQ(interpret("5- 2"), 3);
    EXPECT_EQ(interpret("5 -2"), 3);
    EXPECT_EQ(interpret("5-2"), 3);
}

TEST(ArithmeticTests, BasicMultiplication) {
    EXPECT_EQ(interpret("8 * 2"), 16);
    EXPECT_EQ(interpret("8* 2"), 16);
    EXPECT_EQ(interpret("8 *2"), 16);
    EXPECT_EQ(interpret("8*2"), 16);
}

TEST(ArithmeticTests, BasicDivision) {
    EXPECT_EQ(interpret("8 / 2"), 4);
    EXPECT_EQ(interpret("8/ 2"), 4);
    EXPECT_EQ(interpret("8 /2"), 4);
    EXPECT_EQ(interpret("8/2"), 4);
}

TEST(ArithmeticTests, BasicModulo) {
    EXPECT_EQ(interpret("13 % 6"), 1);
    EXPECT_EQ(interpret("13% 6"), 1);
    EXPECT_EQ(interpret("13 %6"), 1);
    EXPECT_EQ(interpret("13%6"), 1);
}

TEST(ArithmeticTests, BasicPower) {
    EXPECT_EQ(interpret("2 ^ 2"), 4);
    EXPECT_EQ(interpret("2^ 2"), 4);
    EXPECT_EQ(interpret("2 ^2"), 4);
    EXPECT_EQ(interpret("2^2"), 4);
}



TEST(OperatorPrecedenceTests, BasicPrecedence) {
    EXPECT_EQ(interpret("4 + 2 * 3"), 10);
    EXPECT_EQ(interpret("4 / 2 * 3"), 6);
    EXPECT_EQ(interpret("7 % 2 * 8"), 8);
}

TEST(OperatorPrecedenceTests, ParenthesesPrecedence) {
    EXPECT_EQ(interpret("(5 + 5) * 2"), 20);
    EXPECT_EQ(interpret("(90 + 10) / (1 + 3 * 3)"), 10);
}

TEST(OperatorPrecedenceTests, NestedParenthesesPrecedence) {
    EXPECT_EQ(interpret("(8 - (4 + 2)) * 3"), 6);
    EXPECT_EQ(interpret("(10 / (8 - (4 + 2))) * 3"), 15);
}



TEST(VariableTests, BasicVariableAssignment) {
    EXPECT_EQ(interpret("x = 12"), 12);
    EXPECT_EQ(interpret("x"), 12);
    EXPECT_EQ(interpret("x + 8"), 20);
}

TEST(VariableTests, BasicVariableUsage) {
    interpret("a = 15");
    EXPECT_DOUBLE_EQ(interpret("a + 5"), 20);
}

TEST(VariableTests, NestedVariable) {
    interpret("x = 5");
    interpret("y = 2");
    EXPECT_DOUBLE_EQ(interpret("z = x + y"), 7);
    EXPECT_DOUBLE_EQ(interpret("z"), 7);
}

TEST(VariableTests, NonExistentVariable) {
    EXPECT_THROW(interpret("b + 12"), VariableExpansionError);
    interpret("a = 20");
    EXPECT_THROW(interpret("a + b + 1"), VariableExpansionError);
}

TEST(VariableTests, VariableReassignment) {
    interpret("c = 5");
    EXPECT_DOUBLE_EQ(interpret("c"), 5);
    interpret("c = 10");
    EXPECT_DOUBLE_EQ(interpret("c"), 10);
}



TEST(OperationsTest, MixedOperations) {
    interpret("x = 10");
    interpret("y = 2");
    EXPECT_DOUBLE_EQ(interpret("x * y + 5"), 25);
    EXPECT_DOUBLE_EQ(interpret("x / y"), 5);
    EXPECT_DOUBLE_EQ(interpret("x - y"), 8);
}



TEST(EdgeTest, WhitespaceHandling) {
    EXPECT_DOUBLE_EQ(interpret("  x   =  10   "), 10);
    EXPECT_DOUBLE_EQ(interpret("   x +   5 "), 15);
}

TEST(EdgeTest, EmptyInput) {
    EXPECT_THROW(interpret(""), InvalidInputError);
    EXPECT_THROW(interpret(" "), InvalidInputError);
    EXPECT_THROW(interpret("  "), InvalidInputError);
}



TEST(MonolithicTest, FullTest) {
    interpret("x = 8 / 4");
    interpret("y = 3 + 4 * 2 - x ^ 2");
    EXPECT_DOUBLE_EQ(interpret("y + (15 % 4) - (-3) + 2^3 - 5 * 2 + 7 * (6 / 3)"), 25);
}



int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
