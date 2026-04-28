#ifndef _SEQUANCE_H_
#define _SEQUANCE_H_
#include "option.h"
#include "ienumerable.h"


template <typename T>
class Sequence : public IEnumerable<T> {
protected:
    virtual Sequence<T>* Instance() = 0;
    virtual Sequence<T>* AppendImplict(const T &item) = 0;
    virtual Sequence<T>* PrependImplict(const T &item) = 0;
    virtual Sequence<T>* InsertAtImplict(const T &item, int index) = 0;
    virtual Sequence<T>* ConcatImplict(const Sequence<T> *other) = 0;

public:
    virtual const T& GetFirst() const = 0;
    virtual const T& GetLast() const = 0;
    virtual const T& Get(int index) const = 0;
    virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) const = 0;
    virtual int GetLength() const = 0;

    virtual Sequence<T>* Append(const T &item) {
        return Instance()->AppendImplict(item);
    }

    virtual Sequence<T>* Prepend(const T &item) {
        return Instance()->PrependImplict(item);
    }

    virtual Sequence<T>* InsertAt(const T &item, int index) {
        return Instance()->InsertAtImplict(item, index);
    }

    virtual Sequence<T>* Concat(const Sequence<T> *other) {
        return Instance()->ConcatImplict(other);
    }

    virtual Sequence<T>* Map(T (*func)(const T&)) const = 0;
    virtual Sequence<T>* Where(bool (*pred)(const T&)) const = 0;
    virtual void Reduce(T (*func)(const T&, const T&), const T &init, T *result) const = 0;
    virtual Option<T> TryGetFirst(bool (*pred)(const T&) = nullptr) const = 0;
    virtual Option<T> TryGetLast(bool (*pred)(const T&) = nullptr) const = 0;

    virtual IEnumerator<T>* GetEnumerator() const = 0;

    virtual const T& operator[](int index) {
        return Get(index);
    };

    virtual ~Sequence() {}
};

#endif /* _SEQUANCE_H_ */