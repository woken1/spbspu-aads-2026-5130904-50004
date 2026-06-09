#ifndef AUSHEV_HASH_TABLE_HPP
#define AUSHEV_HASH_TABLE_HPP

#include <functional>
#include <stdexcept>

namespace aushev {

template <typename Key, typename Value, typename Hash = std::hash<Key>, typename Equal = std::equal_to<Key>>
class HashTable {
public:
    using SlotUpdater = std::function<size_t(size_t)>;

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

    HashTable(size_t slots = 10) : slots_(slots), size_(0), buckets_(new Node*[slots]()),
        maxLoadFactor_(1.0), maxChainLength_(10), autoRehash_(false),
        slotUpdater_([](size_t before) { return before < 10 ? 20 : before * 2; }) {
        for (size_t i = 0; i < slots_; ++i) {
            buckets_[i] = nullptr;
        }
    }

    ~HashTable() {
        clear();
        delete[] buckets_;
    }

    HashTable(const HashTable& other) : slots_(other.slots_), size_(0), buckets_(new Node*[other.slots_]()),
        maxLoadFactor_(other.maxLoadFactor_), maxChainLength_(other.maxChainLength_),
        autoRehash_(other.autoRehash_), slotUpdater_(other.slotUpdater_) {
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
            maxLoadFactor_ = other.maxLoadFactor_;
            maxChainLength_ = other.maxChainLength_;
            autoRehash_ = other.autoRehash_;
            slotUpdater_ = other.slotUpdater_;
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
        if (autoRehash_) {
            if (loadFactor() > maxLoadFactor_ || maxChainLength() > maxChainLength_) {
                rehash(slotUpdater_(slots_));
            }
        }
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

    const Value& get(const Key& key) const {
        size_t index = hash_(key) % slots_;
        Node* current = buckets_[index];
        while (current != nullptr) {
            if (equal_(current->key_, key)) {
                return current->value_;
            }
            current = current->next_;
        }
        throw std::out_of_range("Key not found");
    }

    Value& getRef(const Key& key) {
        size_t index = hash_(key) % slots_;
        Node* current = buckets_[index];
        while (current != nullptr) {
            if (equal_(current->key_, key)) {
                return current->value_;
            }
            current = current->next_;
        }
        throw std::out_of_range("Key not found");
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

    size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    double loadFactor() const {
        return static_cast<double>(size_) / slots_;
    }

    size_t maxChainLength() const {
        size_t maxLen = 0;
        for (size_t i = 0; i < slots_; ++i) {
            size_t len = 0;
            Node* current = buckets_[i];
            while (current != nullptr) {
                ++len;
                current = current->next_;
            }
            if (len > maxLen) {
                maxLen = len;
            }
        }
        return maxLen;
    }

    void setMaxLoadFactor(double factor) {
        maxLoadFactor_ = factor;
    }

    void setMaxChainLength(size_t length) {
        maxChainLength_ = length;
    }

    void enableAutoRehash(bool enable) {
        autoRehash_ = enable;
    }

    void setSlotUpdater(SlotUpdater updater) {
        slotUpdater_ = updater;
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
    double maxLoadFactor_;
    size_t maxChainLength_;
    bool autoRehash_;
    SlotUpdater slotUpdater_;
};

}

#endif
