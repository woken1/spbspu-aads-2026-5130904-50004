#ifndef LCITER_H
#define LCITER_H

#include "node.h"

namespace aushev {
  
  template< class T >
  class List;
  
  template< class T >
  class LCIter
  {
    friend class List< T >;
    
  public:
    LCIter() noexcept;
    LCIter& operator++();
    LCIter operator++(int);
    LCIter& operator--();
    LCIter operator--(int);
    const T& operator*() const noexcept;
    const T* operator->() const noexcept;
    bool operator==(const LCIter& other) const noexcept;
    bool operator!=(const LCIter& other) const noexcept;
    
  private:
    const Node< T >* node_;
    const List< T >* list_;
  };
  
  template< class T >
  LCIter< T >::LCIter() noexcept
    : node_(nullptr)
    , list_(nullptr)
  {
  }
  
  template< class T >
  LCIter< T >& LCIter< T >::operator++()
  {
    if (node_) {
      node_ = node_->next;
    }
    return *this;
  }
  
  template< class T >
  LCIter< T > LCIter< T >::operator++(int)
  {
    LCIter tmp = *this;
    ++(*this);
    return tmp;
  }
  
  template< class T >
  LCIter< T >& LCIter< T >::operator--()
  {
    if (node_) {
      node_ = node_->prev;
    } else if (list_) {
      node_ = list_->tail_;
    }
    return *this;
  }
  
  template< class T >
  LCIter< T > LCIter< T >::operator--(int)
  {
    LCIter tmp = *this;
    --(*this);
    return tmp;
  }
  
  template< class T >
  const T& LCIter< T >::operator*() const noexcept
  {
    return node_->data;
  }
  
  template< class T >
  const T* LCIter< T >::operator->() const noexcept
  {
    return &(node_->data);
  }
  
  template< class T >
  bool LCIter< T >::operator==(const LCIter& other) const noexcept
  {
    return node_ == other.node_;
  }
  
  template< class T >
  bool LCIter< T >::operator!=(const LCIter& other) const noexcept
  {
    return node_ != other.node_;
  }
  
}

#endif
