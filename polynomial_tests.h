#ifndef TESTS_POLYNOMIAL_H
#define TESTS_POLYNOMIAL_H

#include <iostream>
#include <cassert>
#include <sstream>
#include <cmath>
#include "complex.h"
#include "square_matrix.h"
#include "polynomial.h"
#include "lab2/exceptions.h"

// Счётчик тестов (общий с tests.h)
static int tests_passed;
static int tests_failed;

#define TEST(name) void name()
#define RUN_TEST(name) do { \
    try { \
        std::cout << "Running: " << #name << "... "; \
        name(); \
        std::cout << "✓ PASSED\n"; \
        tests_passed++; \
    } catch (const std::exception& e) { \
        std::cout << "✗ FAILED: " << e.what() << "\n"; \
        tests_failed++; \
    } catch (...) { \
        std::cout << "✗ FAILED: Unknown exception\n"; \
        tests_failed++; \
    } \
} while(0)

#define ASSERT_EQ(a, b) do { \
    if ((a) != (b)) { \
        throw std::runtime_error("Assertion failed: " + std::to_string(a) + " != " + std::to_string(b)); \
    } \
} while(0)

#define ASSERT_TRUE(cond) do { \
    if (!(cond)) { \
        throw std::runtime_error("Assertion failed: condition is false"); \
    } \
} while(0)

#define ASSERT_FALSE(cond) do { \
    if (cond) { \
        throw std::runtime_error("Assertion failed: expected false, but got true"); \
    } \
} while(0)

#define ASSERT_THROW(expr, ExceptionType) do { \
    bool caught = false; \
    try { expr; } \
    catch (const ExceptionType&) { caught = true; } \
    if (!caught) throw std::runtime_error("Expected exception " #ExceptionType " not thrown"); \
} while(0)

// Вспомогательные функции для сравнения
inline bool complexEqual(const Complex& a, const Complex& b, double epsilon = 1e-9) {
    return std::abs(a.real() - b.real()) < epsilon && 
           std::abs(a.imag() - b.imag()) < epsilon;
}

inline bool doubleEqual(double a, double b, double epsilon = 1e-9) {
    return std::abs(a - b) < epsilon;
}

// ==================== Complex Tests ====================

TEST(test_complex_constructors) {
    Complex c1;
    ASSERT_EQ(c1.real(), 0.0);
    ASSERT_EQ(c1.imag(), 0.0);
    
    Complex c2(3.5, -2.1);
    ASSERT_EQ(c2.real(), 3.5);
    ASSERT_EQ(c2.imag(), -2.1);
    
    Complex c3(c2);
    ASSERT_TRUE(complexEqual(c3, c2));
}

TEST(test_complex_getters) {
    Complex c(7.2, -4.8);
    ASSERT_EQ(c.real(), 7.2);
    ASSERT_EQ(c.imag(), -4.8);
}

TEST(test_complex_addition) {
    Complex a(2.0, 3.0);
    Complex b(4.0, -1.0);
    Complex result = a + b;
    
    ASSERT_TRUE(complexEqual(result, Complex(6.0, 2.0)));
    ASSERT_TRUE(complexEqual(a + b, b + a));
    
    Complex zero(0.0, 0.0);
    ASSERT_TRUE(complexEqual(a + zero, a));
}

TEST(test_complex_subtraction) {
    Complex a(5.0, 7.0);
    Complex b(2.0, 3.0);
    Complex result = a - b;
    
    ASSERT_TRUE(complexEqual(result, Complex(3.0, 4.0)));
    ASSERT_TRUE(complexEqual(a - a, Complex(0.0, 0.0)));
    
    Complex neg = -a;
    ASSERT_TRUE(complexEqual(neg, Complex(-5.0, -7.0)));
}

TEST(test_complex_multiplication) {
    Complex a(2.0, 3.0);
    Complex b(4.0, 5.0);
    Complex result = a * b;
    
    ASSERT_TRUE(complexEqual(result, Complex(2*4 - 3*5, 2*5 + 3*4)));
    
    Complex scaled = a * 3.0;
    ASSERT_TRUE(complexEqual(scaled, Complex(6.0, 9.0)));
    
    Complex one(1.0, 0.0);
    ASSERT_TRUE(complexEqual(a * one, a));
    
    Complex zero(0.0, 0.0);
    ASSERT_TRUE(complexEqual(a * zero, zero));
}

