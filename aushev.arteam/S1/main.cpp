#include <iostream>
#include "list.h"

namespace aushev {

namespace {

struct Sequence {
  std::string name;
  List< int > numbers;
};

}

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
        std::cerr << "Error: invalid input format" << std::endl;
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
    std::cout << "0" << std::endl;
    return 0;
  }

  bool first = true;
  for (auto it = sequences.begin(); it != sequences.end(); ++it) {
    if (!first) {
      std::cout << " ";
    }
    std::cout << (*it).name;
    first = false;
  }
  std::cout << std::endl;

  size_t maxSize = 0;
  for (auto it = sequences.begin(); it != sequences.end(); ++it) {
    if ((*it).numbers.size() > maxSize) {
      maxSize = (*it).numbers.size();
    }
  }

  List< long long > sumsList;

  for (size_t i = 0; i < maxSize; ++i) {
    long long currentSum = 0;
    bool rowHasElements = false;

    for (auto it = sequences.begin(); it != sequences.end(); ++it) {
      if (i < (*it).numbers.size()) {
        auto numIt = (*it).numbers.begin();
        for (size_t j = 0; j < i; ++j) {
          ++numIt;
        }

        if (!rowHasElements) {
          rowHasElements = true;
        } else {
          std::cout << " ";
        }
        std::cout << *numIt;
        currentSum += *numIt;
      }
    }
    std::cout << std::endl;
    sumsList.push_back(currentSum);
  }

  first = true;
  for (auto it = sumsList.begin(); it != sumsList.end(); ++it) {
    if (!first) {
      std::cout << " ";
    }
    std::cout << *it;
    first = false;
  }
  std::cout << std::endl;

  return 0;
}

}

int main()
{
  try {
    return aushev::processSequences();
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 2;
  }
}
