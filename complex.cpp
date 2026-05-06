#include "complex.h"


Complex::Complex(double re, double im) : re(re), im(im) {}

Complex Complex::operator+(const Complex &other) const {
    return Complex(re + other.re, im + other.im);
}

Complex Complex::operator-(const Complex &other) const {
    return Complex(re - other.re, im - other.im);
}

Complex Complex::operator*(const Complex &other) const {
    return Complex(re * other.re - im * other.im,
                   re * other.im + im * other.re);
}

Complex Complex::operator*(double scalar) const {
    return Complex(re * scalar, im * scalar);
}

Complex Complex::operator/(const Complex &other) const {
    double denom = other.re * other.re + other.im * other.im;
    if (denom == 0.0) {
        throw std::runtime_error("Division by zero in Complex::operator/");
    }
    return Complex((re * other.re + im * other.im) / denom,
                   (im * other.re - re * other.im) / denom);
}

Complex Complex::operator-() const { 
    return Complex(-re, -im); 
}

bool Complex::operator==(const Complex &other) const {
    return re == other.re && im == other.im;
}

bool Complex::operator!=(const Complex &other) const { 
    return !(*this == other); 
}

std::ostream& operator<<(std::ostream& os, const Complex& c) {
    os << c.re << (c.im >= 0 ? "+" : "") << c.im << "i";
    return os;
}

std::istream& operator>>(std::istream &is, Complex &c) {
    char sign, i_char;
    is >> c.re >> sign >> c.im >> i_char;
    if (sign == '-') 
        c.im = -c.im;
    return is;
}
