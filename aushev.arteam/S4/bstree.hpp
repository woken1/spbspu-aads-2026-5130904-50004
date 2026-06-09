#ifndef AUSHEV_BSTREE_HPP
#define AUSHEV_BSTREE_HPP

#include <utility>
#include <stdexcept>
#include <functional>

namespace aushev {

template <typename Key, typename Value, typename Compare = std::less<Key>>
class BSTree {
    struct Node {
        std::pair<const Key, Value> data_;
        Node* left_;
        Node* right_;
        Node* parent_;

        Node(const Key& k, const Value& val, Node* p) :
            data_(k, val),
            left_(nullptr),
            right_(nullptr),
            parent_(p) {}
    };

    Node* root_;
    size_t size_;
    Compare compare_;

    size_t getHeight(const Node* node) const {
        if (!node) {
            return 0;
        }
        size_t leftH = getHeight(node->left_);
        size_t rightH = getHeight(node->right_);
        return 1 + (leftH > rightH ? leftH : rightH);
    }

    void clearTree(Node* node) {
        if (!node) {
            return;
        }
        clearTree(node->left_);
        clearTree(node->right_);
        delete node;
    }

public:
    class CIterator {
        const Node* current_;
    public:
        friend class BSTree;

        CIterator(const Node* cur) : current_(cur) {}

        const std::pair<const Key, Value>& operator*() const {
            return current_->data_;
        }

        const std::pair<const Key, Value>* operator->() const {
            return &(current_->data_);
        }

        CIterator& operator++() {
            if (current_->right_) {
                current_ = current_->right_;
                while (current_->left_) {
                    current_ = current_->left_;
                }
            } else {
                const Node* parent = current_->parent_;
                while (parent && current_ == parent->right_) {
                    current_ = parent;
                    parent = parent->parent_;
                }
                current_ = parent;
            }
            return *this;
        }

        bool operator==(const CIterator& other) const {
            return this->current_ == other.current_;
        }

        bool operator!=(const CIterator& other) const {
            return !(*this == other);
        }
    };

    class Iterator {
        Node* current_;
    public:
        friend class BSTree;

        Iterator(Node* cur) : current_(cur) {}

        operator CIterator() const {
            return CIterator(this->current_);
        }

        std::pair<const Key, Value>& operator*() {
            return current_->data_;
        }

        std::pair<const Key, Value>* operator->() {
            return &(current_->data_);
        }

        Iterator& operator++() {
            if (current_->right_) {
                current_ = current_->right_;
                while (current_->left_) {
                    current_ = current_->left_;
                }
            } else {
                Node* parent = current_->parent_;
                while (parent && current_ == parent->right_) {
                    current_ = parent;
                    parent = parent->parent_;
                }
                current_ = parent;
            }
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return this->current_ == other.current_;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };

    BSTree(const Compare& com = Compare()) :
        root_(nullptr),
        size_(0),
        compare_(com) {}

    BSTree(const BSTree&) = delete;
    BSTree& operator=(const BSTree&) = delete;

    ~BSTree() {
        clearTree(root_);
    }

    Iterator begin() {
        if (!root_) {
            return Iterator(nullptr);
        }
        Node* node = root_;
        while (node->left_) {
            node = node->left_;
        }
        return Iterator(node);
    }

    Iterator end() {
        return Iterator(nullptr);
    }

    CIterator cbegin() const {
        if (!root_) {
            return CIterator(nullptr);
        }
        const Node* node = root_;
        while (node->left_) {
            node = node->left_;
        }
        return CIterator(node);
    }

    CIterator cend() const {
        return CIterator(nullptr);
    }

    void push(const Key& k, const Value& v) {
        Node* node = root_;
        Node* parent = nullptr;

        while (node) {
            parent = node;
            if (!compare_(node->data_.first, k) && !compare_(k, node->data_.first)) {
                node->data_.second = v;
                return;
            } else if (compare_(node->data_.first, k)) {
                node = node->right_;
            } else {
                node = node->left_;
            }
        }

        Node* newNode = new Node(k, v, parent);
        if (!parent) {
            root_ = newNode;
        } else if (compare_(k, parent->data_.first)) {
            parent->left_ = newNode;
        } else {
            parent->right_ = newNode;
        }
        ++size_;
    }

