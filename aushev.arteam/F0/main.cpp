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
    } else if (command == "lobby") {
      int id = 0;
      std::cin >> id;
      lobby.id = id;
      lobby.banCount = 0;
      lobby.isCreated = true;
      analyzer.clearDraft(lobby);
      std::cout << "Lobby " << id << " ready." << std::endl;
    } else if (command == "ban") {
      if (!lobby.isCreated) {
        std::cout << "Create lobby first." << std::endl;
        continue;
      }
      std::size_t count = 0;
      std::cin >> count;
      const char* banList[aushev::MAX_BANS] = {};
      std::string names[aushev::MAX_BANS] = {};
      for (std::size_t i = 0; i < count && i < aushev::MAX_BANS; ++i) {
        std::cin >> names[i];
        banList[i] = names[i].c_str();
      }
      analyzer.setBans(lobby, banList, count);
      std::cout << "Bans updated." << std::endl;
    } else if (command == "lanes") {
      const char* enemyList[5] = {};
      std::string enemyNames[5] = {};
      for (int i = 0; i < 5; ++i) {
        std::cin >> enemyNames[i];
        enemyList[i] = enemyNames[i].c_str();
      }
      char resLanes[5][100] = {};
      analyzer.detectLanes(enemyList, 5, resLanes);
      for (int i = 0; i < 5; ++i) {
        std::cout << resLanes[i] << std::endl;
      }
    } else if (command == "counter") {
      if (!lobby.isCreated) {
        std::cout << "Create lobby first." << std::endl;
        continue;
      }
      std::string role;
      std::string enemyName;
      std::cin >> role >> enemyName;
      int score = 0;
      const aushev::hero_t* best = analyzer.counterLane(lobby, role.c_str(),
                                                        enemyName.c_str(), score);
      if (best) {
        std::cout << "Counter: " << best->name << " (Score: " << score << ")" << std::endl;
      } else {
        std::cout << "No counter found." << std::endl;
      }
    } else {
      std::cout << "Unknown command" << std::endl;
    }
  }

  return 0;
}
