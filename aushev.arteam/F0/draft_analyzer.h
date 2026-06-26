#ifndef DRAFT_ANALYZER_H
#define DRAFT_ANALYZER_H

#include "hero.h"
#include <cstddef>

namespace aushev {

class DraftAnalyzer {
public:
  explicit DraftAnalyzer(const HashTable &db);

  void setBans(lobby_t &lobby, const char *bans[], std::size_t count);
  void addPick(lobby_t &lobby, const char *team, const char *hero_name);
  void clearDraft(lobby_t &lobby);
  bool isBanned(const lobby_t &lobby, const char *name) const;
  bool isPicked(const lobby_t &lobby, const char *name) const;

  void detectLanes(const char *enemies[], std::size_t count, char output[][100]) const;
  
  const hero_t *counterLane(const lobby_t &lobby, const char *my_role,
                            const char *enemy_name, int &out_score) const;
                            
  const hero_t *counterLobbyTeam(const lobby_t &lobby, const char *my_role, int &out_score) const;

  float calcWinrate(const char *my_team[], std::size_t my_cnt,
                    const char *enemy_team[], std::size_t en_cnt,
                    char res_lines[][100]) const;

  void analyzeSynergy(const lobby_t &lobby, char *output, std::size_t max_len) const;
  void findWeakness(const lobby_t &lobby, char *output, std::size_t max_len) const;

private:
  const HashTable &db_;

  int getMatchupValue(const hero_t &my_hero, const hero_t &enemy_hero) const;
};

}

#endif