TEST(test_complex_division) {
    Complex a(10.0, 5.0);
    Complex b(2.0, 1.0);
    Complex result = a / b;
    
    ASSERT_TRUE(complexEqual(result, Complex(5.0, 0.0)));
    
    Complex c(3.0, 4.0);
    ASSERT_TRUE(complexEqual(c / c, Complex(1.0, 0.0)));
}

TEST(test_complex_comparison) {
    Complex a(1.0, 2.0);
    Complex b(1.0, 2.0);
    Complex c(2.0, 1.0);
    
    ASSERT_TRUE(a == b);
    ASSERT_FALSE(a == c);
    ASSERT_TRUE(a != c);
    ASSERT_FALSE(a != b);
}

TEST(test_complex_assignment) {
    Complex a(1.0, 2.0);
    Complex b;
    b = a;
    
    ASSERT_TRUE(complexEqual(b, a));
    
    a = a;
    ASSERT_TRUE(complexEqual(a, Complex(1.0, 2.0)));
}

TEST(test_complex_stream_operators) {
    Complex c(3.5, -2.1);
    std::ostringstream oss;
    oss << c;
    ASSERT_TRUE(oss.str().find("3.5") != std::string::npos);
    
    std::istringstream iss("4.2-1.5i");
    Complex parsed;
    iss >> parsed;
    ASSERT_TRUE(complexEqual(parsed, Complex(4.2, -1.5)));
}

// ==================== SquareMatrix Tests ====================

TEST(test_square_matrix_default_constructor) {
    SquareMatrix<double, 3> m;
    for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 3; ++j)
            ASSERT_EQ(m.at(i, j), 0.0);
}

TEST(test_square_matrix_diagonal_constructor) {
    SquareMatrix<double, 3> m(5.0);
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            if (i == j)
                ASSERT_EQ(m.at(i, j), 5.0);
            else
                ASSERT_EQ(m.at(i, j), 0.0);
        }
    }
}

TEST(test_square_matrix_from_array) {
    double data[2][2] = {{1.0, 2.0}, {3.0, 4.0}};
    SquareMatrix<double, 2> m(data);
    
    ASSERT_EQ(m.at(0, 0), 1.0);
    ASSERT_EQ(m.at(0, 1), 2.0);
    ASSERT_EQ(m.at(1, 0), 3.0);
    ASSERT_EQ(m.at(1, 1), 4.0);
}

TEST(test_square_matrix_from_flat_array) {
    double flat[] = {1.0, 2.0, 3.0, 4.0};
    SquareMatrix<double, 2> m(flat, 4);
    
    ASSERT_EQ(m.at(0, 0), 1.0);
    ASSERT_EQ(m.at(0, 1), 2.0);
    ASSERT_EQ(m.at(1, 0), 3.0);
    ASSERT_EQ(m.at(1, 1), 4.0);
    
    ASSERT_THROW((SquareMatrix<double, 2>(flat, 3)), IndexOutOfRangeException);
}

TEST(test_square_matrix_at_method) {
    SquareMatrix<double, 3> m;
    m.at(1, 2) = 7.5;
    ASSERT_EQ(m.at(1, 2), 7.5);
    
    const SquareMatrix<double, 3>& cm = m;
    ASSERT_EQ(cm.at(1, 2), 7.5);
    
    ASSERT_THROW(m.at(3, 0), IndexOutOfRangeException);
    ASSERT_THROW(m.at(0, 5), IndexOutOfRangeException);
}

TEST(test_square_matrix_addition) {
    int data1[2][2] = {{1, 2}, {3, 4}};
    int data2[2][2] = {{5, 6}, {7, 8}};
    SquareMatrix<int, 2> a(data1), b(data2);
    
    SquareMatrix<int, 2> result = a + b;
    
    ASSERT_EQ(result.at(0, 0), 6);
    ASSERT_EQ(result.at(0, 1), 8);
    ASSERT_EQ(result.at(1, 0), 10);
    ASSERT_EQ(result.at(1, 1), 12);
    
    ASSERT_TRUE((a + b) == (b + a));
    
    SquareMatrix<int, 2> zero;
    ASSERT_TRUE((a + zero) == a);
}

