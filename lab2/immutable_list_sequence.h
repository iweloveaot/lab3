#ifndef _IMMUTABLE_LIST_SEQUENCE_H_
#define _IMMUTABLE_LIST_SEQUENCE_H_

#include "list_sequence.h"

template <typename T>
class ImmutableListSequence : public ListSequence<T> {
protected:
    virtual Sequence<T>* Instance() override;
    virtual Sequence<T>* CreateSequence(const LinkedList<T> &lst) const override;

public:
    ImmutableListSequence();
    ImmutableListSequence(const T* items, int count);
    ImmutableListSequence(const LinkedList<T> &lst);
    ImmutableListSequence(const ListSequence<T> &other);
};

#include "immutable_list_sequence.tpp"

#endif // _IMMUTABLE_LIST_SEQUENCE_H_