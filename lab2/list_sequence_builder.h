#ifndef _LIST_SEQUENCE_BUILDER_H_
#define _LIST_SEQUENCE_BUILDER_H_

#include "mutable_list_sequence.h"
#include "immutable_list_sequence.h"

template<typename T>
class ListSequenceBuilder {
private:
    LinkedList<T> buffer;
    bool buildImmutable;

public:
    ListSequenceBuilder() : buffer(), buildImmutable(false) {}
    
    ListSequenceBuilder& SetImmutable(bool val) {
        buildImmutable = val;
        return *this;
    }
    
    ListSequenceBuilder& Add(const T& item) {
        buffer.Append(item);
        return *this;
    }
    
    ListSequenceBuilder& AddToFront(const T& item) {
        buffer.Prepend(item);
        return *this;
    }
    
    ListSequenceBuilder& AddRange(const T* items, int count) {
        for (int i = 0; i < count; ++i) {
            buffer.Append(items[i]);
        }
        return *this;
    }
    
    ListSequenceBuilder& AddRange(const Sequence<T>* seq) {
        if (!seq) throw InvalidArgumentException("Null sequence in AddRange");
        for (int i = 0; i < seq->GetLength(); ++i) {
            buffer.Append(seq->Get(i));
        }
        return *this;
    }
    
    ListSequenceBuilder& Clear() {
        buffer = LinkedList<T>();
        return *this;
    }
    
    Sequence<T>* Build() const {
        if (buildImmutable) {
            return new ImmutableListSequence<T>(buffer);
        } else {
            return new MutableListSequence<T>(buffer);
        }
    }
    
    MutableListSequence<T>* BuildMutable() const {
        return new MutableListSequence<T>(buffer);
    }
    
    ImmutableListSequence<T>* BuildImmutable() const {
        return new ImmutableListSequence<T>(buffer);
    }
};

#endif /* _LIST_SEQUENCE_BUILDER_H_ */