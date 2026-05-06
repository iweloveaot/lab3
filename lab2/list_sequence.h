#ifndef _LIST_SEQUENCE_H_
#define _LIST_SEQUENCE_H_

#include "linked_list.h"
#include "sequence.h"

template <typename T>
class ListSequence : public Sequence<T> {
private:
    LinkedList<T> list;

protected:
    virtual Sequence<T>* CreateSequence(const LinkedList<T> &lst) const = 0;
    virtual Sequence<T>* Instance() = 0;

    virtual Sequence<T>* AppendImplict(const T &item) override;
    virtual Sequence<T>* PrependImplict(const T &item) override;
    virtual Sequence<T>* InsertAtImplict(const T &item, int index) override;
    virtual Sequence<T>* ConcatImplict(const Sequence<T>* other) override;

public:
    ListSequence();
    ListSequence(const T* items, int count);
    ListSequence(const LinkedList<T> &lst);
    ListSequence(const ListSequence<T> &other);
    ~ListSequence() override;

    ListSequence<T>& operator=(const ListSequence<T>& other);

    const T& GetFirst() const override;
    const T& GetLast() const override;
    const T& Get(int index) const override;
    int GetLength() const override;
    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override;
    Sequence<T>* Map(T (*func)(const T&)) const override;
    Sequence<T>* Where(bool (*pred)(const T&)) const override;
    void Reduce(T (*func)(const T&, const T&), const T &init, T *result) const override;

    Option<T> TryGetFirst(bool (*pred)(const T&) = nullptr) const;
    Option<T> TryGetLast(bool (*pred)(const T&) = nullptr) const override;

    IEnumerator<T>* GetEnumerator() const override;
};

#include "list_sequence.tpp"

#endif // _LIST_SEQUENCE_H_