#ifndef HERO_H
#define HERO_H

#include <cstddef>

namespace aushev {

const std::size_t MAX_NAME = 30;
const std::size_t MAX_ATTR = 10;
const std::size_t MAX_ROLES = 5;
const std::size_t MAX_ROLE_LEN = 15;
const std::size_t MAX_BANS = 6;
const std::size_t MAX_LOBBIES = 10;

struct hero_t {
  char name[MAX_NAME];
  char attr[MAX_ATTR];
  char roles[MAX_ROLES][MAX_ROLE_LEN];
  int roleCount;
  float winrate;
  char tier;
  int status;
  int distance;
};

struct lobby_t {
  int id;
  char banned[MAX_BANS][MAX_NAME];
  std::size_t banCount;
  bool isCreated;
};

class HashTable {
public:
  explicit HashTable(std::size_t initialSize = 64);
  ~HashTable();

  bool insert(const hero_t& hero);
  bool remove(const char* name);
  const hero_t* find(const char* name) const;

private:
  hero_t* table_;
  std::size_t capacity_;
  std::size_t count_;

  static std::size_t hashFunction(const char* str, std::size_t cap);
  void resize();
};

}

#endif
