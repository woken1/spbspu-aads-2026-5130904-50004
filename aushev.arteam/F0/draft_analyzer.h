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

  void detectLanes(const char* enemies[], std::size_t count, char output[][100]) const;
  const hero_t* counterLane(const lobby_t& lobby, const char* myRole,
                            const char* enemyName, int& outScore) const;

private:
  const HashTable& db_;

  int getMatchupValue(const hero_t& myHero, const hero_t& enemyHero) const;
};

}

#endif
