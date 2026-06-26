#include "file_handler.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

namespace aushev {

bool FileHandler::exportCsv(const HashTable& db, const char* filename)
{
  std::FILE* f = std::fopen(filename, "w");
  if (!f) {
    return false;
  }

  std::fprintf(f, "Name,Attr,Roles,Winrate,Tier\n");
  const hero_t* all = db.getAllHeroes();

  for (std::size_t i = 0; i < db.capacity(); ++i) {
    const hero_t& h = all[i];
    if (h.status == 1) {
      std::fprintf(f, "%s,%s,\"", h.name, h.attr);
      for (int r = 0; r < h.roleCount; ++r) {
        std::fprintf(f, "%s", h.roles[r]);
        if (r < h.roleCount - 1) {
          std::fprintf(f, ";");
        }
      }
      std::fprintf(f, "\",%.1f,%c\n", h.winrate, h.tier);
    }
  }

  std::fclose(f);
  return true;
}

bool FileHandler::importCsv(HashTable& db, const char* filename, std::size_t& loadedCount)
{
  std::FILE* f = std::fopen(filename, "r");
  if (!f) {
    return false;
  }

  db.clear();
  loadedCount = 0;
  char line[512] = {};

  if (!std::fgets(line, sizeof(line), f)) {
    std::fclose(f);
    return false;
  }

  while (std::fgets(line, sizeof(line), f)) {
    hero_t h = {};
    char* token = std::strtok(line, ",");
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

    h.roleCount = 0;
    char* roleStart = token;
    while (roleStart && *roleStart && h.roleCount < static_cast< int >(MAX_ROLES)) {
      char* delim = std::strchr(roleStart, ';');
      if (delim) {
        *delim = '\0';
        std::strncpy(h.roles[h.roleCount], roleStart, MAX_ROLE_LEN - 1);
        h.roleCount++;
        roleStart = delim + 1;
      } else {
        std::strncpy(h.roles[h.roleCount], roleStart, MAX_ROLE_LEN - 1);
        h.roleCount++;
        break;
      }
    }

    token = std::strtok(nullptr, ",\n");
    if (!token) {
      continue;
    }
    h.winrate = static_cast< float >(std::atof(token));

    if (h.winrate > 53.0f) {
      h.tier = 'S';
    } else if (h.winrate > 50.0f) {
      h.tier = 'A';
    } else if (h.winrate > 47.0f) {
      h.tier = 'B';
    } else {
      h.tier = 'C';
    }

    if (db.insert(h)) {
      loadedCount++;
    }
  }

  std::fclose(f);
  return true;
}

}
