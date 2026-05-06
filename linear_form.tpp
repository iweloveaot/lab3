#ifndef _LINEAR_FORM_TPP_
#define _LINEAR_FORM_TPP_

template <typename T>
LinearForm<T>::LinearForm() : coefs(MutableArraySequence<T>()) {}

template <typename T>
LinearForm<T>::LinearForm(const T* data, int count) 
    : coefs(MutableArraySequence<T>(data, count)) {}

template <typename T>
LinearForm<T>::LinearForm(int count) 
    : coefs(MutableArraySequence<T>(count)) {}

template <typename T>
LinearForm<T>::LinearForm(const DynamicArray<T> &arr) 
    : coefs(MutableArraySequence<T>(arr)) {}

template <typename T>
LinearForm<T>::LinearForm(const MutableArraySequence<T> &seq) 
    : coefs(MutableArraySequence<T>(seq)) {}

template <typename T>
LinearForm<T>::LinearForm(const LinearForm<T> &other) 
    : coefs(MutableArraySequence<T>(other.coefs)) {}

template <typename T>
int LinearForm<T>::CoefficientCount() const {
    return coefs.GetLength(); 
}

template <typename T>
const T& LinearForm<T>::GetCoefficient(int num) const {
    if (num > CoefficientCount())
        throw IndexOutOfRangeException("Coefficient number out of range in LinearForm::GetCoefficient");
    else if (num < 0)
        throw IndexOutOfRangeException("Coefficient number must be non-negative in LinearForm::GetCoefficient");
    return coefs[num];
}

template <typename T>
LinearForm<T> LinearForm<T>::Add(const LinearForm<T> &other) {
    LinearForm<T> added = LinearForm<T>();
    int max_len = (coefs.GetLength() > other.coefs.GetLength()) ? coefs.GetLength() : other.coefs.GetLength();
    for (int i = 0; i < max_len; i++) {
        T sum = T();
        if (i < coefs.GetLength())
            sum = sum + coefs[i];
        if (i < other.coefs.GetLength())
            sum = sum + other.coefs.Get(i);
        added.coefs.Append(sum);
    }
    return added;
}

template <typename T>
LinearForm<T> LinearForm<T>::Subtract(const LinearForm<T> &other) {
    LinearForm<T> subtracted = LinearForm<T>();
    int max_len = (coefs.GetLength() > other.coefs.GetLength()) ? coefs.GetLength() : other.coefs.GetLength();
    for (int i = 0; i < max_len; i++) {
        T diff = T();
        if (i < coefs.GetLength())
            diff = diff + coefs[i];
        if (i < other.coefs.GetLength())
            diff = diff - other.coefs.Get(i);
        subtracted.coefs.Append(diff);
    }
    return subtracted;
}

template <typename T>
LinearForm<T> LinearForm<T>::MultiplyScalar(const T &scalar) {
    if (scalar == T()) 
        return LinearForm<T>();
    DynamicArray<T> new_coefs = DynamicArray<T>(coefs.GetLength());
    for (int i = 0; i < coefs.GetLength(); i++) 
        new_coefs.Set(i, coefs[i] * scalar);
    LinearForm<T> multiplied = LinearForm<T>(new_coefs);
    return multiplied;
}

template <typename T>
void LinearForm<T>::Evaluate(const T *x, T &result) {
    result = coefs[0];
    for (int i = 0; i < coefs.GetLength()-1; i++)
        result = result + coefs[i+1] * x[i];
}

template <typename T>
LinearForm<T> LinearForm<T>::AppendCoefficient(const T &coef) {
    LinearForm<T> result = *this;
    result.coefs.Append(coef);
    return result;
}

template <typename T>
LinearForm<T> LinearForm<T>::SetCoefficient(int num, const T &coef) {
    if (num < 0)
        throw IndexOutOfRangeException("Coefficient number must be non-negative in LinearForm::SetCoefficient");

    int new_num = CoefficientCount();
    if (num > new_num) 
        new_num = num + 1;

    DynamicArray<T> new_coefs = DynamicArray<T>(coefs.GetLength());
    for (int i = 0; i < CoefficientCount(); i++) {
        new_coefs.Set(i, GetCoefficient(i));
    }
    new_coefs.Resize(new_num);
    new_coefs.Set(num, coef);
    LinearForm<T> result = LinearForm<T>(new_coefs);
    return result;
}

template <typename T>
LinearForm<T> LinearForm<T>::Map(T (*func)(const T&)) {
    DynamicArray<T> new_coefs = DynamicArray<T>(coefs.GetLength());
    for (int i = 0; i < CoefficientCount(); i++) {
        new_coefs.Set(i, func(GetCoefficient(i)));
    }
    LinearForm<T> result = LinearForm<T>(new_coefs);
    return result;
}

template <typename T>
void LinearForm<T>::Reduce(T (*func)(const T&, const T&), const T &init, T &result) {
    T reduced = init;
    for (int i = 0; i < CoefficientCount(); i++) 
        reduced = func(reduced, GetCoefficient(i));
    result = reduced;
}

template <typename T>
LinearForm<T> LinearForm<T>::Where(bool (*pred)(const T&)) {
    DynamicArray<T> new_coefs = DynamicArray<T>(coefs.GetLength());
    for (int i = 0; i < CoefficientCount(); i++) {
        if (pred(GetCoefficient(i)))
            new_coefs.Set(i, GetCoefficient(i));
        else
            new_coefs.Set(i, T());
    }
    LinearForm<T> result = LinearForm<T>(new_coefs);
    return result;
}

template <typename T>
LinearForm<T> LinearForm<T>::operator+(const LinearForm<T> &other) {
    return this->Add(other);
}

template <typename T>
LinearForm<T> LinearForm<T>::operator-(const LinearForm<T> &other) {
    return this->Subtract(other);
}

template <typename T>
LinearForm<T> LinearForm<T>::operator*(const T &scalar) {
    return this->MultiplyScalar(scalar);
}

template <typename T>
const T& LinearForm<T>::operator[](int index) {
    return GetCoefficient(index);
}

template <typename T>
bool LinearForm<T>::operator==(const LinearForm<T> &other) {
    if (CoefficientCount() != other.CoefficientCount()) return false;
    for (int i = 0; i < CoefficientCount(); i++) {
        if (!(GetCoefficient(i) == other.GetCoefficient(i))) return false;
    }
    return true;
}

template <typename T>
bool LinearForm<T>::operator!=(LinearForm<T> &other) {
    return !(*this == other);
}

template <typename T>
LinearForm<T>& LinearForm<T>::operator=(const LinearForm<T>& other) {
    if (this != &other) {
        this->coefs = other.coefs;
    }
    return *this;
}

template <typename T>
LinearForm<T>& LinearForm<T>::operator+=(const LinearForm<T>& other) {
    *this = Add(other);
    return *this;
}

template <typename T>
LinearForm<T>& LinearForm<T>::operator-=(const LinearForm<T>& other) {
    *this = *this - other;
    return *this;
}

template <typename T>
LinearForm<T>& LinearForm<T>::operator*=(const T& scalar) {
    *this = MultiplyScalar(scalar);
    return *this;
}




#endif /*_LINEAR_FORM_TPP_*/
