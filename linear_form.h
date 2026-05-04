#ifndef _LINEAR_FORM_H_
#define _LINEAR_FORM_H_

#include "lab2/mutable_array_sequence.h"

template <typename T>
class LinearForm {
private:
    MutableArraySequence<T> coefs;

public:
    LinearForm() :  coefs(MutableArraySequence<T>()) {}
    LinearForm(const T* data, int count) : coefs(MutableArraySequence<T>(data, count)) {}
    explicit LinearForm(int count) : coefs(MutableArraySequence<T>(count)) {}
    LinearForm(const DynamicArray<T> &arr) : coefs(MutableArraySequence<T>(arr)) {}
    LinearForm(const MutableArraySequence<T> &seq) : coefs(MutableArraySequence<T>(seq)) {}
    LinearForm(const LinearForm<T> &other) : coefs(MutableArraySequence<T>(other.coefs)) {}


    int CoefficicentCount() const {
        return coefs.GetLength(); 
    }

    const T& GetCoefficient(int num) const {
        if (num > CoefficicentCount())
            throw IndexOutOfRangeException("Coefficient number out of range in LinearForm::GetCoefficient");
        else if (num < 0)
            throw IndexOutOfRangeException("Coefficient number must be non-negative in LinearForm::GetCoefficient");
        return coefs[num];
    } 

    LinearForm<T> Add(const LinearForm<T> &other) {
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

    LinearForm<T> Subtract(const LinearForm<T> &other) {
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

    LinearForm<T> MultiplyScalar(const T &scalar) {
        if (scalar == T()) 
            return LinearForm<T>();
        DynamicArray<T> new_coefs = DynamicArray<T>(coefs.GetLength());
        for (int i = 0; i < coefs.GetLength(); i++) 
            new_coefs.Set(i, coefs[i] * scalar);
        LinearForm<T> multiplied = LinearForm<T>(new_coefs);
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

    LinearForm<T> AppendCoefficient(const T &coef) {
        LinearForm<T> result = *this;
        result.coefs.Append(coef);
        return result;
    }

    LinearForm<T> SetCoefficient(int num, const T &coef) {
        if (num < 0)
            throw IndexOutOfRangeException("Coefficient number must be non-negative in LinearForm::SetCoefficient");

        int new_num = CoefficicentCount();
        if (num > new_num) 
            new_num = num + 1;

        DynamicArray<T> new_coefs = DynamicArray<T>(coefs.GetLength());
        for (int i = 0; i < CoefficicentCount(); i++) {
            new_coefs.Set(i, GetCoefficient(i));
        }
        new_coefs.Resize(new_num);
        new_coefs.Set(num, coef);
        LinearForm<T> result = LinearForm<T>(new_coefs);
        return result;
    }

    LinearForm<T> Map(T (*func)(const T&)) {
        DynamicArray<T> new_coefs = DynamicArray<T>(coefs.GetLength());
        for (int i = 0; i < CoefficicentCount(); i++) {
            new_coefs.Set(i, func(GetCoefficient(i)));
        }
        LinearForm<T> result = LinearForm<T>(new_coefs);
        return result;
    }

    void Reduce(T (*func)(const T&, const T&), const T &init, T &result) {
        T reduced = init;
        for (int i = 0; i < CoefficicentCount(); i++) 
            reduced = func(reduced, GetCoefficient(i));
        result = reduced;
    }

    LinearForm<T> Where(bool (*pred)(const T&)) {
        DynamicArray<T> new_coefs = DynamicArray<T>(coefs.GetLength());
        for (int i = 0; i < CoefficicentCount(); i++) {
            if (pred(GetCoefficient(i)))
                new_coefs.Set(i, GetCoefficient(i));
            else
                new_coefs.Set(i, T());
        }
        LinearForm<T> result = LinearForm<T>(new_coefs);
        return result;
    }


    LinearForm<T> operator+(const LinearForm<T> &other) {
        return this->Add(other);
    }

    LinearForm<T> operator-(const LinearForm<T> &other) {
        return this->Subtract(other);
    }

    LinearForm<T> operator*(const T &scalar) {
        return this->MultiplyScalar(scalar);
    }

    const T& operator[](int index) {
        return GetCoefficient(index);
    }

    bool operator==(const LinearForm<T> &other) {
        if (CoefficicentCount() != other.CoefficicentCount()) return false;
        for (int i = 0; i < CoefficicentCount(); i++) {
            if (!(GetCoefficient(i) == other.GetCoefficient(i))) return false;
        }
        return true;
    }

    bool operator!=(LinearForm<T> &other) {
        return !(*this == other);
    }

    LinearForm<T>& operator=(const LinearForm<T>& other) {
        if (this != &other) {
            this->coefs = other.coefs;
        }
        return *this;
    }

    LinearForm<T>& operator+=(const LinearForm<T>& other) {
        *this = Add(other);
        return *this;
    }

    LinearForm<T>& operator-=(const LinearForm<T>& other) {
        *this = *this - other;
        return *this;
    }

    LinearForm<T>& operator*=(const T& scalar) {
        *this = MultiplyScalar(scalar);
        return *this;
    }
}; 


#endif /*_LINEAR_FORM_H_*/
