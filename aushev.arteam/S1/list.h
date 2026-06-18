#ifndef LIST_H
#define LIST_H

#include <cstddef>
#include "node.h"
#include "liter.h"
#include "lciter.h"

namespace aushev {
  
  template< class T >
  class List
  {
    friend class LIter< T >;
    friend class LCIter< T >;
    
  public:
    List() noexcept;
    ~List() noexcept;
    List(const List& other);
    List& operator=(const List& other);
    List(List&& other) noexcept;
    List& operator=(List&& other) noexcept;
    
    LIter< T > begin() noexcept;
    LIter< T > end() noexcept;
    LCIter< T > begin() const noexcept;
    LCIter< T > end() const noexcept;
    LCIter< T > cbegin() const noexcept;
    LCIter< T > cend() const noexcept;
    
    bool empty() const noexcept;
    size_t size() const noexcept;
    void clear() noexcept;
    
    void push_front(const T& value);
    void push_back(const T& value);
    
    void pop_front() noexcept;
    void pop_back() noexcept;
    
    T& front() noexcept;
    T& back() noexcept;
    const T& front() const noexcept;
    const T& back() const noexcept;
    
  private:
    using NodeT = Node< T >;
    NodeT* head_;
    NodeT* tail_;
    size_t size_;
  };
  
  template< class T >
  List< T >::List() noexcept
    : head_(nullptr)
    , tail_(nullptr)
    , size_(0)
  {
  }
  
  template< class T >
  List< T >::~List() noexcept
  {
    clear();
  }
  
  template< class T >
  List< T >::List(const List& other)
    : head_(nullptr)
    , tail_(nullptr)
    , size_(0)
  {
    for (const auto& item : other) {
      push_back(item);
    }
  }
  
  template< class T >
  List< T >& List< T >::operator=(const List& other)
  {
    if (this != &other) {
      clear();
      for (const auto& item : other) {
        push_back(item);
      }
    }
    return *this;
  }
  
  template< class T >
  List< T >::List(List&& other) noexcept
    : head_(other.head_)
    , tail_(other.tail_)
    , size_(other.size_)
  {
    other.head = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
  }
  
  template< class T >
  List< T >& List< T >::operator=(List&& other) noexcept
  {
    if (this != &other) {
      clear();
      head_ = other.head_;
      tail_ = other.tail_;
      size_ = other.size_;
      other.head_ = nullptr;
      other.tail_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }
  
  template< class T >
  LIter< T > List< T >::begin() noexcept
  {
    LIter< T > it;
    it.node_ = head_;
    it.list_ = this;
    return it;
  }
  
  template< class T >
  LIter< T > List< T >::end() noexcept
  {
    LIter< T > it;
    it.node_ = nullptr;
    it.list_ = this;
    return it;
  }
  
  template< class T >
  LCIter< T > List< T >::begin() const noexcept
  {
    LCIter< T > it;
    it.node_ = head_;
    it.list_ = this;
    return it;
  }
  
  template< class T >
  LCIter< T > List< T >::end() const noexcept
  {
    LCIter< T > it;
    it.node_ = nullptr;
    it.list_ = this;
    return it;
  }
  
  template< class T >
  LCIter< T > List< T >::cbegin() const noexcept
  {
    return begin();
  }
  
  template< class T >
  LCIter< T > List< T >::cend() const noexcept
  {
    return end();
  }
  
  template< class T >
  bool List< T >::empty() const noexcept
  {
    return size_ == 0;
  }
  
  template< class T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }
  
  template< class T >
  void List< T >::clear() noexcept
  {
    while (head_ != nullptr) {
      NodeT* tmp = head_;
      head_ = head_->next;
      delete tmp;
    }
    tail_ = nullptr;
    size_ = 0;
  }
  
  template< class T >
  void List< T >::push_front(const T& value)
  {
    NodeT* newNode = new NodeT;
    newNode->data = value;
    newNode->next = head_;
    newNode->prev = nullptr;
    if (head_) {
      head_->prev = newNode;
    }
    head_ = newNode;
    if (tail_ == nullptr) {
      tail_ = newNode;
    }
    ++size_;
  }
  
  template< class T >
  void List< T >::push_back(const T& value)
  {
    NodeT* newNode = new NodeT;
    newNode->data = value;
    newNode->next = nullptr;
    newNode->prev = tail_;
    if (tail_) {
      tail_->next = newNode;
    }
    tail_ = newNode;
    if (head_ == nullptr) {
      head_ = newNode;
    }
    ++size_;
  }
  
  template< class T >
  void List< T >::pop_front() noexcept
  {
    if (head_ == nullptr) {
      return;
    }
    NodeT* tmp = head_;
    head_ = head_->next;
    if (head_) {
      head_->prev = nullptr;
    } else {
      tail_ = nullptr;
    }
    delete tmp;
    --size_;
  }
  
  template< class T >
  void List< T >::pop_back() noexcept
  {
    if (tail_ == nullptr) {
      return;
    }
    if (head_ == tail_) {
      delete tail_;
      head_ = nullptr;
      tail_ = nullptr;
      --size_;
      return;
    }
    tail_ = tail_->prev;
    delete tail_->next;
    tail_->next = nullptr;
    --size_;
  }
  
  template< class T >
  T& List< T >::front() noexcept
  {
    return head_->data;
  }
  
  template< class T >
  T& List< T >::back() noexcept
  {
    return tail_->data;
  }
  
  template< class T >
  const T& List< T >::front() const noexcept
  {
    return head_->data;
  }
  
  template< class T >
  const T& List< T >::back() const noexcept
  {
    return tail_->data;
  }
  
}

#endif
