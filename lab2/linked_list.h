#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include <iostream>
#include "exceptions.h"
#include "ienumerable.h"

template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node *next;
        Node(const T &val, Node *n = nullptr) : data(val), next(n) {}
    };

    Node *first;
    Node *last;
    int length;

    Node* createNode(const T &val, Node *n = nullptr);

public:
    LinkedList();
    LinkedList(const T *items, int count);
    LinkedList(const LinkedList<T> &other);
    ~LinkedList();

    const T& GetFirst() const;
    const T& GetLast() const;
    const T& Get(int index) const;
    LinkedList<T>* GetSubList(int startIndex, int endIndex) const;
    int GetLength() const;

    void Append(const T &item);
    void Prepend(const T &item);
    void InsertAt(const T &item, int index);
    LinkedList<T>* Concat(const LinkedList<T> &other) const;

    LinkedList<T>& operator=(const LinkedList<T> &other);

    class LinkedListEnumerator : public IEnumerator<T>
    {
    private:
        const LinkedList<T> list; 
        const typename LinkedList<T>::Node *current;       
        bool started;                                
        
    public:
        LinkedListEnumerator(const LinkedList<T> lst); 
            
        bool HasNext() override;
        const T& GetCurrent() const override;
        void Reset() override;
    };

    IEnumerator<T>* GetEnumerator() const;
   
};

#include "linked_list.tpp"

#endif // _LINKED_LIST_H_