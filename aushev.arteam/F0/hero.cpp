#include "hero.h"
#include <cstdlib>
#include <cstring>

namespace aushev {

static const int STATUS_EMPTY = 0;

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

}
