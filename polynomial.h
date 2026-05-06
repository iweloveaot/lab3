#ifndef _POLYNOMIAL_H_
#define _POLYNOMIAL_H_

#include "lab2/mutable_array_sequence.h"

template <typename T>
class Polynomial {
private:
    MutableArraySequence<T> coefs;

    // Приватный вспомогательный метод
    void Normalize();

public:
    // Конструкторы
    Polynomial();
    Polynomial(const T* data, int count);
    explicit Polynomial(int count);
    Polynomial(const DynamicArray<T> &arr);
    Polynomial(const MutableArraySequence<T> &seq);
    Polynomial(const Polynomial<T> &other);

    // Методы доступа
    int Degree() const;
    const T& GetCoefficient(int degree) const;

    // Арифметические операции (возвращают новый объект)
    Polynomial<T> Add(const Polynomial<T> &other);
    Polynomial<T> Multiply(const Polynomial<T> &other);
    Polynomial<T> MultiplyScalar(const T &scalar);
    Polynomial<T> Composition(const Polynomial<T> &other);

    // Вычисление значения полинома
    void Evaluate(const T &x, T &result);

    // Методы модификации (возвращают новый объект)
    Polynomial<T> AppendCoefficient(const T& coef);
    Polynomial<T> SetCoefficient(int degree, const T& coef);

    // Функциональные методы
    Polynomial<T> Map(T (*func)(const T&));
    void Reduce(T (*func)(const T&, const T&), const T &init, T &result);
    Polynomial<T> Where(bool (*pred)(const T&));

    // Операторы
    Polynomial<T> operator+(const Polynomial<T> &other);
    Polynomial<T> operator-(const Polynomial<T> &other);
    Polynomial<T> operator*(const Polynomial<T> &other);
    Polynomial<T> operator*(const T &scalar);
    const T& operator[](int index);
    bool operator==(const Polynomial<T> &other);
    bool operator!=(Polynomial<T> &other);

    // Операторы присваивания и составные
    Polynomial<T>& operator=(const Polynomial<T>& other);
    Polynomial<T>& operator+=(const Polynomial<T>& other);
    Polynomial<T>& operator-=(const Polynomial<T>& other);
    Polynomial<T>& operator*=(const Polynomial<T>& other);
    Polynomial<T>& operator*=(const T& scalar);
};

// Подключаем реализацию шаблонов (в конце файла, внутри гардов!)
#include "polynomial.tpp"

#endif // _POLYNOMIAL_H_