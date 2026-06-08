#ifndef AUSHEV_DYNAMIC_ARRAY_HPP
#define AUSHEV_DYNAMIC_ARRAY_HPP

namespace aushev {

template <typename T>
class DynamicArray {
public:
    DynamicArray() : data_(nullptr), size_(0), capacity_(0) {}

    ~DynamicArray() {
        delete[] data_;
    }

    DynamicArray(const DynamicArray& other) : data_(nullptr), size_(0), capacity_(0) {
        reserve(other.capacity_);
        for (size_t i = 0; i < other.size_; ++i) {
            data_[i] = other.data_[i];
        }
        size_ = other.size_;
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data_;
            data_ = nullptr;
            size_ = 0;
            capacity_ = 0;
            reserve(other.capacity_);
            for (size_t i = 0; i < other.size_; ++i) {
                data_[i] = other.data_[i];
            }
            size_ = other.size_;
        }
        return *this;
    }

    void pushBack(const T& value) {
        if (size_ >= capacity_) {
            size_t newCapacity = capacity_ == 0 ? 4 : capacity_ * 2;
            reserve(newCapacity);
        }
        data_[size_++] = value;
    }

    void reserve(size_t newCapacity) {
        if (newCapacity <= capacity_) {
            return;
        }
        T* newData = new T[newCapacity];
        for (size_t i = 0; i < size_; ++i) {
            newData[i] = data_[i];
        }
        delete[] data_;
        data_ = newData;
        capacity_ = newCapacity;
    }

    size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    T& operator[](size_t index) {
        return data_[index];
    }

    const T& operator[](size_t index) const {
        return data_[index];
    }

    void sort() {
        for (size_t i = 0; i < size_; ++i) {
            for (size_t j = i + 1; j < size_; ++j) {
                if (data_[j] < data_[i]) {
                    T temp = data_[i];
                    data_[i] = data_[j];
                    data_[j] = temp;
                }
            }
        }
    }

private:
    T* data_;
    size_t size_;
    size_t capacity_;
};

}

#endif