TEST(test_square_matrix_subtraction) {
    int data1[2][2] = {{10, 20}, {30, 40}};
    int data2[2][2] = {{1, 2}, {3, 4}};
    SquareMatrix<int, 2> a(data1), b(data2);
    
    SquareMatrix<int, 2> result = a - b;
    
    ASSERT_EQ(result.at(0, 0), 9);
    ASSERT_EQ(result.at(0, 1), 18);
    ASSERT_EQ(result.at(1, 0), 27);
    ASSERT_EQ(result.at(1, 1), 36);
    
    ASSERT_TRUE((a - a) == (SquareMatrix<int, 2>()));
}

TEST(test_square_matrix_multiplication) {
    int data1[2][2] = {{1, 2}, {3, 4}};
    int data2[2][2] = {{5, 6}, {7, 8}};
    SquareMatrix<int, 2> a(data1), b(data2);
    
    SquareMatrix<int, 2> result = a * b;
    
    ASSERT_EQ(result.at(0, 0), 1*5 + 2*7);
    ASSERT_EQ(result.at(0, 1), 1*6 + 2*8);
    ASSERT_EQ(result.at(1, 0), 3*5 + 4*7);
    ASSERT_EQ(result.at(1, 1), 3*6 + 4*8);
    
    SquareMatrix<int, 2> identity(1);
    ASSERT_TRUE((a * identity) == a);
    ASSERT_TRUE((identity * a) == a);
}

TEST(test_square_matrix_scalar_multiplication) {
    int data[2][2] = {{1, 2}, {3, 4}};
    SquareMatrix<int, 2> m(data);
    
    SquareMatrix<int, 2> result = m * 3;
    
    ASSERT_EQ(result.at(0, 0), 3);
    ASSERT_EQ(result.at(0, 1), 6);
    ASSERT_EQ(result.at(1, 0), 9);
    ASSERT_EQ(result.at(1, 1), 12);
    
    ASSERT_TRUE((m * 0) == (SquareMatrix<int, 2>()));
    ASSERT_TRUE((m * 1) == m);
}

TEST(test_square_matrix_comparison) {
    int data1[2][2] = {{1, 2}, {3, 4}};
    int data2[2][2] = {{1, 2}, {3, 4}};
    int data3[2][2] = {{1, 2}, {3, 5}};
    
    SquareMatrix<int, 2> a(data1), b(data2), c(data3);
    
    ASSERT_TRUE(a == b);
    ASSERT_FALSE(a == c);
    ASSERT_TRUE(a != c);
    ASSERT_FALSE(a != b);
}

TEST(test_square_matrix_compound_assignment) {
    int data1[2][2] = {{1, 2}, {3, 4}};
    int data2[2][2] = {{5, 6}, {7, 8}};
    SquareMatrix<int, 2> a(data1), b(data2);
    
    SquareMatrix<int, 2> temp1 = a;
    temp1 += b;
    ASSERT_TRUE(temp1 == (a + b));
    
    SquareMatrix<int, 2> temp2 = a;
    temp2 -= b;
    ASSERT_TRUE(temp2 == (a - b));
    
    SquareMatrix<int, 2> temp3 = a;
    temp3 *= b;
    ASSERT_TRUE(temp3 == (a * b));
    
    SquareMatrix<int, 2> temp4 = a;
    temp4 *= 2;
    ASSERT_TRUE(temp4 == (a * 2));
}

TEST(test_square_matrix_with_complex) {
    Complex data[2][2] = {
        {Complex(1, 2), Complex(3, 4)},
        {Complex(5, 6), Complex(7, 8)}
    };
    
    SquareMatrix<Complex, 2> m(data);
    
    ASSERT_TRUE(m.at(0, 0) == Complex(1, 2));
    ASSERT_TRUE(m.at(1, 1) == Complex(7, 8));
    
    SquareMatrix<Complex, 2> result = m + m;
    ASSERT_TRUE(result.at(0, 0) == Complex(2, 4));
    
    SquareMatrix<Complex, 2> scaled = m * Complex(2, 0);
    ASSERT_TRUE(scaled.at(0, 0) == Complex(2, 4));
}

// ==================== Polynomial Tests ====================

