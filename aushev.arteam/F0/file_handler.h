#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "hero.h"

namespace aushev {

class FileHandler {
public:
  static bool exportCsv(const HashTable& db, const char* filename);
  static bool importCsv(HashTable& db, const char* filename, std::size_t& loadedCount);
};

}

#endif
