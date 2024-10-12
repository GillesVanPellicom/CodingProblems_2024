// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Project      : CodeWars - Simpler Interactive Interpreter                        ║
// ║ Version      : v0.1.1 "final 2ky version"                                        ║
// ║ File         : main.cpp                                                          ║
// ║ Author(s)    : Gilles Van pellicom                                               ║
// ║ Date         : 2024/10/12                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#include <algorithm>
#include <vector>
#include <stack>
#include <unordered_map>
#include <regex>
#include <cmath>
#include <string>
#include <iostream>

// ╔════════════════════════════════════════╗
// ║                 Errors                 ║
// ╚════════════════════════════════════════╝
/**
 * Error thrown when variable expansion fails
 */
class VariableExpansionError final : public std::runtime_error {
  public:
    explicit VariableExpansionError(const std::string& message)
      : std::runtime_error(message) {
    }
};


/**
 * Error thrown when input is invalid
 */
class InvalidInputError final : public std::runtime_error {
  public:
    explicit InvalidInputError(const std::string& message)
      : std::runtime_error(message) {
    }
};


// ╔════════════════════════════════════════╗
// ║               Datatypes                ║
// ╚════════════════════════════════════════╝

/**
 * Lexeme type enumeration for type safety
 */
enum class Type {
  NUMBER,   // "[0-9]+(\\.[0-9]+)?" E.g. 12, 3.14, 9999
  PLUS,     // "+" E.g. a + b
  MIN,      // "-" E.g. a - b
  MUL,      // "*" E.g. a * b
  DIV,      // "/" E.g. a / b
  POW,      // "^" E.g. a ^ b
  MOD,      // "%" E.g. a % b
  UMIN,     // "-" E.g. -b (Unary minus)
  LBRAC,    // "(" E.g. (12)
  RBRAC     // ")" E.g. (12)
};


// Input-defined variables
std::unordered_map<std::string, std::string> variableDefinitions;


// Lexer definitions
std::unordered_map<Type, std::string> tokenDefinitions = {
  { Type::NUMBER, R"(([0-9]+(\.[0-9]+)?))" },
  { Type::PLUS,   R"(\+)" },
  { Type::MIN,    R"(\-)" },
  { Type::MUL,    R"(\*)" },
  { Type::DIV,    R"(\/)" },
  { Type::POW,    R"(\^)" },
  { Type::MOD,    R"(\%)" },
  { Type::LBRAC,  R"(\()" },
  { Type::RBRAC,  R"(\))" }
};


/**
 * Lexeme object used by lexer
 */
struct Lexeme {
  Type type;
  double value;

  explicit Lexeme(const Type type, const double value = -1) : type(type), value(value) {
  }
};

// ╔════════════════════════════════════════╗
// ║            Helper functions            ║
// ╚════════════════════════════════════════╝

/**
 * Applies the correct operation two operands.
 * (A operation B)
 * @param operatorType type of operator Type.
 * @param operandA operand A
 * @param operandB operand B
 * @return Lexeme solution
 */
static Lexeme operate(const Type operatorType, const double operandA, const double operandB) {
  switch (operatorType) {
    case Type::PLUS: return Lexeme(Type::NUMBER, operandA + operandB);
    case Type::MIN: return Lexeme(Type::NUMBER, operandA - operandB);
    case Type::MUL: return Lexeme(Type::NUMBER, operandA * operandB);
    case Type::DIV: return Lexeme(Type::NUMBER, operandA / operandB);
    case Type::POW: return Lexeme(Type::NUMBER, std::pow(operandA, operandB));
    case Type::MOD: return Lexeme(Type::NUMBER, std::fmod(operandA, operandB));
    default: return Lexeme(Type::NUMBER);
  }
}


/**
 * Return true if type is operator
 * @param type Type to be checked
 * @return true if is operator
 */
static bool isOperator(const Type type) {
  return type != Type::NUMBER;
}


