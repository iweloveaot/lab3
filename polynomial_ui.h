#ifndef _POLYNOMIAL_UI_H_
#define _POLYNOMIAL_UI_H_

#include "polynomial.h"
#include "complex.h"
#include "square_matrix.h"
#include "for_all_ui.h"
#include "polynomial_plot.h"
#include "polynomial_interpolation.h"
#include <fstream>
#include <limits>
#include <type_traits>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

void showMainPolyMenu();
void polynomialMenu();
int selectPolynomialIndex(int count, const std::string& prompt = "Select polynomial (0-");
void interpolationUI();
void PolynomialUI();


template<typename T>
void printPolynomial(const Polynomial<T> &poly, const std::string &name = "");

template<typename T>
Polynomial<T> createPolynomialFromInput();

template<typename T>
void runPolyUIForType();

#include "polynomial_ui.tpp"

#endif // _POLYNOMIAL_UI_H_