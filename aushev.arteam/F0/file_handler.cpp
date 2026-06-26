#include "file_handler.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

namespace aushev {

bool FileHandler::exportCsv(const HashTable &db, const char *filename)
{
  std::FILE *f = std::fopen(filename, "w");
  if (!f) {
    return false;
  }

  std::fprintf(f, "Name,Attr,Roles,Winrate,Tier\n");
  const hero_t *all = db.getAllHeroes();

  for (std::size_t i = 0; i < db.capacity(); ++i) {
    const hero_t &h = all[i];
    if (h.status == 1) {
      std::fprintf(f, "%s,%s,\"", h.name, h.attr);
      for (int r = 0; r < h.role_count; ++r) {
        std::fprintf(f, "%s", h.roles[r]);
        if (r < h.role_count - 1) {
          std::fprintf(f, ";");
        }
      }
      std::fprintf(f, "\",%.1f,%c\n", h.winrate, h.tier);
    }
  }

  std::fclose(f);
  return true;
}

bool FileHandler::importCsv(HashTable &db, const char *filename, std::size_t &loaded_count)
{
  std::FILE *f = std::fopen(filename, "r");
  if (!f) {
    return false;
  }

  db.clear();
  loaded_count = 0;

  char line[512] = {};
  if (!std::fgets(line, sizeof(line), f)) {
    std::fclose(f);
    return false;
  }

  while (std::fgets(line, sizeof(line), f)) {
    hero_t h = {};
    char *token = std::strtok(line, ",");
    if (!token) {
      continue;
    }
    std::strncpy(h.name, token, MAX_NAME - 1);

    token = std::strtok(nullptr, ",");
    if (!token) {
      continue;
    }
    std::strncpy(h.attr, token, MAX_ATTR - 1);

    token = std::strtok(nullptr, "\"");
    if (!token) {
      continue;
    }

    h.role_count = 0;
    char *role_start = token;
    while (role_start && *role_start && h.role_count < static_cast<int>(MAX_ROLES)) {
      char *delim = std::strchr(role_start, ';');
      if (delim) {
        *delim = '\0';
        std::strncpy(h.roles[h.role_count], role_start, MAX_ROLE_LEN - 1);
        h.role_count++;
        role_start = delim + 1;
      } else {
        std::strncpy(h.roles[h.role_count], role_start, MAX_ROLE_LEN - 1);
        h.role_count++;
        break;
      }
    }

    token = std::strtok(nullptr, ",\n");
    if (!token) {
      continue;
    }
    h.winrate = static_cast<float>(std::atof(token));

    if (h.winrate > 53.0f) {
      h.tier = 'S';
    } else if (h.winrate > 50.0f) {
      h.tier = 'A';
    } else if (h.winrate > 47.0f) {
      h.tier = 'B';
    } else {
      h.tier = 'C';
    }

    if (h.winrate <= 0.0f || std::strcmp(h.name, "Axe?") == 0) {
      h.status = 0;
    } else {
      h.status = 1;
    }

    if (db.insert(h)) {
      loaded_count++;
    }
  }

  std::fclose(f);
  return true;
}

}
