// square_matrix.h
#ifndef SQUARE_MATRIX_H
#define SQUARE_MATRIX_H

#include <stddef.h>
#include <iostream>
#include "lab2/exceptions.h"

template <typename T, size_t N>
class SquareMatrix {
private:
    T data[N][N];

public:
    SquareMatrix();
    explicit SquareMatrix(const T &diagValue);
    SquareMatrix(const T (&matrix)[N][N]);
    SquareMatrix(const T *flatData, size_t size);

    T& at(size_t i, size_t j);
    const T& at(size_t i, size_t j) const;

    SquareMatrix operator+(const SquareMatrix &other) const;
    SquareMatrix operator-(const SquareMatrix &other) const;
    SquareMatrix operator*(const SquareMatrix &other) const;
    SquareMatrix operator*(const T &scalar) const;

    bool operator==(const SquareMatrix &other) const;
    bool operator!=(const SquareMatrix &other) const;

    SquareMatrix& operator+=(const SquareMatrix &other);
    SquareMatrix& operator-=(const SquareMatrix &other);
    SquareMatrix& operator*=(const SquareMatrix &other);
    SquareMatrix& operator*=(const T &scalar);

    friend std::ostream& operator<<(std::ostream& os, const SquareMatrix& m) {
        os << "[";
        for (size_t i = 0; i < N; i++) {
            os << "[";
            for (size_t j = 0; j < N; ++j) {
                os << m.data[i][j];
                if (j + 1 < N) os << ", ";
            }
            os << "]";
            if (i + 1 < N) os << "\n ";
        }
        os << "]";
        return os;
    }

    friend std::istream& operator>>(std::istream& is, SquareMatrix& m) {
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < N; ++j) {
                is >> m.data[i][j];
            }
        }
        return is;
    }
};

#include "square_matrix.tpp"

#endif // SQUARE_MATRIX_H