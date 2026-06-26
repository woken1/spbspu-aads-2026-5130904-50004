#include "draft_analyzer.h"
#include <cstring>
#include <cstdio>

namespace aushev {

DraftAnalyzer::DraftAnalyzer(const HashTable& db) :
  db_(db)
{
}

void DraftAnalyzer::setBans(lobby_t& lobby, const char* bans[], std::size_t count)
{
  lobby.banCount = (count < MAX_BANS) ? count : MAX_BANS;
  for (std::size_t i = 0; i < lobby.banCount; ++i) {
    std::strncpy(lobby.banned[i], bans[i], MAX_NAME - 1);
    lobby.banned[i][MAX_NAME - 1] = '\0';
  }
}

void DraftAnalyzer::clearDraft(lobby_t& lobby)
{
  lobby.banCount = 0;
  std::memset(lobby.banned, 0, sizeof(lobby.banned));
}

bool DraftAnalyzer::isBanned(const lobby_t& lobby, const char* name) const
{
  for (std::size_t i = 0; i < lobby.banCount; ++i) {
    if (std::strcmp(lobby.banned[i], name) == 0) {
      return true;
    }
  }
  return false;
}

int DraftAnalyzer::getMatchupValue(const hero_t& myHero, const hero_t& enemyHero) const
{
  int score = 0;
  if ((std::strcmp(myHero.attr, "Int") == 0 && std::strcmp(enemyHero.attr, "Str") == 0) ||
      (std::strcmp(myHero.attr, "Str") == 0 && std::strcmp(enemyHero.attr, "Agi") == 0) ||
      (std::strcmp(myHero.attr, "Agi") == 0 && std::strcmp(enemyHero.attr, "Int") == 0)) {
    score += 10;
  }
  if (myHero.winrate > enemyHero.winrate) {
    score += 5;
  }
  return score;
}

}