    Value get(const Key& k) const {
        Node* node = root_;
        while (node) {
            if (!compare_(node->data_.first, k) && !compare_(k, node->data_.first)) {
                return node->data_.second;
            } else if (compare_(node->data_.first, k)) {
                node = node->right_;
            } else {
                node = node->left_;
            }
        }
        throw std::out_of_range("key not found");
    }

    Value drop(const Key& k) {
        Node* target = root_;
        Node* parent = nullptr;
        bool isLeft = false;

        while (target) {
            if (!compare_(target->data_.first, k) && !compare_(k, target->data_.first)) {
                break;
            }
            parent = target;
            if (compare_(target->data_.first, k)) {
                target = target->right_;
                isLeft = false;
            } else {
                target = target->left_;
                isLeft = true;
            }
        }

        if (!target) {
            throw std::out_of_range("key not found");
        }

        Value result = target->data_.second;

        if (!target->left_ || !target->right_) {
            Node* child = target->left_ ? target->left_ : target->right_;
            if (child) {
                child->parent_ = parent;
            }
            if (!parent) {
                root_ = child;
            } else if (isLeft) {
                parent->left_ = child;
            } else {
                parent->right_ = child;
            }
            delete target;
            --size_;
            return result;
        }

        Node* succ = target->right_;
        while (succ->left_) {
            succ = succ->left_;
        }

        Node* succParent = succ->parent_;
        bool succIsLeft = (succParent && succ == succParent->left_);

        if (succParent) {
            if (succIsLeft) {
                succParent->left_ = succ->right_;
            } else {
                succParent->right_ = succ->right_;
            }
            if (succ->right_) {
                succ->right_->parent_ = succParent;
            }
        }

        succ->parent_ = parent;
        if (!parent) {
            root_ = succ;
        } else if (isLeft) {
            parent->left_ = succ;
        } else {
            parent->right_ = succ;
        }

        succ->left_ = target->left_;
        if (target->left_) {
            target->left_->parent_ = succ;
        }
        succ->right_ = target->right_;
        if (target->right_) {
            target->right_->parent_ = succ;
        }

        delete target;
        --size_;
        return result;
    }

    bool contains(const Key& k) const {
        Node* node = root_;
        while (node) {
            if (!compare_(node->data_.first, k) && !compare_(k, node->data_.first)) {
                return true;
            } else if (compare_(node->data_.first, k)) {
                node = node->right_;
            } else {
                node = node->left_;
            }
        }
        return false;
    }

    size_t height() const {
        return getHeight(root_);
    }

    size_t height(CIterator it) const {
        return getHeight(it.current_);
    }

    void clear() {
        clearTree(root_);
        root_ = nullptr;
        size_ = 0;
    }

    CIterator rotateLeft(CIterator it) {
        Node* cur = const_cast<Node*>(it.current_);
        if (!cur || !cur->right_) {
            return it;
        }
        Node* right = cur->right_;

        if (!cur->parent_) {
            root_ = right;
        } else if (cur == cur->parent_->left_) {
            cur->parent_->left_ = right;
        } else if (cur == cur->parent_->right_) {
            cur->parent_->right_ = right;
        }

        right->parent_ = cur->parent_;
        cur->right_ = right->left_;
        if (right->left_) {
            right->left_->parent_ = cur;
        }
        cur->parent_ = right;
        right->left_ = cur;

        return CIterator(right);
    }

    CIterator rotateRight(CIterator it) {
        Node* cur = const_cast<Node*>(it.current_);
        if (!cur || !cur->left_) {
            return it;
        }
        Node* left = cur->left_;

        if (!cur->parent_) {
            root_ = left;
        } else if (cur == cur->parent_->left_) {
            cur->parent_->left_ = left;
        } else if (cur == cur->parent_->right_) {
            cur->parent_->right_ = left;
        }

        left->parent_ = cur->parent_;
        cur->left_ = left->right_;
        if (left->right_) {
            left->right_->parent_ = cur;
        }
        cur->parent_ = left;
        left->right_ = cur;

        return CIterator(left);
    }

    CIterator rotateLargeLeft(CIterator it) {
        Node* node = const_cast<Node*>(it.current_);
        if (node && node->left_) {
            rotateRight(CIterator(node->left_));
        }
        return rotateLeft(it);
    }

    CIterator rotateLargeRight(CIterator it) {
        Node* node = const_cast<Node*>(it.current_);
        if (node && node->right_) {
            rotateLeft(CIterator(node->right_));
        }
        return rotateRight(it);
    }
};

}

#endif
