#include "hero.h"
#include "file_handler.h"
#include "draft_analyzer.h"
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>

int main()
{
  aushev::HashTable db(64);
  aushev::DraftAnalyzer analyzer(db);

  aushev::lobby_t lobbies[aushev::MAX_LOBBIES];
  for (std::size_t i = 0; i < aushev::MAX_LOBBIES; ++i) {
    lobbies[i].id = -1;
    lobbies[i].ban_count = 0;
    lobbies[i].radiant_count = 0;
    lobbies[i].dire_count = 0;
    lobbies[i].my_team_side = 0;
    lobbies[i].is_created = false;
  }

  int current_lobby_idx = -1;
  std::string line;

  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }

    std::stringstream ss(line);
    std::string command;
    ss >> command;

    if (command == "exit") {
      break;
    } else if (command == "load") {
      std::string filename;
      ss >> filename;
      std::size_t loaded = 0;
      if (aushev::FileHandler::importCsv(db, filename.c_str(), loaded)) {
        std::cout << "Загружено " << loaded << " героев." << std::endl;
      } else {
        std::cout << "Ошибка загрузки файла." << std::endl;
      }
    } else if (command == "save") {
      std::string filename;
      ss >> filename;
      if (aushev::FileHandler::exportCsv(db, filename.c_str())) {
        std::cout << "База данных успешно сохранена." << std::endl;
      } else {
        std::cout << "Ошибка сохранения файла." << std::endl;
      }
    } else if (command == "lobby") {
      int target_id = 0;
      ss >> target_id;
      
      int found_idx = -1;
      int empty_idx = -1;
      for (int i = 0; i < static_cast<int>(aushev::MAX_LOBBIES); ++i) {
        if (lobbies[i].is_created && lobbies[i].id == target_id) {
          found_idx = i;
          break;
        }
        if (!lobbies[i].is_created && empty_idx == -1) {
          empty_idx = i;
        }
      }

      if (found_idx != -1) {
        current_lobby_idx = found_idx;
        std::cout << "Переключено на лобби " << target_id << "." << std::endl;
      } else if (empty_idx != -1) {
        lobbies[empty_idx].id = target_id;
        lobbies[empty_idx].is_created = true;
        analyzer.clearDraft(lobbies[empty_idx]);
        current_lobby_idx = empty_idx;
        std::cout << "Лобби " << target_id << " создано и готово к работе." << std::endl;
      } else {
        std::cout << "Достигнут лимит лобби." << std::endl;
      }
    } else if (command == "myteam") {
      if (current_lobby_idx == -1) {
        std::cout << "Сначала создайте лобби." << std::endl;
        continue;
      }
      std::string side;
      if (ss >> side) {
        if (side == "radiant") {
          lobbies[current_lobby_idx].my_team_side = 0;
          std::cout << "Ваша сторона в текущем лобби изменена на: Силы Света (Radiant)." << std::endl;
        } else if (side == "dire") {
          lobbies[current_lobby_idx].my_team_side = 1;
          std::cout << "Ваша сторона в текущем лобби изменена на: Силы Тьмы (Dire)." << std::endl;
        } else {
          std::cout << "Ошибка: укажите 'radiant' или 'dire'." << std::endl;
        }
      } else {
        std::cout << "Ошибка: укажите сторону." << std::endl;
      }
    } else if (command == "status") {
      if (current_lobby_idx == -1) {
        std::cout << "Нет активного лобби. Сначала используйте 'lobby <id>'." << std::endl;
        continue;
      }
      const aushev::lobby_t &curr = lobbies[current_lobby_idx];
      std::cout << "=== Лобби " << curr.id << " ===" << std::endl;
      std::cout << "Ваша сторона: " << ((curr.my_team_side == 0) ? "Силы Света (Radiant)" : "Силы Тьмы (Dire)") << std::endl;
      std::cout << "Баны (" << curr.ban_count << "): ";
      for (std::size_t i = 0; i < curr.ban_count; ++i) {
        std::cout << curr.banned[i] << " ";
      }
      std::cout << "\nПики Света (" << curr.radiant_count << "): ";
      for (std::size_t i = 0; i < curr.radiant_count; ++i) {
        std::cout << curr.radiant_picks[i] << " ";
      }
      std::cout << "\nПики Тьмы (" << curr.dire_count << "): ";
      for (std::size_t i = 0; i < curr.dire_count; ++i) {
        std::cout << curr.dire_picks[i] << " ";
      }
      std::cout << std::endl;
    } else if (command == "ban") {
      if (current_lobby_idx == -1) {
        std::cout << "Сначала создайте лобби." << std::endl;
        continue;
      }
      std::size_t count = 0;
      if (!(ss >> count)) {
        std::cout << "Ошибка: укажите количество банов." << std::endl;
        continue;
      }
      
      const char *ban_list[aushev::MAX_BANS] = {};
      std::string names[aushev::MAX_BANS] = {};
      std::size_t actual_read = 0;

      for (std::size_t i = 0; i < count && i < aushev::MAX_BANS; ++i) {
        if (ss >> names[actual_read]) {
          ban_list[actual_read] = names[actual_read].c_str();
          actual_read++;
        } else {
          break;
        }
      }
      
      if (actual_read > 0) {
        analyzer.setBans(lobbies[current_lobby_idx], ban_list, actual_read);
        std::cout << "Баны обновлены. Успешно внесено: " << actual_read << " героев." << std::endl;
      } else {
        std::cout << "Ошибка: не введено ни одного имени героя." << std::endl;
      }
    } else if (command == "pick") {
      if (current_lobby_idx == -1) {
        std::cout << "Сначала создайте лобби." << std::endl;
        continue;
      }
      std::string team;
      std::string hero_name;
      ss >> team >> hero_name;
      analyzer.addPick(lobbies[current_lobby_idx], team.c_str(), hero_name.c_str());
    } else if (command == "lanes") {
      const char *enemy_list[5] = {};
      std::string enemy_names[5] = {};
      std::size_t count = 0;

      while (count < 5 && (ss >> enemy_names[count])) {
        enemy_list[count] = enemy_names[count].c_str();
        count++;
      }

      if (count == 0) {
        if (current_lobby_idx == -1) {
          std::cout << "Ошибка: нет активного лобби." << std::endl;
          continue;
        }
        const aushev::lobby_t &curr = lobbies[current_lobby_idx];
        std::size_t target_cnt = (curr.my_team_side == 0) ? curr.dire_count : curr.radiant_count;
        if (target_cnt == 0) {
          std::cout << "Ошибка: у противоположной команды нет героев для распределения линий." << std::endl;
          continue;
        }
        count = target_cnt;
        for (std::size_t i = 0; i < count; ++i) {
          enemy_list[i] = (curr.my_team_side == 0) ? curr.dire_picks[i] : curr.radiant_picks[i];
        }
      }

      char res_lanes[5][100] = {};
      analyzer.detectLanes(enemy_list, count, res_lanes);
      for (std::size_t i = 0; i < count; ++i) {
        std::cout << res_lanes[i] << std::endl;
      }
    } else if (command == "counter") {
      if (current_lobby_idx == -1) {
        std::cout << "Сначала создайте лобби." << std::endl;
        continue;
      }
      std::string role;
      std::string enemy_name;
      ss >> role;

      const aushev::hero_t *best = nullptr;
      int score = 0;

      if (ss >> enemy_name) {
        best = analyzer.counterLane(lobbies[current_lobby_idx], role.c_str(), enemy_name.c_str(), score);
      } else {
        best = analyzer.counterLobbyTeam(lobbies[current_lobby_idx], role.c_str(), score);
      }

      if (best) {
        std::cout << "Контр-пик: " << best->name << " (Оценка: " << score << ")" << std::endl;
      } else {
        std::cout << "Контр-пик не найден (возможно, у противоположной команды нет героев)." << std::endl;
      }
    } else if (command == "calc") {
      const char *my_team[5] = {};
      const char *enemy_team[5] = {};
      std::string my_names[5] = {};
      std::string en_names[5] = {};
      std::size_t my_cnt = 0;
      std::size_t en_cnt = 0;

      while (my_cnt < 5 && (ss >> my_names[my_cnt])) {
        my_team[my_cnt] = my_names[my_cnt].c_str();
        my_cnt++;
      }
      if (my_cnt > 0) {
        while (en_cnt < 5 && (ss >> en_names[en_cnt])) {
          enemy_team[en_cnt] = en_names[en_cnt].c_str();
          en_cnt++;
        }
      } else {
        if (current_lobby_idx == -1) {
          std::cout << "Ошибка: нет активного лобби." << std::endl;
          continue;
        }
        const aushev::lobby_t &curr = lobbies[current_lobby_idx];
        if (curr.radiant_count == 0 && curr.dire_count == 0) {
          std::cout << "Ошибка: в лобби нет героев для автоматического расчета." << std::endl;
          continue;
        }
        
        if (curr.my_team_side == 0) {
          my_cnt = curr.radiant_count;
          en_cnt = curr.dire_count;
          for (std::size_t i = 0; i < my_cnt; ++i) my_team[i] = curr.radiant_picks[i];
          for (std::size_t i = 0; i < en_cnt; ++i) enemy_team[i] = curr.dire_picks[i];
        } else {
          my_cnt = curr.dire_count;
          en_cnt = curr.radiant_count;
          for (std::size_t i = 0; i < my_cnt; ++i) my_team[i] = curr.dire_picks[i];
          for (std::size_t i = 0; i < en_cnt; ++i) enemy_team[i] = curr.radiant_picks[i];
        }
      }

      char res_lines[2][100] = {};
      float wr = analyzer.calcWinrate(my_team, my_cnt, enemy_team, en_cnt, res_lines);
      std::cout << "Винрейт вашей стороны: " << wr << "%" << std::endl;
      std::cout << "Преимущество 1: " << res_lines[0] << std::endl;
      std::cout << "Преимущество 2: " << res_lines[1] << std::endl;
    } else if (command == "synergy") {
      if (current_lobby_idx == -1) {
        std::cout << "Сначала создайте лобби." << std::endl;
        continue;
      }
      char out_buf[150] = {};
      analyzer.analyzeSynergy(lobbies[current_lobby_idx], out_buf, sizeof(out_buf));
      std::cout << "Синергия вашей команды: " << out_buf << std::endl;
    } else if (command == "weakness") {
      if (current_lobby_idx == -1) {
        std::cout << "Сначала создайте лобби." << std::endl;
        continue;
      }
      char out_buf[150] = {};
      analyzer.findWeakness(lobbies[current_lobby_idx], out_buf, sizeof(out_buf));
      std::cout << "Анализ уязвимостей врага: " << out_buf << std::endl;
    } else {
      std::cout << "Неизвестная команда" << std::endl;
    }
  }

  return 0;
}
