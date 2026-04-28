#ifndef _IMMUTABLE_LIST_SEQUANCE_H_
#define _IMMUTABLE_LIST_SEQUANCE_H_

#include "list_sequence.h"

template <typename T>
class ImmutableListSequence : public ListSequence<T> {
protected:
    virtual Sequence<T>* Instance() override {
        try {
            return new ImmutableListSequence<T>(*this);
        } catch (...) {
            throw MemoryAllocationException("Failed to create changed list sequence");
        }
    }

    virtual Sequence<T>* CreateSequence(const LinkedList<T> &lst) const override {
        try {
            return new ImmutableListSequence<T>(lst);
        } catch (...) {
            throw MemoryAllocationException("Failed to create immutable list sequence");
        }
    };

public:
    ImmutableListSequence() :  ListSequence<T>() {}
    ImmutableListSequence(const T* items, int count) : ListSequence<T>(items, count) {}
    ImmutableListSequence(const LinkedList<T> &lst) : ListSequence<T>(lst) {}
    ImmutableListSequence(const ListSequence<T> &other) : ListSequence<T>(other) {}
};

#endif /* _MUTABLE_LIST_SEQUANCE_H_ */