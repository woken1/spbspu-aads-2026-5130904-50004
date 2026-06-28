#ifndef QUEUE_H
#define QUEUE_H

#include "list.h"

namespace aushev {

template< typename T >
class Queue {
public:
  Queue();
  ~Queue();
  Queue(const Queue& other);
  Queue& operator=(const Queue& other);
  Queue(Queue&& other) noexcept;
  Queue& operator=(Queue&& other) noexcept;

  void push(T rhs);
  T drop();
  T& front();
  const T& front() const;
  bool empty() const;
  size_t size() const;
  void clear();

private:
  List< T > list_;
};

template< typename T >
Queue< T >::Queue()
  : list_()
{
}

template< typename T >
Queue< T >::~Queue()
{
  clear();
}

template< typename T >
Queue< T >::Queue(const Queue& other)
  : list_(other.list_)
{
}

template< typename T >
Queue< T >& Queue< T >::operator=(const Queue& other)
{
  if (this != &other) {
    list_ = other.list_;
  }
  return *this;
}

template< typename T >
Queue< T >::Queue(Queue&& other) noexcept
  : list_(std::move(other.list_))
{
}

template< typename T >
Queue< T >& Queue< T >::operator=(Queue&& other) noexcept
{
  if (this != &other) {
    list_ = std::move(other.list_);
  }
  return *this;
}

template< typename T >
void Queue< T >::push(T rhs)
{
  list_.push_back(rhs);
}

template< typename T >
T Queue< T >::drop()
{
  if (list_.empty()) {
    throw std::logic_error("Queue is empty");
  }
  T value = list_.front();
  list_.pop_front();
  return value;
}

template< typename T >
T& Queue< T >::front()
{
  if (list_.empty()) {
    throw std::logic_error("Queue is empty");
  }
  return list_.front();
}

template< typename T >
const T& Queue< T >::front() const
{
  if (list_.empty()) {
    throw std::logic_error("Queue is empty");
  }
  return list_.front();
}

template< typename T >
bool Queue< T >::empty() const
{
  return list_.empty();
}

template< typename T >
size_t Queue< T >::size() const
{
  return list_.size();
}

template< typename T >
void Queue< T >::clear()
{
  list_.clear();
}

}

#endif
