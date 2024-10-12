#include <gtest/gtest.h>
#include "main.cpp"


/**
 * Generated by ChatGPT
 */
class InterpreterTest : public ::testing::Test {
protected:
    // Optional: Setup and teardown methods if needed
    void SetUp() override {
        // Reset interpreter state if necessary
    }

    void TearDown() override {
        // Cleanup after tests if necessary
    }
};

// Tests for basic arithmetic operations
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

// Tests for operator precedence
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

// Tests for variable assignment and usage
TEST(VariableAssignmentTests, BasicAssignment) {
    EXPECT_EQ(interpret("x = 12"), 12);
    EXPECT_EQ(interpret("x"), 12);
    EXPECT_EQ(interpret("x + 8"), 20);
}

// Test valid variable usage after assignment
TEST_F(InterpreterTest, VariableUsageAfterAssignment) {
    interpret("a = 15");
    EXPECT_DOUBLE_EQ(interpret("a + 5"), 20);
}

// Test error handling for non-existent variable
TEST_F(InterpreterTest, NonExistentVariable) {
    EXPECT_THROW(interpret("b + 12"), VariableExpansionError);
    interpret("a = 20");
    EXPECT_THROW(interpret("a + b + 1"), VariableExpansionError);
}

// Test variable reassignment
TEST_F(InterpreterTest, VariableReassignment) {
    interpret("c = 5");
    EXPECT_DOUBLE_EQ(interpret("c"), 5);
    interpret("c = 10");
    EXPECT_DOUBLE_EQ(interpret("c"), 10);
}

// Test multiple operations
TEST_F(InterpreterTest, MultipleOperations) {
    interpret("x = 10");
    interpret("y = 2");
    EXPECT_DOUBLE_EQ(interpret("x * y + 5"), 25);
    EXPECT_DOUBLE_EQ(interpret("x / y"), 5);
    EXPECT_DOUBLE_EQ(interpret("x - y"), 8);
}

// Test operator precedence
TEST_F(InterpreterTest, OperatorPrecedence) {
    interpret("x = 10");
    interpret("y = 5");
    EXPECT_DOUBLE_EQ(interpret("x + y * 2"), 20);
    EXPECT_DOUBLE_EQ(interpret("(x + y) * 2"), 30);
}

// Test handling of whitespace
TEST_F(InterpreterTest, WhitespaceHandling) {
    EXPECT_DOUBLE_EQ(interpret("  x   =  10   "), 10);
    EXPECT_DOUBLE_EQ(interpret("   x +   5 "), 15);
}

// Test chained assignments (though the spec states not to consider it)
TEST_F(InterpreterTest, ChainedAssignment) {
    interpret("x = 5");
    interpret("y = 2");
    EXPECT_DOUBLE_EQ(interpret("z = x + y"), 7);
    EXPECT_DOUBLE_EQ(interpret("z"), 7);
}

// Test edge case: empty inputs
TEST_F(InterpreterTest, EmptyInput) {
    EXPECT_THROW(interpret(""), InvalidInputError);
    EXPECT_THROW(interpret(" "), InvalidInputError);
    EXPECT_THROW(interpret("  "), InvalidInputError);
}

// Test handling of undefined variables in expressions
TEST_F(InterpreterTest, UndefinedVariablesInExpression) {
    EXPECT_THROW(interpret("qe + be"), VariableExpansionError);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
