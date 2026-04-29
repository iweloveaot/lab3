// SquareMatrix.h
#ifndef SQUARE_MATRIX_H
#define SQUARE_MATRIX_H

#include <stddef.h>   
#include "lab2/exceptions.h"

template <typename T, size_t N>
class SquareMatrix {
private:
    T data[N][N];   

public:
   
    SquareMatrix() {
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < N; ++j)
                data[i][j] = T();
    }

    explicit SquareMatrix(const T &diagValue) {
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < N; ++j)
                data[i][j] = (i == j) ? diagValue : T();
    }

    SquareMatrix(const T (&matrix)[N][N]) {
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < N; ++j)
                data[i][j] = matrix[i][j];
    }

    SquareMatrix(const T *flatData, size_t size) {
        if (size != N * N) throw IndexOutOfRangeException();
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < N; ++j)
                data[i][j] = flatData[i * N + j];
    }

    T& at(size_t i, size_t j) {
        if (i >= N || j >= N) throw IndexOutOfRangeException("Index out of range for square matrix");
        return data[i][j];
    }
    
    const T& at(size_t i, size_t j) const {
        if (i >= N || j >= N) throw IndexOutOfRangeException("Index out of range for square matrix");
        return data[i][j];
    }

    SquareMatrix operator+(const SquareMatrix &other) const {
        SquareMatrix result;
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < N; ++j)
                result.data[i][j] = data[i][j] + other.data[i][j];
        return result;
    }

    SquareMatrix operator-(const SquareMatrix &other) const {
        SquareMatrix result;
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < N; ++j)
                result.data[i][j] = data[i][j] - other.data[i][j];
        return result;
    }

    SquareMatrix operator*(const SquareMatrix &other) const {
        SquareMatrix result;
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < N; ++j) {
                T sum = T();
                for (size_t k = 0; k < N; ++k)
                    sum = sum + data[i][k] * other.data[k][j];
                result.data[i][j] = sum;
            }
        }
        return result;
    }

    SquareMatrix operator*(const T &scalar) const {
        SquareMatrix result;
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < N; ++j)
                result.data[i][j] = data[i][j] * scalar;
        return result;
    }

    bool operator==(const SquareMatrix &other) const {
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < N; ++j)
                if (data[i][j] != other.data[i][j])
                    return false;
        return true;
    }

    bool operator!=(const SquareMatrix &other) const {
        return !(*this == other);
    }

    SquareMatrix& operator+=(const SquareMatrix &other) {
        *this = *this + other;
        return *this;
    }

    SquareMatrix& operator-=(const SquareMatrix &other) {
        *this = *this - other;
        return *this;
    }

    SquareMatrix& operator*=(const SquareMatrix &other) {
        *this = *this * other;
        return *this;
    }

    SquareMatrix& operator*=(const T &scalar) {
        *this = *this * scalar;
        return *this;
    }

};

#endif // SQUARE_MATRIX_H