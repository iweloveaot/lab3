#ifndef _IMMUTABLE_LIST_SEQUANCE_TPP_
#define _IMMUTABLE_LIST_SEQUANCE_TPP_


template <typename T>
Sequence<T>* ImmutableListSequence<T>::Instance() {
    try {
        return new ImmutableListSequence<T>(*this);
    } catch (...) {
        throw MemoryAllocationException("Failed to create changed list sequence");
    }
}

template <typename T>
Sequence<T>* ImmutableListSequence<T>::CreateSequence(const LinkedList<T> &lst) const {
    try {
        return new ImmutableListSequence<T>(lst);
    } catch (...) {
        throw MemoryAllocationException("Failed to create immutable list sequence");
    }
}

template <typename T>
ImmutableListSequence<T>::ImmutableListSequence() 
    : ListSequence<T>() {}

template <typename T>
ImmutableListSequence<T>::ImmutableListSequence(const T* items, int count) 
    : ListSequence<T>(items, count) {}

template <typename T>
ImmutableListSequence<T>::ImmutableListSequence(const LinkedList<T> &lst) 
    : ListSequence<T>(lst) {}

template <typename T>
ImmutableListSequence<T>::ImmutableListSequence(const ListSequence<T> &other) 
    : ListSequence<T>(other) {}

#endif /* _MUTABLE_LIST_SEQUANCE_TPP_ */