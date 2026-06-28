#ifndef STACK_H
#define STACK_H

#include "list.h"

namespace aushev {

template< typename T >
class Stack {
public:
  Stack();
  ~Stack();
  Stack(const Stack& other);
  Stack& operator=(const Stack& other);
  Stack(Stack&& other) noexcept;
  Stack& operator=(Stack&& other) noexcept;

  void push(T rhs);
  T drop();
  T& top();
  const T& top() const;
  bool empty() const;
  size_t size() const;
  void clear();

private:
  List< T > list_;
};

template< typename T >
Stack< T >::Stack()
  : list_()
{
}

template< typename T >
Stack< T >::~Stack()
{
  clear();
}

template< typename T >
Stack< T >::Stack(const Stack& other)
  : list_(other.list_)
{
}

template< typename T >
Stack< T >& Stack< T >::operator=(const Stack& other)
{
  if (this != &other) {
    list_ = other.list_;
  }
  return *this;
}

template< typename T >
Stack< T >::Stack(Stack&& other) noexcept
  : list_(std::move(other.list_))
{
}

template< typename T >
Stack< T >& Stack< T >::operator=(Stack&& other) noexcept
{
  if (this != &other) {
    list_ = std::move(other.list_);
  }
  return *this;
}

template< typename T >
void Stack< T >::push(T rhs)
{
  list_.push_front(rhs);
}

template< typename T >
T Stack< T >::drop()
{
  if (list_.empty()) {
    throw std::logic_error("Stack is empty");
  }
  T value = list_.front();
  list_.pop_front();
  return value;
}

template< typename T >
T& Stack< T >::top()
{
  if (list_.empty()) {
    throw std::logic_error("Stack is empty");
  }
  return list_.front();
}

template< typename T >
const T& Stack< T >::top() const
{
  if (list_.empty()) {
    throw std::logic_error("Stack is empty");
  }
  return list_.front();
}

template< typename T >
bool Stack< T >::empty() const
{
  return list_.empty();
}

template< typename T >
size_t Stack< T >::size() const
{
  return list_.size();
}

template< typename T >
void Stack< T >::clear()
{
  list_.clear();
}

}

#endif
