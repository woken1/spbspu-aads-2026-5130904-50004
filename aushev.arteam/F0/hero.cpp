#include "hero.h"
#include <cstdlib>
#include <cstring>

namespace aushev {

static const int STATUS_EMPTY = 0;
static const int STATUS_OCCUPIED = 1;

std::size_t HashTable::hashFunction(const char* str, std::size_t cap)
{
  std::size_t hash = 5381;
  int c = 0;
  while ((c = static_cast< int >(*str++))) {
    hash = ((hash << 5) + hash) + static_cast< std::size_t >(c);
  }
  return hash % cap;
}

HashTable::HashTable(std::size_t initialSize) :
  table_(static_cast< hero_t* >(std::malloc(initialSize * sizeof(hero_t)))),
  capacity_(initialSize),
  count_(0)
{
  if (table_) {
    std::memset(table_, 0, capacity_ * sizeof(hero_t));
    for (std::size_t i = 0; i < capacity_; ++i) {
      table_[i].status = STATUS_EMPTY;
    }
  }
}

HashTable::~HashTable()
{
  std::free(table_);
}

void HashTable::resize()
{
  std::size_t oldCap = capacity_;
  hero_t* oldTable = table_;

  capacity_ *= 2;
  table_ = static_cast< hero_t* >(std::malloc(capacity_ * sizeof(hero_t)));
  if (!table_) {
    capacity_ = oldCap;
    table_ = oldTable;
    return;
  }

  std::memset(table_, 0, capacity_ * sizeof(hero_t));
  for (std::size_t i = 0; i < capacity_; ++i) {
    table_[i].status = STATUS_EMPTY;
  }

  count_ = 0;
  for (std::size_t i = 0; i < oldCap; ++i) {
    if (oldTable[i].status == STATUS_OCCUPIED) {
      oldTable[i].distance = 0;
      insert(oldTable[i]);
    }
  }
  std::free(oldTable);
}

}
