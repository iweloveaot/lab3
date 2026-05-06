#ifndef _SQUARE_MATRIX_TPP_
#define _SQUARE_MATRIX_TPP_



template <typename T, size_t N>
SquareMatrix<T, N>::SquareMatrix() {
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            data[i][j] = T();
}

template <typename T, size_t N>
SquareMatrix<T, N>::SquareMatrix(const T &diagValue) {
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            data[i][j] = (i == j) ? diagValue : T();
}

template <typename T, size_t N>
SquareMatrix<T, N>::SquareMatrix(const T (&matrix)[N][N]) {
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            data[i][j] = matrix[i][j];
}

template <typename T, size_t N>
SquareMatrix<T, N>::SquareMatrix(const T *flatData, size_t size) {
    if (size != N * N) throw IndexOutOfRangeException();
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            data[i][j] = flatData[i * N + j];
}

template <typename T, size_t N>
T& SquareMatrix<T, N>::at(size_t i, size_t j) {
    if (i >= N || j >= N) throw IndexOutOfRangeException("Index out of range for square matrix");
    return data[i][j];
}

template <typename T, size_t N>
const T& SquareMatrix<T, N>::at(size_t i, size_t j) const {
    if (i >= N || j >= N) throw IndexOutOfRangeException("Index out of range for square matrix");
    return data[i][j];
}

template <typename T, size_t N>
SquareMatrix<T, N> SquareMatrix<T, N>::operator+(const SquareMatrix &other) const {
    SquareMatrix result;
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            result.data[i][j] = data[i][j] + other.data[i][j];
    return result;
}

template <typename T, size_t N>
SquareMatrix<T, N> SquareMatrix<T, N>::operator-(const SquareMatrix &other) const {
    SquareMatrix result;
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            result.data[i][j] = data[i][j] - other.data[i][j];
    return result;
}

template <typename T, size_t N>
SquareMatrix<T, N> SquareMatrix<T, N>::operator*(const SquareMatrix &other) const {
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

template <typename T, size_t N>
SquareMatrix<T, N> SquareMatrix<T, N>::operator*(const T &scalar) const {
    SquareMatrix result;
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            result.data[i][j] = data[i][j] * scalar;
    return result;
}

template <typename T, size_t N>
bool SquareMatrix<T, N>::operator==(const SquareMatrix &other) const {
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            if (data[i][j] != other.data[i][j])
                return false;
    return true;
}

template <typename T, size_t N>
bool SquareMatrix<T, N>::operator!=(const SquareMatrix &other) const {
    return !(*this == other);
}

template <typename T, size_t N>
SquareMatrix<T, N>& SquareMatrix<T, N>::operator+=(const SquareMatrix &other) {
    *this = *this + other;
    return *this;
}

template <typename T, size_t N>
SquareMatrix<T, N>& SquareMatrix<T, N>::operator-=(const SquareMatrix &other) {
    *this = *this - other;
    return *this;
}

template <typename T, size_t N>
SquareMatrix<T, N>& SquareMatrix<T, N>::operator*=(const SquareMatrix &other) {
    *this = *this * other;
    return *this;
}

template <typename T, size_t N>
SquareMatrix<T, N>& SquareMatrix<T, N>::operator*=(const T &scalar) {
    *this = *this * scalar;
    return *this;
}

#endif