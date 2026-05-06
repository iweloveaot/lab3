#ifndef _LINEAR_FORM_H_
#define _LINEAR_FORM_H_

#include "lab2/mutable_array_sequence.h"

template <typename T>
class LinearForm {
private:
    MutableArraySequence<T> coefs;

public:
    LinearForm();
    LinearForm(const T* data, int count);
    explicit LinearForm(int count);
    LinearForm(const DynamicArray<T> &arr);
    LinearForm(const MutableArraySequence<T> &seq);
    LinearForm(const LinearForm<T> &other);

    int CoefficientCount() const;
    const T& GetCoefficient(int num) const;

    LinearForm<T> Add(const LinearForm<T> &other);
    LinearForm<T> Subtract(const LinearForm<T> &other);
    LinearForm<T> MultiplyScalar(const T &scalar);

    void Evaluate(const T *x, T &result);

    LinearForm<T> AppendCoefficient(const T &coef);
    LinearForm<T> SetCoefficient(int num, const T &coef);

    LinearForm<T> Map(T (*func)(const T&));
    void Reduce(T (*func)(const T&, const T&), const T &init, T &result);
    LinearForm<T> Where(bool (*pred)(const T&));

    LinearForm<T> operator+(const LinearForm<T> &other);
    LinearForm<T> operator-(const LinearForm<T> &other);
    LinearForm<T> operator*(const T &scalar);
    const T& operator[](int index);
    bool operator==(const LinearForm<T> &other);
    bool operator!=(LinearForm<T> &other);

    LinearForm<T>& operator=(const LinearForm<T>& other);
    LinearForm<T>& operator+=(const LinearForm<T>& other);
    LinearForm<T>& operator-=(const LinearForm<T>& other);
    LinearForm<T>& operator*=(const T& scalar);
};

#include "linear_form.tpp"

#endif // _LINEAR_FORM_H_