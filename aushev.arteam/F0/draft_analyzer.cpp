#include "draft_analyzer.h"
#include <cstring>
#include <iostream>
#include <cstdio>

namespace aushev {

DraftAnalyzer::DraftAnalyzer(const HashTable &db) :
  db_(db)
{
}

void DraftAnalyzer::setBans(lobby_t &lobby, const char *bans[], std::size_t count)
{
  lobby.ban_count = (count < MAX_BANS) ? count : MAX_BANS;
  for (std::size_t i = 0; i < lobby.ban_count; ++i) {
    std::strncpy(lobby.banned[i], bans[i], MAX_NAME - 1);
    lobby.banned[i][MAX_NAME - 1] = '\0';
  }
}

void DraftAnalyzer::addPick(lobby_t &lobby, const char *team, const char *hero_name)
{
  if (std::strcmp(team, "radiant") == 0) {
    if (lobby.radiant_count < MAX_PICKS) {
      std::strncpy(lobby.radiant_picks[lobby.radiant_count], hero_name, MAX_NAME - 1);
      lobby.radiant_picks[lobby.radiant_count][MAX_NAME - 1] = '\0';
      lobby.radiant_count++;
      std::cout << "Герой " << hero_name << " выбран за Силы Света." << std::endl;
    } else {
      std::cout << "Команда Сил Света заполнена." << std::endl;
    }
  } else if (std::strcmp(team, "dire") == 0) {
    if (lobby.dire_count < MAX_PICKS) {
      std::strncpy(lobby.dire_picks[lobby.dire_count], hero_name, MAX_NAME - 1);
      lobby.dire_picks[lobby.dire_count][MAX_NAME - 1] = '\0';
      lobby.dire_count++;
      std::cout << "Герой " << hero_name << " выбран за Силы Тьмы." << std::endl;
    } else {
      std::cout << "Команда Сил Тьмы заполнена." << std::endl;
    }
  } else {
    std::cout << "Неизвестная команда. Используйте 'radiant' или 'dire'." << std::endl;
  }
}

void DraftAnalyzer::clearDraft(lobby_t &lobby)
{
  lobby.ban_count = 0;
  lobby.radiant_count = 0;
  lobby.dire_count = 0;
  lobby.my_team_side = 0; // По умолчанию Radiant
  std::memset(lobby.banned, 0, sizeof(lobby.banned));
  std::memset(lobby.radiant_picks, 0, sizeof(lobby.radiant_picks));
  std::memset(lobby.dire_picks, 0, sizeof(lobby.dire_picks));
}

bool DraftAnalyzer::isBanned(const lobby_t &lobby, const char *name) const
{
  for (std::size_t i = 0; i < lobby.ban_count; ++i) {
    if (std::strcmp(lobby.banned[i], name) == 0) {
      return true;
    }
  }
  return false;
}

bool DraftAnalyzer::isPicked(const lobby_t &lobby, const char *name) const
{
  for (std::size_t i = 0; i < lobby.radiant_count; ++i) {
    if (std::strcmp(lobby.radiant_picks[i], name) == 0) {
      return true;
    }
  }
  for (std::size_t i = 0; i < lobby.dire_count; ++i) {
    if (std::strcmp(lobby.dire_picks[i], name) == 0) {
      return true;
    }
  }
  return false;
}

int DraftAnalyzer::getMatchupValue(const hero_t &my_hero, const hero_t &enemy_hero) const
{
  int score = 0;
  if (std::strcmp(my_hero.attr, "Int") == 0 && std::strcmp(enemy_hero.attr, "Str") == 0) {
    score += 10;
  } else if (std::strcmp(my_hero.attr, "Str") == 0 && std::strcmp(enemy_hero.attr, "Agi") == 0) {
    score += 10;
  } else if (std::strcmp(my_hero.attr, "Agi") == 0 && std::strcmp(enemy_hero.attr, "Int") == 0) {
    score += 10;
  }
  
  if (my_hero.winrate > enemy_hero.winrate) {
    score += 5;
  }
  return score;
}

void DraftAnalyzer::detectLanes(const char *enemies[], std::size_t count, char output[][100]) const
{
  int role_used[5] = {0, 0, 0, 0, 0};
  const char *default_roles[5] = {"Safe", "Mid", "Offlane", "Support", "Roam"};

  for (std::size_t i = 0; i < count; ++i) {
    const hero_t *hero = db_.find(enemies[i]);
    int best_role_idx = -1;
    int min_usage = 999;

    if (hero && hero->status == 1) {
      for (int r = 0; r < hero->role_count; ++r) {
        int idx = -1;
        for (int k = 0; k < 5; ++k) {
          if (std::strcmp(hero->roles[r], default_roles[k]) == 0) {
            idx = k;
            break;
          }
        }
        if (idx != -1 && role_used[idx] < min_usage) {
          min_usage = role_used[idx];
          best_role_idx = idx;
        }
      }
    }

    if (best_role_idx == -1) {
      for (int k = 0; k < 5; ++k) {
        if (role_used[k] < min_usage) {
          min_usage = role_used[k];
          best_role_idx = k;
        }
      }
    }

    role_used[best_role_idx]++;
    std::snprintf(output[i], 99, "%s-%s", default_roles[best_role_idx], enemies[i]);
  }
}

const hero_t *DraftAnalyzer::counterLane(const lobby_t &lobby, const char *my_role,
                                        const char *enemy_name, int &out_score) const
{
  const hero_t *enemy = db_.find(enemy_name);
  if (!enemy || enemy->status != 1) {
    out_score = 0;
    return nullptr;
  }

  const hero_t *best_hero = nullptr;
  int max_score = -999;
  const hero_t *all_heroes = db_.getAllHeroes();

  for (std::size_t i = 0; i < db_.capacity(); ++i) {
    if (all_heroes[i].status != 1 || isBanned(lobby, all_heroes[i].name) || isPicked(lobby, all_heroes[i].name)) {
      continue;
    }
    bool has_role = false;
    for (int r = 0; r < all_heroes[i].role_count; ++r) {
      if (std::strcmp(all_heroes[i].roles[r], my_role) == 0) {
        has_role = true;
        break;
      }
    }
    if (!has_role) {
      continue;
    }

    int score = getMatchupValue(all_heroes[i], *enemy);
    if (score > max_score) {
      max_score = score;
      best_hero = &all_heroes[i];
    }
  }

  out_score = max_score;
  return best_hero;
}

const hero_t *DraftAnalyzer::counterLobbyTeam(const lobby_t &lobby, const char *my_role, int &out_score) const
{
  std::size_t enemy_cnt = (lobby.my_team_side == 0) ? lobby.dire_count : lobby.radiant_count;
  if (enemy_cnt == 0) {
    out_score = 0;
    return nullptr;
  }

  const hero_t *best_hero = nullptr;
  int max_total_score = -9999;
  const hero_t *all_heroes = db_.getAllHeroes();

  for (std::size_t i = 0; i < db_.capacity(); ++i) {
    if (all_heroes[i].status != 1 || isBanned(lobby, all_heroes[i].name) || isPicked(lobby, all_heroes[i].name)) {
      continue;
    }
    bool has_role = false;
    for (int r = 0; r < all_heroes[i].role_count; ++r) {
      if (std::strcmp(all_heroes[i].roles[r], my_role) == 0) {
        has_role = true;
        break;
      }
    }
    if (!has_role) {
      continue;
    }

    int total_score = 0;
    for (std::size_t j = 0; j < enemy_cnt; ++j) {
      const char *en_name = (lobby.my_team_side == 0) ? lobby.dire_picks[j] : lobby.radiant_picks[j];
      const hero_t *enemy = db_.find(en_name);
      if (enemy && enemy->status == 1) {
        total_score += getMatchupValue(all_heroes[i], *enemy);
      }
    }

    if (total_score > max_total_score) {
      max_total_score = total_score;
      best_hero = &all_heroes[i];
    }
  }

  out_score = max_total_score;
  return best_hero;
}

float DraftAnalyzer::calcWinrate(const char *my_team[], std::size_t my_cnt,
                                const char *enemy_team[], std::size_t en_cnt,
                                char res_lines[][100]) const
{
  float base_wr = 50.0f;
  float my_sum = 0.0f;
  float en_sum = 0.0f;
  int my_valid = 0;
  int en_valid = 0;

  for (std::size_t i = 0; i < my_cnt; ++i) {
    const hero_t *my_h = db_.find(my_team[i]);
    if (my_h && my_h->status == 1) {
      my_sum += my_h->winrate;
      my_valid++;
    }
  }
  for (std::size_t i = 0; i < en_cnt; ++i) {
    const hero_t *en_h = db_.find(enemy_team[i]);
    if (en_h && en_h->status == 1) {
      en_sum += en_h->winrate;
      en_valid++;
    }
  }
  
  float my_avg = (my_valid > 0) ? (my_sum / static_cast<float>(my_valid)) : 50.0f;
  float en_avg = (en_valid > 0) ? (en_sum / static_cast<float>(en_valid)) : 50.0f;
  base_wr += my_avg - en_avg;

  std::size_t compare_limit = (my_cnt < en_cnt) ? my_cnt : en_cnt;
  int adv_count = 0;

  for (std::size_t i = 0; i < compare_limit; ++i) {
    const hero_t *my_h = db_.find(my_team[i]);
    const hero_t *en_h = db_.find(enemy_team[i]);

    if (my_h && my_h->status == 1 && en_h && en_h->status == 1) {
      int score = getMatchupValue(*my_h, *en_h);
      if (score >= 10) {
        base_wr += 1.5f;
        if (adv_count < 2) {
          std::snprintf(res_lines[adv_count], 99, "Линия %d (%s против %s)", 
                        static_cast<int>(i + 1), my_h->name, en_h->name);
          adv_count++;
        }
      }
    }
  }

  if (adv_count == 0) {
    std::strcpy(res_lines[0], "Нет явного преимущества по линиям");
    std::strcpy(res_lines[1], "Нет явного преимущества по линиям");
  } else if (adv_count == 1) {
    std::strcpy(res_lines[1], "Нет других линий с преимуществом");
  }

  return base_wr;
}

void DraftAnalyzer::analyzeSynergy(const lobby_t &lobby, char *output, std::size_t max_len) const
{
  std::size_t count = (lobby.my_team_side == 0) ? lobby.radiant_count : lobby.dire_count;
  if (count < 2) {
    std::snprintf(output, max_len, "Недостаточно героев в вашей команде для оценки.");
    return;
  }

  int str_cnt = 0;
  int agi_cnt = 0;
  int int_cnt = 0;

  for (std::size_t i = 0; i < count; ++i) {
    const char *name = (lobby.my_team_side == 0) ? lobby.radiant_picks[i] : lobby.dire_picks[i];
    const hero_t *h = db_.find(name);
    if (h && h->status == 1) {
      if (std::strcmp(h->attr, "Str") == 0) {
        str_cnt++;
      } else if (std::strcmp(h->attr, "Agi") == 0) {
        agi_cnt++;
      } else if (std::strcmp(h->attr, "Int") == 0) {
        int_cnt++;
      }
    }
  }

  if (str_cnt > 0 && agi_cnt > 0 && int_cnt > 0) {
    std::snprintf(output, max_len, "Отличный баланс! Присутствуют все типы атрибутов (Сила/Ловкость/Интеллект).");
  } else if (str_cnt > 2 || agi_cnt > 2 || int_cnt > 2) {
    std::snprintf(output, max_len, "Дисбаланс: Слишком много героев одного атрибута. Драфт уязвим.");
  } else {
    std::snprintf(output, max_len, "Нормальная синергия, но состав можно сбалансировать лучше.");
  }
}

void DraftAnalyzer::findWeakness(const lobby_t &lobby, char *output, std::size_t max_len) const
{
  std::size_t enemy_cnt = (lobby.my_team_side == 0) ? lobby.dire_count : lobby.radiant_count;
  if (enemy_cnt == 0) {
    std::snprintf(output, max_len, "У врага нет пиков для анализа уязвимостей.");
    return;
  }

  int str_cnt = 0;
  int agi_cnt = 0;
  int int_cnt = 0;

  for (std::size_t i = 0; i < enemy_cnt; ++i) {
    const char *name = (lobby.my_team_side == 0) ? lobby.dire_picks[i] : lobby.radiant_picks[i];
    const hero_t *h = db_.find(name);
    if (h && h->status == 1) {
      if (std::strcmp(h->attr, "Str") == 0) {
        str_cnt++;
      } else if (std::strcmp(h->attr, "Agi") == 0) {
        agi_cnt++;
      } else if (std::strcmp(h->attr, "Int") == 0) {
        int_cnt++;
      }
    }
  }

  if (str_cnt >= agi_cnt && str_cnt >= int_cnt) {
    std::snprintf(output, max_len, "Враг упорствует в СИЛУ. Их уязвимость — Интеллект (магический урон).");
  } else if (agi_cnt >= str_cnt && agi_cnt >= int_cnt) {
    std::snprintf(output, max_len, "Враг упорствует в ЛОВКОСТЬ. Их уязвимость — Сила (плотный физический урон).");
  } else {
    std::snprintf(output, max_len, "Враг упорствует в ИНТЕЛЛЕКТ. Их уязвимость — Ловкость (быстрый физ. урон/бёрст).");
  }
}

}
