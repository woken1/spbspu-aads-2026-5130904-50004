#ifndef NODE_H
#define NODE_H

namespace aushev {
  
  template< class T >
  struct Node
  {
    T data;
    Node* next;
    Node* prev;
  };
  
}

#endif