TEST(test_polynomial_constructors) {
    Polynomial<double> p1;
    ASSERT_EQ(p1.Degree(), -1);
    
    double coeffs[] = {1.0, 2.0, 3.0};
    Polynomial<double> p2(coeffs, 3);
    ASSERT_EQ(p2.Degree(), 2);
    ASSERT_EQ(p2.GetCoefficient(0), 1.0);
    ASSERT_EQ(p2.GetCoefficient(2), 3.0);
    
    Polynomial<double> p3(5);
    ASSERT_EQ(p3.Degree(), 4);
    
    Polynomial<double> p4(p2);
    ASSERT_EQ(p4.Degree(), p2.Degree());
    ASSERT_EQ(p4.GetCoefficient(1), p2.GetCoefficient(1));
}

TEST(test_polynomial_degree_and_coefficients) {
    double coeffs[] = {5.0, 0.0, 0.0, 2.0};
    Polynomial<double> p(coeffs, 4);
    
    ASSERT_EQ(p.Degree(), 3);
    ASSERT_EQ(p.GetCoefficient(0), 5.0);
    ASSERT_EQ(p.GetCoefficient(3), 2.0);
    
    double coeffs2[] = {1.0, 2.0, 0.0, 0.0};
    Polynomial<double> p2(coeffs2, 4);
    ASSERT_EQ(p2.Degree(), 3);
    
    ASSERT_THROW(p.GetCoefficient(-1), IndexOutOfRangeException);
    ASSERT_THROW(p.GetCoefficient(10), IndexOutOfRangeException);
}

TEST(test_polynomial_addition) {
    double c1[] = {1.0, 2.0, 3.0};
    double c2[] = {4.0, 5.0};
    
    Polynomial<double> p1(c1, 3), p2(c2, 2);
    Polynomial<double> result = p1 + p2;
    
    ASSERT_EQ(result.Degree(), 2);
    ASSERT_EQ(result.GetCoefficient(0), 5.0);
    ASSERT_EQ(result.GetCoefficient(1), 7.0);
    ASSERT_EQ(result.GetCoefficient(2), 3.0);
    
    ASSERT_TRUE((p1 + p2) == (p2 + p1));
    
    Polynomial<double> zero;
    ASSERT_TRUE((p1 + zero) == p1);
}

TEST(test_polynomial_subtraction) {
    double c1[] = {10.0, 20.0, 30.0};
    double c2[] = {1.0, 2.0};
    
    Polynomial<double> p1(c1, 3), p2(c2, 2);
    Polynomial<double> result = p1 - p2;
    
    ASSERT_EQ(result.GetCoefficient(0), 9.0);
    ASSERT_EQ(result.GetCoefficient(1), 18.0);
    ASSERT_EQ(result.GetCoefficient(2), 30.0);
    
    ASSERT_EQ((p1 - p1).Degree(), 0);
    ASSERT_EQ((p1 - p1).GetCoefficient(0), 0.0);
}

TEST(test_polynomial_multiplication) {
    double c1[] = {1.0, 2.0};
    double c2[] = {3.0, 4.0};
    
    Polynomial<double> p1(c1, 2), p2(c2, 2);
    Polynomial<double> result = p1 * p2;
    
    ASSERT_EQ(result.Degree(), 2);
    ASSERT_EQ(result.GetCoefficient(0), 3.0);
    ASSERT_EQ(result.GetCoefficient(1), 10.0);
    ASSERT_EQ(result.GetCoefficient(2), 8.0);
    
    double c3[] = {5.0};
    Polynomial<double> scalar(c3, 1);
    Polynomial<double> scaled = p1 * scalar;
    
    ASSERT_EQ(scaled.GetCoefficient(0), 5.0);
    ASSERT_EQ(scaled.GetCoefficient(1), 10.0);
    
    Polynomial<double> zero;
    Polynomial<double> zeroResult = p1 * zero;
    ASSERT_EQ(zeroResult.Degree(), 0);
    ASSERT_EQ(zeroResult.GetCoefficient(0), 0.0);
}

TEST(test_polynomial_scalar_multiplication) {
    double coeffs[] = {1.0, 2.0, 3.0};
    Polynomial<double> p(coeffs, 3);
    
    Polynomial<double> result = p.MultiplyScalar(2.0);
    
    ASSERT_EQ(result.GetCoefficient(0), 2.0);
    ASSERT_EQ(result.GetCoefficient(1), 4.0);
    ASSERT_EQ(result.GetCoefficient(2), 6.0);
    
    Polynomial<double> zeroed = p.MultiplyScalar(0.0);
    ASSERT_EQ(zeroed.Degree(), -1);
    
    ASSERT_TRUE((p.MultiplyScalar(1.0)) == p);
}

