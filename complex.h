#ifndef _COMPLEX_H_
#define _COMPLEX_H_

#include <iostream>

class Complex {
private:
    double re, im;

public:
    Complex(double re = 0.0, double im = 0.0);
    Complex(const Complex &other) = default;

    double real() const { return re; }
    double imag() const { return im; }

    Complex operator+(const Complex &other) const;
    Complex operator-(const Complex &other) const;
    Complex operator*(const Complex &other) const;
    Complex operator*(double scalar) const;
    Complex operator/(const Complex &other) const;
    Complex operator-() const;

    bool operator==(const Complex &other) const;
    bool operator!=(const Complex &other) const;

    Complex& operator=(const Complex &other) = default;

    friend std::ostream& operator<<(std::ostream& os, const Complex& c);
    friend std::istream& operator>>(std::istream &is, Complex &c);
};


#endif // _COMPLEX_H_