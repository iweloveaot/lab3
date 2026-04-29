#ifndef _COMPLEX_H_
#define _COMPLEX_H_

class Complex {
private:
    double re, im;
public:
    Complex(double re = 0.0, double im = 0.0) : re(re), im(im) {}
    Complex(const Complex &other) = default;
    
    double real() const { 
        return re; 
    }
    double imag() const { 
        return im; 
    }

    Complex operator+(const Complex &other) const {
        return Complex(re + other.re, im + other.im);
    }

    Complex operator-(const Complex &other) const {
        return Complex(re - other.re, im - other.im);
    }

    Complex operator*(const Complex &other) const {
        return Complex(re * other.re - im * other.im,
                       re * other.im + im * other.re);
    }

    Complex operator*(double scalar) const {
        return Complex(re * scalar, im * scalar);
    }

    Complex operator/(const Complex &other) const {
        double denom = other.re * other.re + other.im * other.im;
        return Complex((re * other.re + im * other.im) / denom, (im * other.re - re * other.im) / denom);
    }

    Complex operator-() const { 
        return Complex(-re, -im); 
    }

    bool operator==(const Complex &other) const {
        return re == other.re && im == other.im;
    }
    
    bool operator!=(const Complex &other) const { 
        return !(*this == other); 
    }

    Complex& operator=(const Complex &other) = default;
};


#endif /* _COMPLEX_H_ */