TEST(test_polynomial_evaluation) {
    double coeffs[] = {1.0, 2.0, 3.0};
    Polynomial<double> p(coeffs, 3);
    
    double result;
    p.Evaluate(2.0, result);
    ASSERT_TRUE(doubleEqual(result, 17.0));
    
    p.Evaluate(0.0, result);
    ASSERT_TRUE(doubleEqual(result, 1.0));
    
    p.Evaluate(-1.0, result);
    ASSERT_TRUE(doubleEqual(result, 2.0));
}

TEST(test_polynomial_composition) {
    double c1[] = {1.0, 1.0};
    double c2[] = {2.0, 3.0};
    
    Polynomial<double> p(c1, 2), q(c2, 2);
    Polynomial<double> result = p.Composition(q);
    
    ASSERT_EQ(result.Degree(), 1);
    ASSERT_EQ(result.GetCoefficient(0), 3.0);
    ASSERT_EQ(result.GetCoefficient(1), 3.0);
    
    double c3[] = {1.0, 0.0, 1.0};
    double c4[] = {0.0, 2.0};
    
    Polynomial<double> p2(c3, 3), q2(c4, 2);
    Polynomial<double> result2 = p2.Composition(q2);
    
    ASSERT_EQ(result2.Degree(), 2);
    ASSERT_EQ(result2.GetCoefficient(0), 1.0);
    ASSERT_EQ(result2.GetCoefficient(1), 0.0);
    ASSERT_EQ(result2.GetCoefficient(2), 4.0);
}

TEST(test_polynomial_append_set_coefficient) {
    double coeffs[] = {1.0, 2.0};
    Polynomial<double> p(coeffs, 2);
    
    Polynomial<double> appended = p.AppendCoefficient(3.0);
    ASSERT_EQ(appended.Degree(), 2);
    ASSERT_EQ(appended.GetCoefficient(2), 3.0);
    ASSERT_EQ(p.Degree(), 1);
    
    Polynomial<double> modified = p.SetCoefficient(0, 5.0);
    ASSERT_EQ(modified.GetCoefficient(0), 5.0);
    ASSERT_EQ(modified.GetCoefficient(1), 2.0);
    
    Polynomial<double> extended = p.SetCoefficient(3, 7.0);
    ASSERT_EQ(extended.Degree(), 3);
    ASSERT_EQ(extended.GetCoefficient(3), 7.0);
    
    ASSERT_THROW(p.SetCoefficient(-1, 1.0), IndexOutOfRangeException);
}

TEST(test_polynomial_map_where_reduce) {
    double coeffs[] = {1.0, 2.0, 3.0, 4.0};
    Polynomial<double> p(coeffs, 4);
    
    auto square = [](const double& x) { return x * x; };
    Polynomial<double> mapped = p.Map(square);
    
    ASSERT_EQ(mapped.GetCoefficient(0), 1.0);
    ASSERT_EQ(mapped.GetCoefficient(1), 4.0);
    ASSERT_EQ(mapped.GetCoefficient(2), 9.0);
    ASSERT_EQ(mapped.GetCoefficient(3), 16.0);
    
    auto greaterThan2 = [](const double& x) { return x > 2.0; };
    Polynomial<double> filtered = p.Where(greaterThan2);
    
    ASSERT_EQ(filtered.GetCoefficient(0), 0.0);
    ASSERT_EQ(filtered.GetCoefficient(1), 0.0);
    ASSERT_EQ(filtered.GetCoefficient(2), 3.0);
    ASSERT_EQ(filtered.GetCoefficient(3), 4.0);
    
    auto add = [](const double& a, const double& b) { return a + b; };
    double sum;
    p.Reduce(add, 0.0, sum);
    ASSERT_TRUE(doubleEqual(sum, 10.0));
}

