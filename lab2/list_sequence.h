#ifndef _LIST_SEQUANCE_H_
#define _LIST_SEQUANCE_H_

#include "linked_list.h"
#include "sequence.h"

template <typename T>
class ListSequence : public Sequence<T> {
private:
    LinkedList<T> list; 

protected:
    virtual Sequence<T>* CreateSequence(const LinkedList<T> &lst) const = 0;
    virtual Sequence<T>* Instance() = 0;
    virtual Sequence<T>* AppendImplict(const T &item) override {
        list.Append(item);
        return this;
    };

    virtual Sequence<T>* PrependImplict(const T &item) override {
        list.Prepend(item);
        return this;
    }

    virtual Sequence<T>* InsertAtImplict(const T &item, int index) override {
        list.InsertAt(item, index);
        return this;
    }

    virtual Sequence<T>* ConcatImplict(const Sequence<T>* other) override {
        if (other == nullptr)
            throw NullReferenceException("Null pointer in ListSequence::Concat");  
        IEnumerator<T> *iter = other->GetEnumerator();
        while (iter->HasNext()) {
            list.Append(iter->GetCurrent());
        }
        delete iter;
        return this;
    }

public:
    ListSequence() : list(LinkedList<T>()) {}
    ListSequence(const T* items, int count) : list(LinkedList<T>(items, count)) {}
    ListSequence(const LinkedList<T> &lst) : list(LinkedList<T>(lst)) {}
    ListSequence(const ListSequence<T> &other) : list(LinkedList<T>(other.list)) {}
    ~ListSequence() override {}

    ListSequence<T>& operator=(const ListSequence<T>& other) {
        if (this != &other) {
            list = other.list;    
        }
        return *this;
    }

    const T& GetFirst() const override {
        return list.GetFirst();
    }

    const T& GetLast() const override {
        return list.GetLast();
    }

    const T& Get(int index) const override {
        return list.Get(index);
    }

    int GetLength() const override {
        return list.GetLength();
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override 
    {
        LinkedList<T> *tmp_list = list.GetSubList(startIndex, endIndex);
        Sequence<T>* sub_seq = CreateSequence(*tmp_list);
        delete tmp_list;
        return sub_seq;
    }
    
    Sequence<T>* Map(T (*func)(const T&)) const override {
        LinkedList<T> mapped;
        IEnumerator<T> *iter = GetEnumerator();
        while (iter->HasNext()) {
            mapped.Append(func(iter->GetCurrent()));
        }
        Sequence<T>* result = CreateSequence(mapped);
        delete iter;
        return result;
    }

    Sequence<T>* Where(bool (*pred)(const T&)) const override {
        LinkedList<T> filtered;
        IEnumerator<T> *iter = GetEnumerator();
        while (iter->HasNext()) {
            T val = iter->GetCurrent();
            if (pred(val))
                filtered.Append(val);
        }
        Sequence<T>* result = CreateSequence(filtered);
        delete iter;
        return result;
    }

    void Reduce(T (*func)(const T&, const T&), const T &init, T *result) const override {
        T reduced = init;
        IEnumerator<T> *iter = GetEnumerator();
        while (iter->HasNext())
            reduced = func(reduced, iter->GetCurrent());
        *result = reduced;
        delete iter;
    }

    Option<T> TryGetFirst(bool (*pred)(const T&) = nullptr) const {
        for (int i = 0; i < list.GetLength(); ++i) {
            const T& val = Get(i);
            if (!pred || pred(val))
                return Option<T>(val);
        }
        return Option<T>::None();
    }

    Option<T> TryGetLast(bool (*pred)(const T&) = nullptr) const override {
        for (int i = list.GetLength() - 1; i >= 0; --i) {
            const T& val = Get(i);
            if (!pred || pred(val))
                return Option<T>(val);
        }
        return Option<T>::None();
    }

    IEnumerator<T>* GetEnumerator() const override {
        return list.GetEnumerator();
    }

};

#endif /* _LIST_SEQUANCE_H_ */