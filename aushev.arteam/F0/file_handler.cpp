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

}
