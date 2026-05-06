#ifndef _LIST_SEQUENCE_TPP_
#define _LIST_SEQUENCE_TPP_


template <typename T>
Sequence<T>* ListSequence<T>::AppendImplict(const T &item) {
    list.Append(item);
    return this;
}

template <typename T>
Sequence<T>* ListSequence<T>::PrependImplict(const T &item) {
    list.Prepend(item);
    return this;
}

template <typename T>
Sequence<T>* ListSequence<T>::InsertAtImplict(const T &item, int index) {
    list.InsertAt(item, index);
    return this;
}

template <typename T>
Sequence<T>* ListSequence<T>::ConcatImplict(const Sequence<T>* other) {
    if (other == nullptr)
        throw NullReferenceException("Null pointer in ListSequence::Concat");  
    IEnumerator<T> *iter = other->GetEnumerator();
    while (iter->HasNext()) {
        list.Append(iter->GetCurrent());
    }
    delete iter;
    return this;
}

template <typename T>
ListSequence<T>::ListSequence() : list(LinkedList<T>()) {}

template <typename T>
ListSequence<T>::ListSequence(const T* items, int count) 
    : list(LinkedList<T>(items, count)) {}

template <typename T>
ListSequence<T>::ListSequence(const LinkedList<T> &lst) 
    : list(LinkedList<T>(lst)) {}

template <typename T>
ListSequence<T>::ListSequence(const ListSequence<T> &other) 
    : list(LinkedList<T>(other.list)) {}

template <typename T>
ListSequence<T>::~ListSequence() {}

template <typename T>
ListSequence<T>& ListSequence<T>::operator=(const ListSequence<T>& other) {
    if (this != &other) {
        list = other.list;    
    }
    return *this;
}

template <typename T>
const T& ListSequence<T>::GetFirst() const {
    return list.GetFirst();
}

template <typename T>
const T& ListSequence<T>::GetLast() const {
    return list.GetLast();
}

template <typename T>
const T& ListSequence<T>::Get(int index) const {
    return list.Get(index);
}

template <typename T>
int ListSequence<T>::GetLength() const {
    return list.GetLength();
}

template <typename T>
Sequence<T>* ListSequence<T>::GetSubsequence(int startIndex, int endIndex) const {
    LinkedList<T> *tmp_list = list.GetSubList(startIndex, endIndex);
    Sequence<T>* sub_seq = CreateSequence(*tmp_list);
    delete tmp_list;
    return sub_seq;
}

template <typename T>
Sequence<T>* ListSequence<T>::Map(T (*func)(const T&)) const {
    LinkedList<T> mapped;
    IEnumerator<T> *iter = GetEnumerator();
    while (iter->HasNext()) {
        mapped.Append(func(iter->GetCurrent()));
    }
    Sequence<T>* result = CreateSequence(mapped);
    delete iter;
    return result;
}

template <typename T>
Sequence<T>* ListSequence<T>::Where(bool (*pred)(const T&)) const {
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

template <typename T>
void ListSequence<T>::Reduce(T (*func)(const T&, const T&), const T &init, T *result) const {
    T reduced = init;
    IEnumerator<T> *iter = GetEnumerator();
    while (iter->HasNext())
        reduced = func(reduced, iter->GetCurrent());
    *result = reduced;
    delete iter;
}

template <typename T>
Option<T> ListSequence<T>::TryGetFirst(bool (*pred)(const T&)) const {
    for (int i = 0; i < list.GetLength(); ++i) {
        const T& val = Get(i);
        if (!pred || pred(val))
            return Option<T>(val);
    }
    return Option<T>::None();
}

template <typename T>
Option<T> ListSequence<T>::TryGetLast(bool (*pred)(const T&)) const {
    for (int i = list.GetLength() - 1; i >= 0; --i) {
        const T& val = Get(i);
        if (!pred || pred(val))
            return Option<T>(val);
    }
    return Option<T>::None();
}

template <typename T>
IEnumerator<T>* ListSequence<T>::GetEnumerator() const {
    return list.GetEnumerator();
}

#endif /*_LIST_SEQUENCE_TPP_*/