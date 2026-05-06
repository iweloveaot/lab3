#ifndef _MUTABLE_LIST_SEQUANCE_TPP_
#define _MUTABLE_LIST_SEQUANCE_TPP_

template <typename T>
Sequence<T>* MutableListSequence<T>::Instance() {
    return this;
}

template <typename T>
Sequence<T>* MutableListSequence<T>::CreateSequence(const LinkedList<T> &lst) const {
    try {
        return new MutableListSequence<T>(lst);
    } catch (...) {
        throw MemoryAllocationException("Failed to create mutable list sequence");
    }
}

template <typename T>
MutableListSequence<T>::MutableListSequence() 
    : ListSequence<T>() {}

template <typename T>
MutableListSequence<T>::MutableListSequence(const T* items, int count) 
    : ListSequence<T>(items, count) {}

template <typename T>
MutableListSequence<T>::MutableListSequence(const LinkedList<T> &lst) 
    : ListSequence<T>(lst) {}

template <typename T>
MutableListSequence<T>::MutableListSequence(const ListSequence<T> &other) 
    : ListSequence<T>(other) {}

#endif /* _MUTABLE_LIST_SEQUANCE_TPP_ */