#ifndef _IMMUTABLE_ARRAY_SEQUANCE_H_
#define _IMMUTABLE_ARRAY_SEQUANCE_H_

#include "array_sequence.h"

template <typename T>
class ImmutableArraySequence : public ArraySequence<T> {
protected:
    virtual Sequence<T>* Instance() override {
        try {
            return new ImmutableArraySequence<T>(*this);
        } catch (...) {
            throw MemoryAllocationException("Failed to create changed array sequence");
        }
    }

    virtual Sequence<T>* CreateSequence(const DynamicArray<T> &arr) const override {
        try {
            return new ImmutableArraySequence<T>(arr);
        } catch (...) {
            throw MemoryAllocationException("Failed to create immutable array sequence");
        }
    }

public:
    ImmutableArraySequence() :  ArraySequence<T>() {}
    ImmutableArraySequence(const T* items, int count) : ArraySequence<T>(items, count) {}
    explicit ImmutableArraySequence(int size) : ArraySequence<T>(size) {}
    ImmutableArraySequence(const DynamicArray<T> &arr) : ArraySequence<T>(arr) {}
    ImmutableArraySequence(const ArraySequence<T> &other) : ArraySequence<T>(other) {}
};

#endif /* _MUTABLE_ARRAY_SEQUANCE_H_ */