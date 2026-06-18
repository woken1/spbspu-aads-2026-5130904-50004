#ifndef LITER_H
#define LITER_H

#include "node.h"

namespace aushev {
  
  template< class T >
  class List;
  
  template< class T >
  class LIter
  {
    friend class List< T >;
    
  public:
    LIter() noexcept;
    LIter& operator++();
    LIter operator++(int);
    LIter& operator--();
    LIter operator--(int);
    T& operator*() const noexcept;
    T* operator->() const noexcept;
    bool operator==(const LIter& other) const noexcept;
    bool operator!=(const LIter& other) const noexcept;
    
  private:
    Node< T >* node_;
    List< T >* list_;
  };
  
  template< class T >
  LIter< T >::LIter() noexcept
    : node_(nullptr)
    , list_(nullptr)
  {
  }
  
  template< class T >
  LIter< T >& LIter< T >::operator++()
  {
    if (node_) {
      node_ = node_->next;
    }
    return *this;
  }
  
  template< class T >
  LIter< T > LIter< T >::operator++(int)
  {
    LIter tmp = *this;
    ++(*this);
    return tmp;
  }
  
  template< class T >
  LIter< T >& LIter< T >::operator--()
  {
    if (node_) {
      node_ = node_->prev;
    } else if (list_) {
      node_ = list_->tail_;
    }
    return *this;
  }
  
  template< class T >
  LIter< T > LIter< T >::operator--(int)
  {
    LIter tmp = *this;
    --(*this);
    return tmp;
  }
  
  template< class T >
  T& LIter< T >::operator*() const noexcept
  {
    return node_->data;
  }
  
  template< class T >
  T* LIter< T >::operator->() const noexcept
  {
    return &(node_->data);
  }
  
  template< class T >
  bool LIter< T >::operator==(const LIter& other) const noexcept
  {
    return node_ == other.node_;
  }
  
  template< class T >
  bool LIter< T >::operator!=(const LIter& other) const noexcept
  {
    return node_ != other.node_;
  }
  
}

#endif
