#ifndef _LINKED_LIST_TPP_
#define _LINKED_LIST_TPP_


template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::createNode(const T &val, Node *n) {
    Node *newNode = nullptr;
    try {
        newNode = new Node(val, n);
    } catch (...) {
        throw MemoryAllocationException();
    }
    return newNode;
}


template <typename T>
LinkedList<T>::LinkedList() : first(nullptr), last(nullptr), length(0) {}

template <typename T>
LinkedList<T>::LinkedList(const T *items, int count) : first(nullptr), last(nullptr), length(0) {
    if (count < 0)
        throw InvalidArgumentException("Negative count for linked list");
    for (int i = 0; i < count; i++) {
        Append(items[i]);
    }
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T> &other) : first(nullptr), last(nullptr), length(0) {
    Node *current = other.first; 
    while (current) {
        Append(current->data);            
        current = current->next;
    }
}

template <typename T>
LinkedList<T>::~LinkedList() {
    while (first) {
        Node *tmp = first;
        first = first->next;
        delete tmp;
    }
}

template <typename T>
const T& LinkedList<T>::GetFirst() const {
    if (length == 0) 
        throw EmptyContainerException("List is empty");
    return first->data;
}

template <typename T>
const T& LinkedList<T>::GetLast() const {
    if (length == 0) 
        throw EmptyContainerException("List is empty");
    return last->data;
}

template <typename T>
const T& LinkedList<T>::Get(int index) const { 
    if (index < 0 || index >= length)
        throw IndexOutOfRangeException("Index out of range in LinkedList::Get");
    Node *current = first;
    for (int i = 0; i < index; i++)
        current = current->next;
    return current->data;
}

template <typename T>
LinkedList<T>* LinkedList<T>::GetSubList(int startIndex, int endIndex) const {
    if (startIndex < 0)
        throw IndexOutOfRangeException("Negative index for sublist");
    else if (endIndex >= length)
        throw IndexOutOfRangeException("Invalid sublist end index");
    else if (startIndex > endIndex)
        throw IndexOutOfRangeException("Invalid sublist indexes (start > end)");

    LinkedList<T> *sub_list = nullptr;
    try {
        sub_list = new LinkedList<T>();
    } catch (...) {
        throw MemoryAllocationException();
    }

    Node *cur = first;
    for (int i = 0; i < startIndex; i++)
        cur = cur->next;
    for (int i = startIndex; i <= endIndex; i++) {
        sub_list->Append(cur->data);
        cur = cur->next;
    }
    return sub_list;
}

template <typename T>
int LinkedList<T>::GetLength() const { 
    return length;
}

template <typename T>
void LinkedList<T>::Append(const T &item) {
    Node *newNode = createNode(item);
    if (first) 
        last->next = newNode;
    else
        first = newNode;
    last = newNode;
    length++;
}

template <typename T>
void LinkedList<T>::Prepend(const T &item) {
    Node *newNode = createNode(item);
    if (first) 
        newNode->next = first;
    else 
        last = newNode;
    first = newNode;
    length++;
}

template <typename T>
void LinkedList<T>::InsertAt(const T &item, int index) {
    if (index < 0 || index > length)
        throw IndexOutOfRangeException("Index out of range in LinkedList::InsertAt");
    if (index == 0) {
        Prepend(item);
        return;
    }
    if (index == length) {
        Append(item);
        return;
    }
    Node *current = first;
    for (int i = 0; i < index-1; i++)
        current = current->next;
    Node *newNode = createNode(item, current->next);
    current->next = newNode;
    length++;
}

template <typename T>
LinkedList<T>* LinkedList<T>::Concat(const LinkedList<T> &other) const {
    LinkedList<T> *result = nullptr;
    try {
        result = new LinkedList<T>();
    } catch (...) {
        throw MemoryAllocationException();
    }
    *result = *this;
    Node *cur = other.first;
    while (cur) {
        result->Append(cur->data);
        cur = cur->next;
    }
    return result;
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T> &other) {
    if (this != &other) {
        while (first) {
            Node* tmp = first;
            first = first->next;
            delete tmp;
        }
        first = last = nullptr;
        length = 0;
        Node *current = other.first; 
        while (current) {
            Append(current->data);            
            current = current->next;
        } 
    }
    return *this;
}

template <typename T>
LinkedList<T>::LinkedListEnumerator::LinkedListEnumerator(const LinkedList<T> lst) 
    : list(lst), current(nullptr), started(false) {}


template <typename T>
bool LinkedList<T>::LinkedListEnumerator::HasNext() {
    if (!started) {
        current = list.first;
        started = true;
    } else if (current) {
        current = current->next;
    }
    return current != nullptr;
}

template <typename T>
const T& LinkedList<T>::LinkedListEnumerator::GetCurrent() const {
    if (!current) {
        throw IndexOutOfRangeException("Enumerator not positioned");
    }
    return current->data;
}

template <typename T>
void LinkedList<T>::LinkedListEnumerator::Reset() {
    current = nullptr;
    started = false;
}

template <typename T>
IEnumerator<T>* LinkedList<T>::GetEnumerator() const {
    try {
        return new LinkedListEnumerator(*this);
    } catch (...) {
        throw MemoryAllocationException("Failed to get enumerator for linked list");
    }
}

#endif /* _LINKED_LIST_TPP_ */