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
    } else if (command == "load") {
      std::string filename;
      std::cin >> filename;
      std::size_t loaded = 0;
      if (aushev::FileHandler::importCsv(db, filename.c_str(), loaded)) {
        std::cout << "Loaded " << loaded << " heroes." << std::endl;
      } else {
        std::cout << "Error loading file." << std::endl;
      }
    } else if (command == "save") {
      std::string filename;
      std::cin >> filename;
      if (aushev::FileHandler::exportCsv(db, filename.c_str())) {
        std::cout << "Database saved successfully." << std::endl;
      } else {
        std::cout << "Error saving file." << std::endl;
      }
    } else {
      std::cout << "Unknown command" << std::endl;
    }
  }

  return 0;
}
