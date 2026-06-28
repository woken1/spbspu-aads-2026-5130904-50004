#include <iostream>
#include <fstream>
#include "stack.h"
#include "queue.h"

namespace aushev {

namespace {

const int MAX_TOKEN_SIZE = 100;

struct Token {
  char data[MAX_TOKEN_SIZE];
  size_t size;

  Token() : size(0) {
    data[0] = '\0';
  }

  void append(char c) {
    if (size < MAX_TOKEN_SIZE - 1) {
      data[size++] = c;
      data[size] = '\0';
    }
  }

  void clear() {
    size = 0;
    data[0] = '\0';
  }

  bool isEmpty() const {
    return size == 0;
  }

  bool isDigit(char c) const {
    return c >= '0' && c <= '9';
  }

  bool isSpace(char c) const {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
  }

  bool isNumber() const {
    if (size == 0) {
      return false;
    }
    for (size_t i = 0; i < size; ++i) {
      if (i == 0 && data[i] == '-') {
        if (size == 1) {
          return false;
        }
        continue;
      }
      if (!isDigit(data[i])) {
        return false;
      }
    }
    return true;
  }

  bool isOperator() const {
    if (size != 1) {
      return false;
    }
    char c = data[0];
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
  }

  bool isOpenParen() const {
    return size == 1 && data[0] == '(';
  }

  bool isCloseParen() const {
    return size == 1 && data[0] == ')';
  }

  long long toNumber() const {
    long long result = 0;
    long long sign = 1;
    size_t start = 0;

    if (size > 0 && data[0] == '-') {
      sign = -1;
      start = 1;
    }

    for (size_t i = start; i < size; ++i) {
      result = result * 10 + (data[i] - '0');
    }

    return result * sign;
  }

  char getOperator() const {
    return data[0];
  }
};

int getPrecedence(const Token& op)
{
  if (op.size != 1) {
    return 0;
  }
  char c = op.data[0];
  if (c == '+' || c == '-') {
    return 1;
  }
  if (c == '*' || c == '/' || c == '%') {
    return 2;
  }
  return 0;
}

long long applyOperator(long long a, long long b, char op)
{
  switch (op) {
  case '+':
    return a + b;
  case '-':
    return a - b;
  case '*':
    return a * b;
  case '/':
    if (b == 0) {
      throw std::logic_error("Division by zero");
    }
    return a / b;
  case '%':
    if (b == 0) {
      throw std::logic_error("Modulo by zero");
    }
    return a % b;
  default:
    throw std::logic_error("Unknown operator");
  }
}

List< Token > infixToPostfix(const List< Token >& infix)
{
  List< Token > postfix;
  Stack< Token > opStack;

  for (auto it = infix.begin(); it != infix.end(); ++it) {
    const Token& token = *it;

    if (token.isNumber()) {
      postfix.push_back(token);
    } else if (token.isOpenParen()) {
      opStack.push(token);
    } else if (token.isCloseParen()) {
      while (!opStack.empty() && !opStack.top().isOpenParen()) {
        postfix.push_back(opStack.drop());
      }
      if (opStack.empty()) {
        throw std::logic_error("Mismatched parentheses");
      }
      opStack.drop();
    } else if (token.isOperator()) {
      while (!opStack.empty() && !opStack.top().isOpenParen() &&
             getPrecedence(opStack.top()) >= getPrecedence(token)) {
        postfix.push_back(opStack.drop());
      }
      opStack.push(token);
    } else {
      throw std::logic_error("Invalid token");
    }
  }

  while (!opStack.empty()) {
    Token op = opStack.drop();
    if (op.isOpenParen() || op.isCloseParen()) {
      throw std::logic_error("Mismatched parentheses");
    }
    postfix.push_back(op);
  }

  return postfix;
}

long long evaluatePostfix(const List< Token >& postfix)
{
  Stack< long long > evalStack;

  for (auto it = postfix.begin(); it != postfix.end(); ++it) {
    const Token& token = *it;

    if (token.isNumber()) {
      evalStack.push(token.toNumber());
    } else if (token.isOperator()) {
      if (evalStack.size() < 2) {
        throw std::logic_error("Invalid expression: not enough operands");
      }
      long long b = evalStack.drop();
      long long a = evalStack.drop();
      long long result = applyOperator(a, b, token.getOperator());
      evalStack.push(result);
    } else {
      throw std::logic_error("Invalid token in postfix");
    }
  }

  if (evalStack.size() != 1) {
    throw std::logic_error("Invalid expression: too many operands");
  }

  return evalStack.drop();
}

List< Token > parseLine(std::istream& input, bool& eof)
{
  List< Token > tokens;
  Token token;
  char c;
  bool lineHasContent = false;

  while (input.get(c)) {
    Token t;
    if (t.isSpace(c)) {
      if (c == '\n') {
        if (!token.isEmpty()) {
          tokens.push_back(token);
          token.clear();
        }
        if (lineHasContent || !tokens.empty()) {
          return tokens;
        }
      } else {
        if (!token.isEmpty()) {
          tokens.push_back(token);
          token.clear();
        }
      }
    } else {
      lineHasContent = true;
      token.append(c);
    }
  }

  eof = true;
  if (!token.isEmpty()) {
    tokens.push_back(token);
  }

  return tokens;
}

int processExpressions(std::istream& input)
{
  Queue< long long > results;
  bool eof = false;

  while (!eof) {
    List< Token > tokens = parseLine(input, eof);

    if (tokens.empty()) {
      continue;
    }

    try {
      List< Token > postfix = infixToPostfix(tokens);
      long long result = evaluatePostfix(postfix);
      results.push(result);
    } catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return 1;
    }
  }

  if (results.empty()) {
    return 0;
  }

  Stack< long long > reversed;
  while (!results.empty()) {
    reversed.push(results.drop());
  }

  bool first = true;
  while (!reversed.empty()) {
    if (!first) {
      std::cout << " ";
    }
    std::cout << reversed.drop();
    first = false;
  }
  std::cout << std::endl;

  return 0;
}

}

}

int main(int argc, char* argv[])
{
  try {
    if (argc > 2) {
      std::cerr << "Error: Too many arguments" << std::endl;
      return 1;
    }

    if (argc == 2) {
      std::ifstream file(argv[1]);
      if (!file.is_open()) {
        std::cerr << "Error: Cannot open file: " << argv[1] << std::endl;
        return 1;
      }
      return aushev::processExpressions(file);
    } else {
      return aushev::processExpressions(std::cin);
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 2;
  }
}
