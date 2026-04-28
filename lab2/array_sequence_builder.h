#ifndef _ARRAY_SEQUENCE_BUILDER_H_
#define _ARRAY_SEQUENCE_BUILDER_H_

#include "mutable_array_sequence.h"
#include "immutable_array_sequence.h"

template<typename T>
class ArraySequenceBuilder {
private:
    DynamicArray<T> buffer;
    bool buildImmutable;

public:
    ArraySequenceBuilder() : buffer(), buildImmutable(false) {}
    
    ArraySequenceBuilder& SetImmutable(bool val) {
        buildImmutable = val;
        return *this;
    }
    
    ArraySequenceBuilder& Add(const T& item) {
        int size = buffer.GetSize();
        buffer.Resize(size + 1);
        buffer.Set(size, item);
        return *this;
    }
    
    ArraySequenceBuilder& AddRange(const T* items, int count) {
        for (int i = 0; i < count; ++i) {
            Add(items[i]);
        }
        return *this;
    }
    
    ArraySequenceBuilder& AddRange(const Sequence<T>* seq) {
        if (!seq) throw InvalidArgumentException("Null sequence in AddRange");
        for (int i = 0; i < seq->GetLength(); ++i) {
            Add(seq->Get(i));
        }
        return *this;
    }
    
    ArraySequenceBuilder& Clear() {
        buffer = DynamicArray<T>();
        return *this;
    }
    
    Sequence<T>* Build() const {
        if (buildImmutable) {
            return new ImmutableArraySequence<T>(buffer);
        } else {
            return new MutableArraySequence<T>(buffer);
        }
    }
    
    MutableArraySequence<T>* BuildMutable() const {
        return new MutableArraySequence<T>(buffer);
    }
    
    ImmutableArraySequence<T>* BuildImmutable() const {
        return new ImmutableArraySequence<T>(buffer);
    }
};

#endif /* _ARRAY_SEQUENCE_BUILDER_H_ */