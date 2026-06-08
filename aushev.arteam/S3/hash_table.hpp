#ifndef AUSHEV_HASH_TABLE_HPP
#define AUSHEV_HASH_TABLE_HPP

#include <functional>
#include <stdexcept>

namespace aushev {

template <typename Key, typename Value, typename Hash = std::hash<Key>, typename Equal = std::equal_to<Key>>
class HashTable {
public:
    struct Node {
        Key key_;
        Value value_;
        Node* next_;
        Node(const Key& key, const Value& value) : key_(key), value_(value), next_(nullptr) {}
    };

    class Iterator {
    public:
        Iterator(Node* node, HashTable* table) : node_(node), table_(table), bucketIndex_(0) {
            if (node_ == nullptr && table_ != nullptr) {
                findNext();
            }
        }
        Iterator& operator++() {
            if (node_ != nullptr) {
                node_ = node_->next_;
                if (node_ == nullptr) {
                    findNext();
                }
            }
            return *this;
        }
        bool operator!=(const Iterator& other) const {
            return node_ != other.node_;
        }
        std::pair<const Key, Value>& operator*() {
            return *reinterpret_cast<std::pair<const Key, Value>*>(node_);
        }
    private:
        void findNext() {
            while (bucketIndex_ < table_->slots_ && table_->buckets_[bucketIndex_] == nullptr) {
                ++bucketIndex_;
            }
            if (bucketIndex_ < table_->slots_) {
                node_ = table_->buckets_[bucketIndex_];
            }
        }
        Node* node_;
        HashTable* table_;
        size_t bucketIndex_;
    };

    class ConstIterator {
    public:
        ConstIterator(const Node* node, const HashTable* table) : node_(node), table_(table), bucketIndex_(0) {
            if (node_ == nullptr && table_ != nullptr) {
                findNext();
            }
        }
        ConstIterator& operator++() {
            if (node_ != nullptr) {
                node_ = node_->next_;
                if (node_ == nullptr) {
                    findNext();
                }
            }
            return *this;
        }
        bool operator!=(const ConstIterator& other) const {
            return node_ != other.node_;
        }
        const std::pair<const Key, Value>& operator*() const {
            return *reinterpret_cast<const std::pair<const Key, Value>*>(node_);
        }
    private:
        void findNext() {
            while (bucketIndex_ < table_->slots_ && table_->buckets_[bucketIndex_] == nullptr) {
                ++bucketIndex_;
            }
            if (bucketIndex_ < table_->slots_) {
                node_ = table_->buckets_[bucketIndex_];
            }
        }
        const Node* node_;
        const HashTable* table_;
        size_t bucketIndex_;
    };

    HashTable(size_t slots = 10) : slots_(slots), size_(0), buckets_(new Node*[slots]()) {
        for (size_t i = 0; i < slots_; ++i) {
            buckets_[i] = nullptr;
        }
    }

    ~HashTable() {
        clear();
        delete[] buckets_;
    }

    HashTable(const HashTable& other) : slots_(other.slots_), size_(0), buckets_(new Node*[other.slots_]()) {
        for (size_t i = 0; i < slots_; ++i) {
            buckets_[i] = nullptr;
        }
        for (size_t i = 0; i < other.slots_; ++i) {
            Node* current = other.buckets_[i];
            while (current != nullptr) {
                add(current->key_, current->value_);
                current = current->next_;
            }
        }
    }

    HashTable& operator=(const HashTable& other) {
        if (this != &other) {
            clear();
            delete[] buckets_;
            slots_ = other.slots_;
            size_ = 0;
            buckets_ = new Node*[slots_]();
            for (size_t i = 0; i < slots_; ++i) {
                buckets_[i] = nullptr;
            }
            for (size_t i = 0; i < other.slots_; ++i) {
                Node* current = other.buckets_[i];
                while (current != nullptr) {
                    add(current->key_, current->value_);
                    current = current->next_;
                }
            }
        }
        return *this;
    }

    void add(const Key& key, const Value& value) {
        size_t index = hash_(key) % slots_;
        Node* current = buckets_[index];
        while (current != nullptr) {
            if (equal_(current->key_, key)) {
                current->value_ = value;
                return;
            }
            current = current->next_;
        }
        Node* newNode = new Node(key, value);
        newNode->next_ = buckets_[index];
        buckets_[index] = newNode;
        ++size_;
    }

    bool has(const Key& key) const {
        size_t index = hash_(key) % slots_;
        Node* current = buckets_[index];
        while (current != nullptr) {
            if (equal_(current->key_, key)) {
                return true;
            }
            current = current->next_;
        }
        return false;
    }

    Value drop(const Key& key) {
        size_t index = hash_(key) % slots_;
        Node* current = buckets_[index];
        Node* prev = nullptr;
        while (current != nullptr) {
            if (equal_(current->key_, key)) {
                Value val = current->value_;
                if (prev == nullptr) {
                    buckets_[index] = current->next_;
                } else {
                    prev->next_ = current->next_;
                }
                delete current;
                --size_;
                return val;
            }
            prev = current;
            current = current->next_;
        }
        throw std::out_of_range("Key not found");
    }

    void rehash(size_t newSlots) {
        Node** newBuckets = new Node*[newSlots]();
        for (size_t i = 0; i < newSlots; ++i) {
            newBuckets[i] = nullptr;
        }
        for (size_t i = 0; i < slots_; ++i) {
            Node* current = buckets_[i];
            while (current != nullptr) {
                Node* next = current->next_;
                size_t newIndex = hash_(current->key_) % newSlots;
                current->next_ = newBuckets[newIndex];
                newBuckets[newIndex] = current;
                current = next;
            }
        }
        delete[] buckets_;
        buckets_ = newBuckets;
        slots_ = newSlots;
    }

    void clear() {
        for (size_t i = 0; i < slots_; ++i) {
            Node* current = buckets_[i];
            while (current != nullptr) {
                Node* next = current->next_;
                delete current;
                current = next;
            }
            buckets_[i] = nullptr;
        }
        size_ = 0;
    }

    Iterator begin() {
        return Iterator(nullptr, this);
    }

    Iterator end() {
        return Iterator(nullptr, nullptr);
    }

    ConstIterator begin() const {
        return ConstIterator(nullptr, this);
    }

    ConstIterator end() const {
        return ConstIterator(nullptr, nullptr);
    }

protected:
    size_t slots_;
    size_t size_;
    Node** buckets_;
    Hash hash_;
    Equal equal_;
};

}

#endif
