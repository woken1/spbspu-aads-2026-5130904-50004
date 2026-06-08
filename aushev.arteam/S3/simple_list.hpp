#ifndef AUSHEV_SIMPLE_LIST_HPP
#define AUSHEV_SIMPLE_LIST_HPP

#include <stdexcept>

namespace aushev {

template <typename T>
class SimpleList {
public:
    struct Node {
        T data_;
        Node* next_;
        Node(const T& data) : data_(data), next_(nullptr) {}
    };

    SimpleList() : head_(nullptr), size_(0) {}

    SimpleList(const SimpleList& other) : head_(nullptr), size_(0) {
        Node* current = other.head_;
        while (current != nullptr) {
            pushBack(current->data_);
            current = current->next_;
        }
    }

    SimpleList& operator=(const SimpleList& other) {
        if (this != &other) {
            clear();
            Node* current = other.head_;
            while (current != nullptr) {
                pushBack(current->data_);
                current = current->next_;
            }
        }
        return *this;
    }

    ~SimpleList() {
        clear();
    }

    void pushBack(const T& data) {
        Node* newNode = new Node(data);
        if (head_ == nullptr) {
            head_ = newNode;
        } else {
            Node* current = head_;
            while (current->next_ != nullptr) {
                current = current->next_;
            }
            current->next_ = newNode;
        }
        ++size_;
    }

    bool removeOne(const T& data) {
        if (head_ == nullptr) {
            return false;
        }
        if (head_->data_ == data) {
            Node* temp = head_;
            head_ = head_->next_;
            delete temp;
            --size_;
            return true;
        }
        Node* current = head_;
        while (current->next_ != nullptr) {
            if (current->next_->data_ == data) {
                Node* temp = current->next_;
                current->next_ = temp->next_;
                delete temp;
                --size_;
                return true;
            }
            current = current->next_;
        }
        return false;
    }

    void clear() {
        Node* current = head_;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next_;
            delete temp;
        }
        head_ = nullptr;
        size_ = 0;
    }

    size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    class Iterator {
    public:
        Iterator(Node* node) : node_(node) {}
        T& operator*() { return node_->data_; }
        Iterator& operator++() {
            node_ = node_->next_;
            return *this;
        }
        bool operator!=(const Iterator& other) const {
            return node_ != other.node_;
        }
    private:
        Node* node_;
    };

    Iterator begin() {
        return Iterator(head_);
    }

    Iterator end() {
        return Iterator(nullptr);
    }

private:
    Node* head_;
    size_t size_;
};

}

#endif
