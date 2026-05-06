#ifndef _MUTABLE_LIST_SEQUENCE_H_
#define _MUTABLE_LIST_SEQUENCE_H_

#include "list_sequence.h"

template <typename T>
class MutableListSequence : public ListSequence<T> {
protected:
    virtual Sequence<T>* Instance() override;
    virtual Sequence<T>* CreateSequence(const LinkedList<T> &lst) const override;

public:
    MutableListSequence();
    MutableListSequence(const T* items, int count);
    MutableListSequence(const LinkedList<T> &lst);
    MutableListSequence(const ListSequence<T> &other);
};

#include "mutable_list_sequence.tpp"

#endif // _MUTABLE_LIST_SEQUENCE_H_