#include <iostream>
#include <string>
#include "list.h"

namespace aushev {

struct Sequence {
  std::string name;
  List< int > numbers;
};

int processSequences()
{
  List< Sequence > sequences;
  std::string name;

  while (std::cin >> name) {
    Sequence seq;
    seq.name = name;
    int number;
    while (std::cin.peek() != '\n' && std::cin.peek() != EOF) {
      if (!(std::cin >> number)) {
        if (std::cin.eof()) {
          break;
        }
        std::cerr << "Error: invalid input\n";
        return 1;
      }
      seq.numbers.push_back(number);
    }
    sequences.push_back(seq);
    if (std::cin.eof()) {
      break;
    }
    std::cin.ignore();
  }

  if (sequences.empty()) {
    std::cout << "0\n";
    return 0;
  }

  for (auto it = sequences.begin(); it != sequences.end(); ++it) {
    if (it != sequences.begin()) {
      std::cout << " ";
    }
    std::cout << (*it).name;
  }
  std::cout << "\n";

  size_t maxSize = 0;
  for (auto it = sequences.begin(); it != sequences.end(); ++it) {
    if ((*it).numbers.size() > maxSize) {
      maxSize = (*it).numbers.size();
    }
  }

  List< List< int > > transposed;
  for (size_t i = 0; i < maxSize; ++i) {
    List< int > row;
    for (auto it = sequences.begin(); it != sequences.end(); ++it) {
      if (i < (*it).numbers.size()) {
        auto numIt = (*it).numbers.begin();
        for (size_t j = 0; j < i; ++j) {
          ++numIt;
        }
        row.push_back(*numIt);
      }
    }
    transposed.push_back(row);
  }

  for (auto it = transposed.begin(); it != transposed.end(); ++it) {
    for (auto numIt = (*it).begin(); numIt != (*it).end(); ++numIt) {
      if (numIt != (*it).begin()) {
        std::cout << " ";
      }
      std::cout << *numIt;
    }
    std::cout << "\n";
  }

  for (auto it = transposed.begin(); it != transposed.end(); ++it) {
    long long sum = 0;
    bool canSum = true;
    for (auto numIt = (*it).begin(); numIt != (*it).end(); ++numIt) {
      sum += *numIt;
    }
    if (canSum) {
      if (it != transposed.begin()) {
        std::cout << " ";
      }
      std::cout << sum;
    } else {
      std::cerr << "Error: cannot calculate sum\n";
      return 1;
    }
  }
  std::cout << "\n";

  return 0;
}

}

int main()
{
  return aushev::processSequences();
}