/**
 * Give precedence value based on Type
 * @param operatorType Type to be checked
 * @return precedence level as int
 */
static int getPrecedence(const Type operatorType) {
  switch (operatorType) {
    case Type::PLUS:
      [[fallthrough]];
    case Type::MIN:
      return 1;

    case Type::MUL:
      [[fallthrough]];
    case Type::DIV:
      [[fallthrough]];
    case Type::MOD:
      return 2;

    case Type::POW:
      [[fallthrough]];
    case Type::UMIN:
      return 3;
    default:
      return 0;
  }
}


/**
 * Basic space trim function
 * @param str input
 * @return trimmed string
 */
std::string trim(const std::string& str) {
  // No std::ranges since code is written for c++17
  auto left = std::find_if_not(str.begin(), str.end(), ::isspace);
  auto right = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();
  return {left, right};
}


// ╔════════════════════════════════════════╗
// ║            Evaluation Logic            ║
// ╚════════════════════════════════════════╝

/**
 * Function used to define a variable for later use
 * @param expression variable definition
 * @return definition value
 */
std::string defineVariable(const std::string& expression) {
  const size_t pos = expression.find('=');
  const std::string key = trim(expression.substr(0, pos - 1));
  const std::string value = '(' + expression.substr(pos + 1) + " )";

  variableDefinitions[key] = value;
  return value;
}


/**
 * Expands all variables in an expression using known definitions
 * @param expression string to be expanded
 * @return expanded expression
 */
std::string variableExpansion(const std::string& expression) {
  const std::regex pattern(R"([a-zA-Z_][a-zA-Z0-9_]*)"); // Match valid variable names
  std::string res = expression;
  std::smatch match; // Variable to store the matched pattern

  // Continue processing while the string contains the pattern
  while (std::regex_search(res, match, pattern)) {
    // Check if the variable exists in definitions
    // No contains() since code is written for c++17
    if (std::string varName = match[0]; variableDefinitions.find(varName) != variableDefinitions.end()) {
      // Replace variable with its value
      res = std::regex_replace(res, std::regex(varName), variableDefinitions[varName]);
    } else {
      // If variable is undefined, throw an error or handle it as needed
      throw VariableExpansionError("ERROR: Invalid identifier. No variable with name '" + varName
        + "' was found.");
    }
  }

  return res; // Return the modified string instead of the original expression
}


/**
 * Lexical analasys function.
 * Splits expression up into lexemes
 * @param input expression as string
 * @return expression as lexemes, INFIX
 */
std::vector<Lexeme> lexer(const std::string& input) {
  std::vector<Lexeme> lexemes;
  std::regex pattern("([0-9]+(\\.[0-9]+)?)|[-+*/()^%]");
  auto tokens_begin = std::sregex_iterator(input.begin(), input.end(), pattern);
  auto tokens_end = std::sregex_iterator();

  std::vector<std::string> tokens;
  for (std::sregex_iterator i = tokens_begin; i != tokens_end; ++i) {
    tokens.push_back(i->str());
  }

  // Turn tokens into lexemes
  for (const auto& curr : tokens) {
    for (const auto& [t, regex] : tokenDefinitions) {
      if (std::regex curr_pattern(regex); std::regex_match(curr, curr_pattern)) {
        if (t == Type::NUMBER) {
          lexemes.emplace_back(t, std::stod(curr));
        } else {
          lexemes.emplace_back(t);
        }
        break;
      }
    }
  }

  // Mark unary operators
  // If first character is minus, unary minus
  if (!lexemes.empty() && lexemes[0].type == Type::MIN) {
    lexemes[0].type = Type::UMIN;
  }

  for (size_t i = 1; i < lexemes.size(); i++) {
    if (isOperator(lexemes[i - 1].type) && lexemes[i].type == Type::MIN) {
      if (lexemes[i - 1].type != Type::RBRAC) {
        lexemes[i].type = Type::UMIN;
      }
    }
  }
  return lexemes;
}


