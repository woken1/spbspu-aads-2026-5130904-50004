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
    BSTree(const Compare& com = Compare()) :
        root_(nullptr),
        size_(0),
        compare_(com) {}

    BSTree(const BSTree&) = delete;
    BSTree& operator=(const BSTree&) = delete;

    ~BSTree() {
        clearTree(root_);
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
};

}

#endif
