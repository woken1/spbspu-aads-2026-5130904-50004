#ifndef DRAFT_ANALYZER_H
#define DRAFT_ANALYZER_H

#include "hero.h"
#include <cstddef>

namespace aushev {

class DraftAnalyzer {
public:
  explicit DraftAnalyzer(const HashTable& db);

  void setBans(lobby_t& lobby, const char* bans[], std::size_t count);
  void clearDraft(lobby_t& lobby);
  bool isBanned(const lobby_t& lobby, const char* name) const;

private:
  const HashTable& db_;
};

}

#endif