TEST(test_polynomial_operators) {
    double c1[] = {1.0, 2.0};
    double c2[] = {3.0, 4.0};
    
    Polynomial<double> p1(c1, 2), p2(c2, 2);
    
    ASSERT_EQ(p1[0], 1.0);
    ASSERT_EQ(p1[1], 2.0);
    
    Polynomial<double> p3(c1, 2);
    ASSERT_TRUE(p1 == p3);
    ASSERT_FALSE(p1 != p3);
    ASSERT_TRUE(p1 != p2);
    
    Polynomial<double> temp1 = p1;
    temp1 += p2;
    ASSERT_TRUE(temp1 == (p1 + p2));
    
    Polynomial<double> temp2 = p1;
    temp2 -= p2;
    ASSERT_TRUE(temp2 == (p1 - p2));
    
    Polynomial<double> temp3 = p1;
    temp3 *= p2;
    ASSERT_TRUE(temp3 == (p1 * p2));
    
    Polynomial<double> temp4 = p1;
    temp4 *= 3.0;
    ASSERT_TRUE(temp4 == p1.MultiplyScalar(3.0));
}

TEST(test_polynomial_with_complex) {
    Complex coeffs[] = {Complex(1, 0), Complex(0, 1), Complex(2, 0)};
    Polynomial<Complex> p(coeffs, 3);
    
    ASSERT_EQ(p.Degree(), 2);
    ASSERT_TRUE(complexEqual(p.GetCoefficient(0), Complex(1, 0)));
    ASSERT_TRUE(complexEqual(p.GetCoefficient(1), Complex(0, 1)));
    
    Complex x(1.0, 1.0);
    Complex result;
    p.Evaluate(x, result);
    
    Complex expected(0.0, 5.0);
    ASSERT_TRUE(complexEqual(result, expected));
}

TEST(test_polynomial_with_square_matrix) {
    SquareMatrix<Complex, 2> I(Complex(1, 0));
    SquareMatrix<Complex, 2> A;
    A.at(0, 0) = Complex(1, 0);
    A.at(0, 1) = Complex(2, 0);
    A.at(1, 0) = Complex(3, 0);
    A.at(1, 1) = Complex(4, 0);
    
    using MatrixPoly = Polynomial<SquareMatrix<Complex, 2>>;
    
    SquareMatrix<Complex, 2> coeffs[] = {A, I};
    MatrixPoly p(coeffs, 2);
    
    ASSERT_EQ(p.Degree(), 1);
    
    SquareMatrix<Complex, 2> result;
    p.Evaluate(A, result);
    
    SquareMatrix<Complex, 2> expected = A * Complex(2, 0);
    ASSERT_TRUE(result == expected);
}

// ==================== Test Runner ====================

int runPolynomialTests() {
    std::cout << "\n=== Running Polynomial Lab Tests ===\n\n";
    
    // Complex
    RUN_TEST(test_complex_constructors);
    RUN_TEST(test_complex_getters);
    RUN_TEST(test_complex_addition);
    RUN_TEST(test_complex_subtraction);
    RUN_TEST(test_complex_multiplication);
    RUN_TEST(test_complex_division);
    RUN_TEST(test_complex_comparison);
    RUN_TEST(test_complex_assignment);
    RUN_TEST(test_complex_stream_operators);
    
    // SquareMatrix
    RUN_TEST(test_square_matrix_default_constructor);
    RUN_TEST(test_square_matrix_diagonal_constructor);
    RUN_TEST(test_square_matrix_from_array);
    RUN_TEST(test_square_matrix_from_flat_array);
    RUN_TEST(test_square_matrix_at_method);
    RUN_TEST(test_square_matrix_addition);
    RUN_TEST(test_square_matrix_subtraction);
    RUN_TEST(test_square_matrix_multiplication);
    RUN_TEST(test_square_matrix_scalar_multiplication);
    RUN_TEST(test_square_matrix_comparison);
    RUN_TEST(test_square_matrix_compound_assignment);
    RUN_TEST(test_square_matrix_with_complex);
    
    // Polynomial
    RUN_TEST(test_polynomial_constructors);
    RUN_TEST(test_polynomial_degree_and_coefficients);
    RUN_TEST(test_polynomial_addition);
    RUN_TEST(test_polynomial_subtraction);
    RUN_TEST(test_polynomial_multiplication);
    RUN_TEST(test_polynomial_scalar_multiplication);
    RUN_TEST(test_polynomial_evaluation);
    RUN_TEST(test_polynomial_composition);
    RUN_TEST(test_polynomial_append_set_coefficient);
    RUN_TEST(test_polynomial_map_where_reduce);
    RUN_TEST(test_polynomial_operators);
    RUN_TEST(test_polynomial_with_complex);
    RUN_TEST(test_polynomial_with_square_matrix);
    
    return tests_failed > 0 ? 1 : 0;
}

#endif // TESTS_POLYNOMIAL_H
