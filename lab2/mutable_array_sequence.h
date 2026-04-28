#ifndef _MUTABLE_ARRAY_SEQUANCE_H_
#define _MUTABLE_ARRAY_SEQUANCE_H_

#include "array_sequence.h"

template <typename T>
class MutableArraySequence : public ArraySequence<T> {
protected:
    virtual Sequence<T>* Instance() override {
        return this;
    }

    virtual Sequence<T>* CreateSequence(const DynamicArray<T> &arr) const override {
        try {
            return new MutableArraySequence<T>(arr);
        } catch (...) {
            throw MemoryAllocationException("Failed to create mutable array sequence");
        }
    }

public:
    MutableArraySequence() :  ArraySequence<T>() {}
    MutableArraySequence(const T* items, int count) : ArraySequence<T>(items, count) {}
    explicit MutableArraySequence(int size) : ArraySequence<T>(size) {}
    MutableArraySequence(const DynamicArray<T> &arr) : ArraySequence<T>(arr) {}
    MutableArraySequence(const ArraySequence<T> &other) : ArraySequence<T>(other) {}
};

#endif /* _MUTABLE_ARRAY_SEQUANCE_H_ */