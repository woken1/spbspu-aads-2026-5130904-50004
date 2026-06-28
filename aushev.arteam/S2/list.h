#ifndef LIST_H
#define LIST_H

#include <cstddef>

namespace aushev {

template< class T >
struct Node {
  T data;
  Node* next;
  Node* prev;
};

template< class T >
class List;

template< class T >
class LIter {
  friend class List< T >;

public:
  LIter();
  LIter& operator++();
  LIter operator++(int);
  LIter& operator--();
  LIter operator--(int);
  T& operator*() const;
  T* operator->() const;
  bool operator==(const LIter& other) const;
  bool operator!=(const LIter& other) const;

private:
  Node< T >* node_;
  List< T >* list_;
};

template< class T >
class LCIter {
  friend class List< T >;

public:
  LCIter();
  LCIter& operator++();
  LCIter operator++(int);
  LCIter& operator--();
  LCIter operator--(int);
  const T& operator*() const;
  const T* operator->() const;
  bool operator==(const LCIter& other) const;
  bool operator!=(const LCIter& other) const;

private:
  const Node< T >* node_;
  const List< T >* list_;
};

template< class T >
class List {
  friend class LIter< T >;
  friend class LCIter< T >;

public:
  List();
  ~List();
  List(const List& other);
  List& operator=(const List& other);
  List(List&& other) noexcept;
  List& operator=(List&& other) noexcept;

  LIter< T > begin();
  LIter< T > end();
  LCIter< T > begin() const;
  LCIter< T > end() const;
  LCIter< T > cbegin() const;
  LCIter< T > cend() const;

  bool empty() const;
  size_t size() const;
  void clear();

  void push_front(const T& value);
  void push_back(const T& value);

  void pop_front();
  void pop_back();

  T& front();
  T& back();
  const T& front() const;
  const T& back() const;

private:
  using NodeT = Node< T >;
  NodeT* head_;
  NodeT* tail_;
  size_t size_;
};

template< class T >
LIter< T >::LIter()
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
T& LIter< T >::operator*() const
{
  return node_->data;
}

template< class T >
T* LIter< T >::operator->() const
{
  return &(node_->data);
}

template< class T >
bool LIter< T >::operator==(const LIter& other) const
{
  return node_ == other.node_;
}

template< class T >
bool LIter< T >::operator!=(const LIter& other) const
{
  return node_ != other.node_;
}

template< class T >
LCIter< T >::LCIter()
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
const T& LCIter< T >::operator*() const
{
  return node_->data;
}

template< class T >
const T* LCIter< T >::operator->() const
{
  return &(node_->data);
}

template< class T >
bool LCIter< T >::operator==(const LCIter& other) const
{
  return node_ == other.node_;
}

template< class T >
bool LCIter< T >::operator!=(const LCIter& other) const
{
  return node_ != other.node_;
}

template< class T >
List< T >::List()
  : head_(nullptr)
  , tail_(nullptr)
  , size_(0)
{
}

template< class T >
List< T >::~List()
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
  other.head_ = nullptr;
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
LIter< T > List< T >::begin()
{
  LIter< T > it;
  it.node_ = head_;
  it.list_ = this;
  return it;
}

template< class T >
LIter< T > List< T >::end()
{
  LIter< T > it;
  it.node_ = nullptr;
  it.list_ = this;
  return it;
}

template< class T >
LCIter< T > List< T >::begin() const
{
  LCIter< T > it;
  it.node_ = head_;
  it.list_ = this;
  return it;
}

template< class T >
LCIter< T > List< T >::end() const
{
  LCIter< T > it;
  it.node_ = nullptr;
  it.list_ = this;
  return it;
}

template< class T >
LCIter< T > List< T >::cbegin() const
{
  return begin();
}

template< class T >
LCIter< T > List< T >::cend() const
{
  return end();
}

template< class T >
bool List< T >::empty() const
{
  return size_ == 0;
}

template< class T >
size_t List< T >::size() const
{
  return size_;
}

template< class T >
void List< T >::clear()
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
void List< T >::pop_front()
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
void List< T >::pop_back()
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
T& List< T >::front()
{
  return head_->data;
}

template< class T >
T& List< T >::back()
{
  return tail_->data;
}

template< class T >
const T& List< T >::front() const
{
  return head_->data;
}

template< class T >
const T& List< T >::back() const
{
  return tail_->data;
}

}

#endif
