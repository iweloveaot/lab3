// polynomial_plot.h
#ifndef POLYNOMIAL_PLOT_H
#define POLYNOMIAL_PLOT_H

#include "polynomial.h"
#include <fstream>
#include <cmath>
#include <limits>
#include <algorithm>
#include <type_traits>
#include <iostream>


template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, double>::type
evalPolynomialDouble(Polynomial<T>& poly, double x);

template<typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, double>::type
evalPolynomialDouble(Polynomial<T>& poly, double x);

template<typename T>
void plotPolynomialToSVG(Polynomial<T>& poly, 
                         const std::string& filename,
                         double x_min = -10.0,
                         double x_max = 10.0,
                         int width = 800,
                         int height = 600,
                         int samples = 500);


#include "polynomial_plot.tpp"

#endif // POLYNOMIAL_PLOT_H