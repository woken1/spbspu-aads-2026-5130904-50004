#include "hero.h"
#include <cstdlib>
#include <cstring>

namespace aushev {

static const int STATUS_EMPTY = 0;
static const int STATUS_OCCUPIED = 1;
static const int STATUS_DELETED = 2;

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

bool HashTable::insert(const hero_t& hero)
{
  if (count_ >= capacity_ * 0.75) {
    resize();
  }

  std::size_t index = hashFunction(hero.name, capacity_);
  hero_t entry = hero;
  entry.status = STATUS_OCCUPIED;
  entry.distance = 0;

  while (true) {
    if (table_[index].status == STATUS_EMPTY || table_[index].status == STATUS_DELETED) {
      table_[index] = entry;
      ++count_;
      return true;
    }

    if (entry.distance > table_[index].distance) {
      hero_t temp = table_[index];
      table_[index] = entry;
      entry = temp;
      entry.distance = 0;
    } else {
      ++entry.distance;
    }

    ++index;
    if (index >= capacity_) {
      index = 0;
    }
  }
}

bool HashTable::remove(const char* name)
{
  std::size_t index = hashFunction(name, capacity_);
  int dist = 0;

  while (table_[index].status != STATUS_EMPTY) {
    if (table_[index].status == STATUS_OCCUPIED && std::strcmp(table_[index].name, name) == 0) {
      table_[index].status = STATUS_DELETED;
      --count_;
      return true;
    }
    ++dist;
    if (dist > table_[index].distance) {
      return false;
    }
    ++index;
    if (index >= capacity_) {
      index = 0;
    }
  }
  return false;
}

const hero_t* HashTable::find(const char* name) const
{
  std::size_t index = hashFunction(name, capacity_);
  int dist = 0;

  while (table_[index].status != STATUS_EMPTY) {
    if (table_[index].status == STATUS_OCCUPIED && std::strcmp(table_[index].name, name) == 0) {
      return &table_[index];
    }
    ++dist;
    if (dist > table_[index].distance) {
      return nullptr;
    }
    ++index;
    if (index >= capacity_) {
      index = 0;
    }
  }
  return nullptr;
}

void HashTable::clear()
{
  for (std::size_t i = 0; i < capacity_; ++i) {
    table_[i].status = STATUS_EMPTY;
  }
  count_ = 0;
}

std::size_t HashTable::size() const
{
  return count_;
}

std::size_t HashTable::capacity() const
{
  return capacity_;
}

const hero_t* HashTable::getAllHeroes() const
{
  return table_;
}

}
