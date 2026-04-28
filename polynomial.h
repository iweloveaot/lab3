#ifndef _POLYNOMIAL_
#define _POLYNOMIAL_

#include "lab2/mutable_array_sequence.h"

template <typename T>
class Polynomial {
private:
    MutableArraySequence<T> coefs;

public:
    Polynomial() :  coefs(MutableArraySequence<T>()) {}
    Polynomial(const T* data, int count) : coefs(MutableArraySequence<T>(data, count)) {}
    explicit Polynomial(int count) : coefs(MutableArraySequence<T>(count)) {}
    Polynomial(const DynamicArray<T> &arr) : coefs(MutableArraySequence<T>(arr)) {}
    Polynomial(const MutableArraySequence<T> &seq) : coefs(MutableArraySequence<T>(seq)) {}
    Polynomial(const Polynomial<T> &other) : coefs(MutableArraySequence<T>(other.coefs)) {}


    int Degree() const {
        return coefs.GetLength() - 1; 
    }

    const T& GetCoefficient(int degree) {
        return coefs[degree];
    } 

    Polynomial<T> Add(const Polynomial<T> &other) {
        Polynomial<T> added = Polynomial<T>();
        int max_len = (coefs.GetLength() > other.coefs.GetLength()) ? coefs.GetLength() : other.coefs.GetLength();
        for (int i = 0; i < max_len; i++) {
            T sum = T(0);
            if (i < coefs.GetLength())
                sum = sum + coefs[i];
            if (i < other.coefs.GetLength())
                sum = sum + other.coefs.Get(i);
            added.coefs.Append(sum);
        }
        return added;
    }

    Polynomial<T> Multiply(const Polynomial<T> &other) {
        DynamicArray<T> new_coefs = DynamicArray<T>(Degree() + other.Degree() + 1);
        for (int i = 0; i < coefs.GetLength(); i++) {
            for (int j = 0; j < other.coefs.GetLength(); j++) {
                new_coefs.Set((i + j), new_coefs.Get(i + j) + coefs[i] * other.coefs.Get(j));
            }
        }
        Polynomial<T> multiplied = Polynomial<T>(new_coefs);
        return multiplied;
    }

    Polynomial<T> MultiplyScalar(const T& scalar) {
        DynamicArray<T> new_coefs = DynamicArray<T>(coefs.GetLength());
        for (int i = 0; i < coefs.GetLength(); i++) 
            new_coefs.Set(i, coefs[i] * scalar);
        Polynomial<T> multiplied = Polynomial<T>(new_coefs);
        return multiplied;
    }

    void Evalute(const T& x, T& result) {
        result = T(0);
        T power = T(1);
        for (int i = 0; i < coefs.GetLength(); i++) {
            result = result + coefs[i] * power;
            power = power * x; 
        }
    }

    Polynomial<T> Composition(const Polynomial<T> &other) {
        Polynomial<T> result = Polynomial<T>(Degree() * other.Degree());
        DynamicArray<T> first = DynamicArray<T>(1);
        first.Set(0, T(1));
        Polynomial<T> power = Polynomial<T>(first);
        
        for (int i = 0; i < coefs.GetLength(); i++) {
            Polynomial<T> term = power.MultiplyScalar(coefs[i]);
            result = result.Add(term);
            power = power.Multiply(other);
        }
        return result;    
    }
}; 




#endif /*_POLYNOMIAL_*/