#ifndef _POLYNOMIAL_INTERPOLATION_H_
#define _POLYNOMIAL_INTERPOLATION_H_

#include <math.h>
#include <iostream>
#include "polynomial.h"


void Gaussian(double **X, double *y, double *res, int n) {
    
    for (int k = 0; k < n; ++k) {
        int pivotRow = k;
        double maxAbs = std::fabs(X[k][k]);
        for (int i = k + 1; i < n; ++i) {
            double absVal = std::fabs(X[i][k]);
            if (absVal > maxAbs) {
                maxAbs = absVal;
                pivotRow = i;
            }
        }

        if (pivotRow != k) {
            for (int j = k; j < n; ++j) {
                double tmp = X[k][j];
                X[k][j] = X[pivotRow][j];
                X[pivotRow][j] = tmp;
            }
            double tmp = y[k];
            y[k] = y[pivotRow];
            y[pivotRow] = tmp;
        }

        double diag = X[k][k];
        for (int i = k + 1; i < n; ++i) {
            double factor = X[i][k] / diag;
            if (factor == 0.0) 
                continue;
            for (int j = k; j < n; ++j) {
                X[i][j] -= factor * X[k][j];
            }
            y[i] -= factor * y[k];
        }
    }
            

    for (int i = n - 1; i >= 0; --i) {
        double sum = y[i];
        for (int j = i + 1; j < n; ++j) {
            sum -= X[i][j] * res[j];
        }
        res[i] = sum / X[i][i];
    }
}

Polynomial<double> BaseInterpolation(double *x, double *y, int n) {
    double** X = new double*[n];
    for (int i = 0; i < n; i++) X[i] = new double[n];
    for (int i=0; i<n; i++) for (int j=0; j<n; j++) X[i][j] = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            X[i][j] = pow(x[i], j);
        }
    }

    double result[n];
    for (int i=0; i<n; i++) result[i] = 0;
    Gaussian(X, y, result, n);
    

    Polynomial<double> interpol_poly(result, n);
    for (int i=0; i<n; i++) delete[] X[i];
        delete[] X;
    return interpol_poly;
}


Polynomial<double> BasisPolynomial_i(double *x, int n, int i) {
    double diff[] = {1};
    Polynomial<double> result(diff, 1);
    for (int j = 0; j < n; j++) {
        if (j == i) continue;
        double diff[] = {-x[j], 1};
        Polynomial<double> tmp(diff, 2);
        double t = 1.0/(x[i]-x[j]);
        result *= (tmp * t);
    }
    return result;
}

Polynomial<double> Lagrange(double *x, double *y, int n) {
    Polynomial<double> result(1);
    for (int i = 0; i < n; i++) {
        result += (BasisPolynomial_i(x, n, i) * y[i]);
    }
    return result;
}

#endif /* _POLYNOMIAL_INTERPOLATION_H_ */