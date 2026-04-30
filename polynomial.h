#ifndef _POLYNOMIAL_
#define _POLYNOMIAL_

#include "lab2/mutable_array_sequence.h"

template <typename T>
class Polynomial {
private:
    MutableArraySequence<T> coefs;

    void Normalize() {
        int degree = Degree();
        while (degree >= 0 && GetCoefficient(degree) == T()) {
            degree--;
        }
        if (degree < 0) degree = 0;

        if (degree < Degree()) {
            DynamicArray<T> normalized(degree + 1);
            for (int i = 0; i <= degree; i++) {
                normalized.Set(i, GetCoefficient(i));
            }
            *this = Polynomial<T>(normalized);
        }
    }



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

    const T& GetCoefficient(int degree) const {
        if (degree > Degree())
            throw IndexOutOfRangeException("Degree out of range in Polynomial::GetCoefficient");
        else if (degree < 0)
            throw IndexOutOfRangeException("Degree must be non-negative in Polynomial::GetCoefficient");
        return coefs[degree];
    } 

    Polynomial<T> Add(const Polynomial<T> &other) {
        Polynomial<T> added = Polynomial<T>();
        int max_len = (coefs.GetLength() > other.coefs.GetLength()) ? coefs.GetLength() : other.coefs.GetLength();
        for (int i = 0; i < max_len; i++) {
            T sum = T();
            if (i < coefs.GetLength())
                sum = sum + coefs[i];
            if (i < other.coefs.GetLength())
                sum = sum + other.coefs.Get(i);
            added.coefs.Append(sum);
        }
        added.Normalize();
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
        multiplied.Normalize();
        return multiplied;
    }

    Polynomial<T> MultiplyScalar(const T &scalar) {
        if (scalar == T()) 
            return Polynomial<T>();
        DynamicArray<T> new_coefs = DynamicArray<T>(coefs.GetLength());
        for (int i = 0; i < coefs.GetLength(); i++) 
            new_coefs.Set(i, coefs[i] * scalar);
        Polynomial<T> multiplied = Polynomial<T>(new_coefs);
        multiplied.Normalize();
        return multiplied;
    }

    void Evaluate(const T &x, T &result) {
        result = T();
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
        result.Normalize();
        return result;    
    }

    Polynomial<T> AppendCoefficient(const T& coef) {
        Polynomial<T> result = *this;
        result.coefs.Append(coef);
        return result;
    }

    Polynomial<T> SetCoefficient(int degree, const T& coef) {
        if (degree < 0)
            throw IndexOutOfRangeException("Degree must be non-negative in Polynomial::SetCoefficient");

        int new_deg = Degree();
        if (degree > new_deg) 
            new_deg = degree;

        DynamicArray<T> new_coefs = DynamicArray<T>(coefs.GetLength());
        for (int i = 0; i <= Degree(); i++) {
            new_coefs.Set(i, GetCoefficient(i));
        }
        new_coefs.Resize(new_deg + 1);
        new_coefs.Set(degree, coef);
        Polynomial<T> result = Polynomial<T>(new_coefs);
        result.Normalize();
        return result;
    }

    Polynomial<T> Map(T (*func)(const T&)) {
        DynamicArray<T> new_coefs = DynamicArray<T>(coefs.GetLength());
        for (int i = 0; i <= Degree(); i++) {
            new_coefs.Set(i, func(GetCoefficient(i)));
        }
        Polynomial<T> result = Polynomial<T>(new_coefs);
        result.Normalize();
        return result;
    }

    void Reduce(T (*func)(const T&, const T&), const T &init, T &result) {
        T reduced = init;
        for (int i = 0; i <= Degree(); i++) 
            reduced = func(reduced, GetCoefficient(i));
        result = reduced;
    }

    Polynomial<T> Where(bool (*pred)(const T&)) {
        DynamicArray<T> new_coefs = DynamicArray<T>(coefs.GetLength());
        for (int i = 0; i <= Degree(); i++) {
            if (pred(GetCoefficient(i)))
                new_coefs.Set(i, GetCoefficient(i));
            else
                new_coefs.Set(i, T());
        }
        Polynomial<T> result = Polynomial<T>(new_coefs);
        return result;
    }


    Polynomial<T> operator+(const Polynomial<T> &other) {
        return this->Add(other);
    }

    Polynomial<T> operator-(const Polynomial<T> &other) {
        Polynomial<T> subtracted = Polynomial<T>();
        int max_len = (coefs.GetLength() > other.coefs.GetLength()) ? coefs.GetLength() : other.coefs.GetLength();
        for (int i = 0; i < max_len; i++) {
            T diff = T();
            if (i < coefs.GetLength())
                diff = diff + coefs[i];
            if (i < other.coefs.GetLength())
                diff = diff - other.coefs.Get(i);
            subtracted.coefs.Append(diff);
        }
        subtracted.Normalize();
        return subtracted;
    }

    Polynomial<T> operator*(const Polynomial<T> &other) {
        return this->Multiply(other);
    }

    Polynomial<T> operator*(const T &scalar) {
        return this->MultiplyScalar(scalar);
    }

    const T& operator[](int index) {
        return GetCoefficient(index);
    }

    bool operator==(const Polynomial<T> &other) {
        if (Degree() != other.Degree()) return false;
        for (int i = 0; i <= Degree(); i++) {
            if (!(GetCoefficient(i) == other.GetCoefficient(i))) return false;
        }
        return true;
    }

    bool operator!=(Polynomial<T> &other) {
        return !(*this == other);
    }

    Polynomial<T>& operator=(const Polynomial<T>& other) {
        if (this != &other) {
            this->coefs = other.coefs;
        }
        return *this;
    }

    Polynomial<T>& operator+=(const Polynomial<T>& other) {
        *this = Add(other);
        return *this;
    }

    Polynomial<T>& operator-=(const Polynomial<T>& other) {
        *this = *this - other;
        return *this;
    }

    Polynomial<T>& operator*=(const Polynomial<T>& other) {
        *this = Multiply(other);
        return *this;
    }

    Polynomial<T>& operator*=(const T& scalar) {
        *this = MultiplyScalar(scalar);
        return *this;
    }
}; 


#endif /*_POLYNOMIAL_*/