#ifndef _ARRAY_SEQUANCE_H_
#define _ARRAY_SEQUANCE_H_

#include "exceptions.h"
#include "dynamic_array.h"
#include "sequence.h"

template <typename T>
class ArraySequence : public Sequence<T> {
private:
    DynamicArray<T> array; 

protected:
    virtual Sequence<T>* CreateSequence(const DynamicArray<T> &arr) const = 0;
    virtual Sequence<T>* Instance() = 0;
    virtual Sequence<T>* AppendImplict(const T &item) override {
        int size = array.GetSize();
        array.Resize(size+1);
        array.Set(size, item);
        return this;
    };

    virtual Sequence<T>* PrependImplict(const T &item) override {  // переписать без препенда
        int size = array.GetSize();
        DynamicArray<T> tmp_arr = DynamicArray<T>(size+1);
        for (int i = 1; i <= array.GetSize(); i++) {
            tmp_arr.Set(i, array.Get(i-1));
        }
        tmp_arr.Set(0, item);
        array = tmp_arr;
        return this;
    }

    virtual Sequence<T>* InsertAtImplict(const T &item, int index) override { 
        if (index < 0 || index > array.GetSize())
            throw IndexOutOfRangeException("Index out of range in Sequence::InsertAt");
        
        if (index == array.GetSize())
            AppendImplict(item);
        else if (index == 0)
            PrependImplict(item);
        else {
            DynamicArray<T> tmp_arr = DynamicArray<T>(array.GetSize()+1);
            for (int i = 0; i < index; i++)
                tmp_arr.Set(i, array.Get(i));
            tmp_arr.Set(index, item);
            for (int i = index; i < array.GetSize(); i++)
                tmp_arr.Set(i+1, array.Get(i));
            array = tmp_arr;
        }
        return this;
    }

    virtual Sequence<T>* ConcatImplict(const Sequence<T>* other) override {
        if (other == nullptr)
            throw NullReferenceException("Null pointer in ArraySequence::Concat");  
        for (int i = 0; i < other->GetLength(); i++) {
            AppendImplict(other->Get(i));
        }
        return this;
    }

public:
    ArraySequence() : array(DynamicArray<T>()) {}
    ArraySequence(const T* items, int count) : array(DynamicArray<T>(items, count)) {}
    explicit ArraySequence(int size) : array(DynamicArray<T>(size)) {}
    ArraySequence(const DynamicArray<T> &arr) : array(DynamicArray<T>(arr)) {}
    ArraySequence(const ArraySequence<T> &other) : array(DynamicArray<T>(other.array)) {}
    ~ArraySequence() override {}

    ArraySequence<T>& operator=(const ArraySequence<T>& other) {
        if (this != &other) {
            array = other.array;    
        }
        return *this;
    }

    const T& GetFirst() const override {
        if (array.GetSize() == 0)
            throw EmptyContainerException("Array sequence is empty");
        return array.Get(0);
    }

    const T& GetLast() const override {
        if (array.GetSize() == 0)
            throw EmptyContainerException("Array sequence is empty");
        return array.Get(array.GetSize()-1);
    }

    const T& Get(int index) const override {
        return array.Get(index);
    }

    int GetLength() const override {
        return array.GetSize();
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override 
    {
        if (startIndex < 0 || endIndex >= array.GetSize() || startIndex > endIndex)
            throw IndexOutOfRangeException("Invalid subsequence indexes");
        DynamicArray<T> tmp_array = DynamicArray<T>(endIndex-startIndex+1);
        int tmp_ind = 0;
        for (int i = startIndex; i <= endIndex; i++) {
            tmp_array.Set(tmp_ind, array.Get(i));
            tmp_ind++;
        }
        Sequence<T>* sub_seq = CreateSequence(tmp_array);
        return sub_seq;
    }

    Sequence<T>* Map(T (*func)(const T&)) const override {
        int size = array.GetSize();
        DynamicArray<T> mapped = DynamicArray<T>(size);
        for (int i = 0; i < size; i++) {
            T val = array.Get(i);
            mapped.Set(i, func(val));
        }
        Sequence<T>* result = CreateSequence(mapped);
        return result;
    }

    Sequence<T>* Where(bool (*pred)(const T&)) const override {
        int size = array.GetSize();
        DynamicArray<T> filtered;
        for (int i = 0; i < size; i++) {
            T val = array.Get(i);
            if (pred(val)) {
                filtered.Resize(filtered.GetSize() + 1);
                filtered.Set(filtered.GetSize() - 1, val);
            }
        }
        Sequence<T>* result = CreateSequence(filtered);
        return result;
    }

    void Reduce(T (*func)(const T&, const T&), const T &init, T *result) const override {
        int size = array.GetSize();
        T reduced = init;
        for (int i = 0; i < size; i++)
            reduced = func(reduced, array.Get(i));
        *result = reduced;
    }

    Option<T> TryGetFirst(bool (*pred)(const T&) = nullptr) const {
        for (int i = 0; i < array.GetSize(); i++) {
            const T& val = Get(i);
            if (!pred || pred(val))
                return Option<T>(val);
        }
        return Option<T>::None();
    }

    Option<T> TryGetLast(bool (*pred)(const T&) = nullptr) const {
        for (int i = array.GetSize() - 1; i >= 0; i--) {
            const T& val = Get(i);
            if (!pred || pred(val))
                return Option<T>(val);
        }
        return Option<T>::None();
    }

    class ArrayEnumerator : public IEnumerator<T> {
    private:
        const ArraySequence<T>& arr_seq;
        int currentIndex;
    public:
        ArrayEnumerator(const ArraySequence<T>& s) : arr_seq(s), currentIndex(-1) {}
            
        bool HasNext() override {
            if (currentIndex + 1 < arr_seq.GetLength()) {
                ++currentIndex;
                return true;
            }
            return false;
        }

        const T& GetCurrent() const override {
            if (currentIndex < 0 || currentIndex >= arr_seq.GetLength())
                throw IndexOutOfRangeException("Enumerator not positioned");
            return arr_seq.Get(currentIndex);
        }
        
        void Reset() override { 
            currentIndex = -1; 
        }
    };

    IEnumerator<T>* GetEnumerator() const override {
        return new ArrayEnumerator(*this);
    }

};
    
#endif /* _ARRAY_SEQUANCE_H_ */