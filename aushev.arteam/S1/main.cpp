#include <iostream>
#include <string>
#include <limits>
#include "list.h"

namespace aushev {

  namespace {

    struct Sequence
    {
      std::string name;
      List< unsigned long long > numbers;
    };

    bool readSequences(std::istream& in, List< Sequence >& sequences)
    {
      std::string line;
      while (std::getline(in, line)) {
        if (line.empty()) {
          continue;
        }

        size_t pos = line.find(' ');
        Sequence seq;

        if (pos == std::string::npos) {
          seq.name = line;
        } else {
          seq.name = line.substr(0, pos);
          std::string numbersPart = line.substr(pos + 1);
          size_t start = 0;

          while (start < numbersPart.length()) {
            while (start < numbersPart.length() && numbersPart[start] == ' ') {
              ++start;
            }
            if (start >= numbersPart.length()) {
              break;
            }

            size_t end = numbersPart.find(' ', start);
            std::string numStr = (end == std::string::npos) ? numbersPart.substr(start) : numbersPart.substr(start, end - start);

            try {
              size_t processedChars = 0;
              unsigned long long number = std::stoull(numStr, &processedChars);
              if (processedChars != numStr.length()) {
                return false;
              }
              seq.numbers.push_back(number);
            } catch (...) {
              return false;
            }

            if (end == std::string::npos) {
              break;
            }
            start = end + 1;
          }
        }

        sequences.push_back(seq);
      }
      return true;
    }

    void printNames(const List< Sequence >& sequences)
    {
      if (sequences.empty()) {
        return;
      }
      auto it = sequences.begin();
      std::cout << (*it).name;
      ++it;
      for (; it != sequences.end(); ++it) {
        std::cout << " " << (*it).name;
      }
      std::cout << std::endl;
    }

    size_t getMaxSize(const List< Sequence >& sequences)
    {
      size_t maxSize = 0;
      for (auto it = sequences.begin(); it != sequences.end(); ++it) {
        if ((*it).numbers.size() > maxSize) {
          maxSize = (*it).numbers.size();
        }
      }
      return maxSize;
    }

    bool printMatrixAndCalculateSums(const List< Sequence >& sequences, size_t maxSize, List< unsigned long long >& sums)
    {
      for (size_t i = 0; i < maxSize; ++i) {
        unsigned long long currentSum = 0;
        auto it = sequences.begin();

        while (it != sequences.end() && i >= (*it).numbers.size()) {
          ++it;
        }

        if (it != sequences.end()) {
          auto numIt = (*it).numbers.begin();
          for (size_t j = 0; j < i; ++j) {
            ++numIt;
          }
          std::cout << *numIt;
          currentSum += *numIt;
          ++it;

          for (; it != sequences.end(); ++it) {
            if (i < (*it).numbers.size()) {
              auto nextNumIt = (*it).numbers.begin();
              for (size_t j = 0; j < i; ++j) {
                ++nextNumIt;
              }
              std::cout << " " << *nextNumIt;

              if (std::numeric_limits< unsigned long long >::max() - currentSum < *nextNumIt) {
                return false;
              }
              currentSum += *nextNumIt;
            }
          }
        }

        std::cout << std::endl;
        sums.push_back(currentSum);
      }
      return true;
    }

    void printSums(const List< unsigned long long >& sums)
    {
      if (sums.empty()) {
        return;
      }

      auto it = sums.begin();
      std::cout << *it;
      ++it;

      for (; it != sums.end(); ++it) {
        std::cout << " " << *it;
      }
      std::cout << std::endl;
    }

  }

}

int main()
{
  aushev::List< aushev::Sequence > sequences;
  if (!aushev::readSequences(std::cin, sequences)) {
    std::cerr << "Error: invalid input format" << std::endl;
    return 1;
  }

  if (sequences.empty()) {
    std::cout << "0" << std::endl;
    return 0;
  }

  aushev::printNames(sequences);

  size_t maxSize = aushev::getMaxSize(sequences);
  if (maxSize == 0) {
    std::cout << "0" << std::endl;
    return 0;
  }

  aushev::List< unsigned long long > sums;
  if (!aushev::printMatrixAndCalculateSums(sequences, maxSize, sums)) {
    std::cerr << "Error: sum overflow" << std::endl;
    return 1;
  }

  aushev::printSums(sums);
  return 0;
}
