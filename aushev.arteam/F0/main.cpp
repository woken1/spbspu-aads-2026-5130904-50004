#include "hero.h"
#include "file_handler.h"
#include "draft_analyzer.h"
#include <iostream>
#include <string>
#include <cstring>

int main()
{
  aushev::HashTable db(64);
  aushev::DraftAnalyzer analyzer(db);
  aushev::lobby_t lobby = {0, {}, 0, false};

  std::string command;
  while (std::cin >> command) {
    if (command == "exit") {
      break;
    } else {
      std::cout << "Unknown command" << std::endl;
    }
  }

  return 0;
}
