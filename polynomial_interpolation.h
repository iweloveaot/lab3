#ifndef _POLYNOMIAL_INTERPOLATION_H_
#define _POLYNOMIAL_INTERPOLATION_H_

#include "polynomial.h"


void Gaussian(double **X, double *y, double *res, int n);
Polynomial<double> BaseInterpolation(double *x, double *y, int n);


Polynomial<double> BasisPolynomial_i(double *x, int n, int i);
Polynomial<double> Lagrange(double *x, double *y, int n);

#endif /* _POLYNOMIAL_INTERPOLATION_H_ */