/**
 * Converts INFIX lexemes into POSTFIX
 * @param infixLexemes vector of INFIX lexemes
 * @return vector of POSTFIX lexemes
 */
static std::vector<Lexeme> shuntingYard(const std::vector<Lexeme>& infixLexemes) {
  std::vector<Lexeme> output;
  std::stack<Lexeme> opStack;

  for (const auto& curr : infixLexemes) {
    // Push numbers to output
    if (curr.type == Type::NUMBER) {
      output.push_back(curr);
      continue;
    }

    // Push left brackets to opStack
    if (curr.type == Type::LBRAC) {
      opStack.push(curr);
      continue;
    }

    // Push contents op opStack to output until right bracket then remove right bracket
    if (curr.type == Type::RBRAC) {
      while (!opStack.empty() && opStack.top().type != Type::LBRAC) {
        output.push_back(opStack.top());
        opStack.pop();
      }
      opStack.pop();
      continue;
    }

    // If opStack is empty, push current operator to opStack
    while (!opStack.empty() && ((getPrecedence(opStack.top().type) > getPrecedence(curr.type)) ||
    (getPrecedence(opStack.top().type) == getPrecedence(curr.type) &&
      // Is operator left associative
      (curr.type == Type::PLUS || curr.type == Type::MIN ||
    curr.type == Type::MUL || curr.type == Type::DIV ||
    curr.type == Type::MOD)))) {
      output.push_back(opStack.top());
      opStack.pop();
    }
    opStack.push(curr);
  }

  // Cleanup
  while (!opStack.empty()) {
    output.push_back(opStack.top());
    opStack.pop();
  }
  return output;
}


/**
 * Evaluates POSTFIX expression
 * @param postfixLexemes vector of POSTFIX lexemes to be evaluated
 * @return result as double
 */
static double postfixEvaluator(const std::vector<Lexeme>& postfixLexemes) {
  std::stack<Lexeme> evalStack;

  // For all lexemes, left to right
  for (const auto& curr : postfixLexemes) {
    // If current lexeme is a number
    if (curr.type == Type::NUMBER) {
      evalStack.push(curr);
      continue;
    }

    // If current lexeme is an operator
    if (isOperator(curr.type)) {
      if (curr.type == Type::UMIN) {
        const double value = evalStack.top().value; // Get the top value
        evalStack.pop(); // Remove the top value from the stack
        evalStack.emplace(Type::NUMBER, -value); // Push the negated value back
        continue;
      }

      const double b = evalStack.top().value;
      evalStack.pop();
      const double a = evalStack.top().value;
      evalStack.pop();
      evalStack.push(operate(curr.type, a, b));
    }
  }
  // Return final element on stack as result
  return evalStack.top().value;
}


// ╔════════════════════════════════════════╗
// ║     Evaluator Wrapper & Edge Cases     ║
// ╚════════════════════════════════════════╝

/**
 * Interpret expression and return calculated result
 * @param expression string to be interpreted
 * @return result
 */
double interpret(const std::string& expression) {
  // Base cases: f string empty or only contains spaces
  // No std::ranges since code is written for c++17
  if (expression.empty() || std::all_of(expression.begin(), expression.end(), ::isspace)) {
    throw InvalidInputError("ERROR: Invalid input'" + expression + "'");
  }

  // No contains() since code is written for c++17
  if (expression.find('=') != std::string::npos) {
    // Variable definition
    // Return computed variable
    return postfixEvaluator(shuntingYard(lexer(variableExpansion(
      defineVariable(expression)))));
  }
  // Expression evaluation
  return postfixEvaluator(shuntingYard(lexer(variableExpansion(expression))));
}

// int main() {
//    std::cout << "Expected: 25" << std::endl;
//    std::cout << "Actual: "<< interpret("(3 + 4 * 2 - (8 / 4) ^ 2) + (15 % 4) - (-3) + 2^3 - 5 * 2 + 7 * (6 / 3)") << std::endl;
//      return 0;
// }
