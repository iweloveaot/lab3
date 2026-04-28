#ifndef _DYNAMIC_ARRAY_H_
#define _DUNAMIC_ARRAY_H_

#include "exceptions.h"

template <class T>
T* allocateMemory(int size) {
    if (size == 0) {
        return nullptr;
    }
    T* ptr = nullptr;
    try {
        ptr = new T[size]();
    } catch (...) {
        throw MemoryAllocationException("Failed to create dynamic array");
    }
    return ptr;
}

template <typename T>
class DynamicArray {
private:

    T* data;
    int size;
    int capacity;

    void ensureCapacity(int newSize) {
        if (newSize <= capacity) 
            return;
        int newCap = capacity * 2;
        if (newCap < newSize) newCap = newSize;
        T* newData = allocateMemory<T>(newCap);
        for (int i = 0; i < size; ++i)
            newData[i] = data[i];
        delete[] data;
        data = newData;
        capacity = newCap;
    }

public:
    DynamicArray() : data(nullptr), size(0), capacity(0) {}

    DynamicArray(const T *items, int count) : size(count), capacity(count) {
        if (count < 0) 
            throw InvalidArgumentException("Negative count for dynamic array:", count);
        if (items == nullptr && count > 0)
            throw NullReferenceException("Null pointer for dynamic array");
        data = allocateMemory<T>(capacity);
        for (int i = 0; i < count; i++)
            data[i] = items[i];
    }

    explicit DynamicArray(int size) : size(size), capacity(size) {
        if (size < 0) throw InvalidArgumentException("Negative size for dynamic array:", size);
        data = allocateMemory<T>(capacity);
        for (int i=0; i<size; i++)
            data[i] = T();
    }

    DynamicArray(const DynamicArray<T>& other) : size(other.size), capacity(other.capacity) {
        if (other.data == nullptr && other.size > 0)
            throw NullReferenceException("Null pointer when copying a dynamic array");
        data = allocateMemory<T>(capacity);
        for (int i=0; i<size; i++)
            data[i] = other.data[i];
    }

    ~DynamicArray() {
        delete[] data;
    }

    const T& Get(int index) const {
        if (index < 0 || index >= size)
            throw IndexOutOfRangeException("Index out of range in DynamicArray::Get:", index);
        return data[index];
    }

    void Set(int index, T value) {
        if (index < 0 || index >= size)
            throw IndexOutOfRangeException("Index out of range in DynamicArray::Set:", index);
        data[index] = value;
    }

    int GetSize() const { 
        return size; 
    }

    void Resize(int newSize) {
        if (newSize < 0) 
            throw InvalidArgumentException("Negative new size for dynamic array:", newSize);
        if (newSize == size)    
            return;
        ensureCapacity(newSize);
        if (newSize < size) {
            T *newData = allocateMemory<T>(capacity);
            for (int i = 0; i < newSize; i++)
                newData[i] = data[i];
            delete[] data;
            data = newData;
        }
        if (newSize > size) {
            for (int i=size; i<newSize; i++)
                data[i] = T();
        }
        size = newSize;
    }

    DynamicArray<T>& operator=(const DynamicArray<T> &other) {
        if (other.data == nullptr && other.size > 0)
            throw NullReferenceException("Null pointer when copying a dynamic array");
        if (this != &other) {
            delete[] data;
            size = other.size;
            capacity = other.capacity;
            data = allocateMemory<T>(capacity);
            for (int i = 0; i < size; ++i)
                data[i] = other.data[i];
        }
        return *this;
    }
};

#endif // _DYNAMIC_ARRAY_H_