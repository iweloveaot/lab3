#ifndef _POLYNOMIAL_UI_H_
#define _POLYNOMIAL_UI_H_


#include "polynomial.h"
#include "complex.h"
#include "square_matrix.h"
#include "for_all_ui.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

void showMainPolyMenu() {
    std::cout << "======== WORKING WITH POLYNOMIALS ========\n";
    std::cout << "Choose coefficient type:\n";
    std::cout << "1. int\n";
    std::cout << "2. double\n";
    std::cout << "3. Complex\n";
    std::cout << "4. SquareMatrix<int, 3>  \n";
    std::cout << "0. Exit\n";
    std::cout << "Choise: ";
}

void polynomialMenu() {
    std::cout << "\n--- Polynomial Operations ---:\n";
    std::cout << "1. Create polynomial\n";
    std::cout << "2. Append coefficient\n";
    std::cout << "3. Change coefficient\n";
    std::cout << "4. Add two polynomials\n";
    std::cout << "5. Multiply two polynomials\n";
    std::cout << "6. Multiply polynomial by scalar\n";
    std::cout << "7. Calculate composition of polynomials\n";
    std::cout << "8. Delete all polynomials\n";
    std::cout << "9. Show all polynomials\n";
    std::cout << "10. Evaluate polynomial at a value\n";
    std::cout << "0. Back to main menu\n";
    std::cout << "Choice: ";
}

template<typename T>
void printPolynomial(const Polynomial<T> &poly, const std::string &name = "") {
    if (!name.empty()) {
        std::cout << name << ": ";
    }
    if (poly.Degree() < 0 || (poly.Degree() == 0 && poly.GetCoefficient(0) == T())) {
        std::cout << "0";
    } else {
        bool first = true;
        for (int i = poly.Degree(); i >= 0; --i) {
            T coef = poly.GetCoefficient(i);
            if (coef == T()) continue;
            
            if (!first) std::cout << " + ";
            
            if (first || coef != T(1) || i == 0) {
                std::cout << coef;
            }
            if (i > 0) std::cout << "x";
            if (i > 1) std::cout << "^" << i;
            first = false;
        }
        if (first) std::cout << "0"; // все коэффициенты нулевые
    }
    std::cout << " [Degree: " << poly.Degree() << "]\n";
}

template<typename T>
Polynomial<T> createPolynomialFromInput() {
    std::cout << "Enter polynomial degree: ";
    int degree;
    while (!(std::cin >> degree) || degree < 0) {
        std::cout << "Invalid degree. Enter non-negative integer: ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    std::cin.ignore(10000, '\n');
    
    std::vector<T> coeffs(degree + 1);
    std::cout << "Enter " << (degree + 1) << " coefficients (from a0 to a" << degree << "):\n";
    for (int i = 0; i <= degree; ++i) {
        coeffs[i] = readValue<T>("  a[" + std::to_string(i) + "] = ");
    }
    
    return Polynomial<T>(coeffs.data(), degree + 1);
}

int selectPolynomialIndex(int count, const std::string& prompt = "Select polynomial (0-" ) {
    int idx;
    std::cout << prompt << count - 1 << "): ";
    while (!(std::cin >> idx) || idx < 0 || idx >= count) {
        std::cout << "Invalid index. " << prompt << count - 1 << "): ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    std::cin.ignore(10000, '\n');
    return idx;
}

template<typename T>
void runPolyUIForType() {
    Polynomial<T>* polynomials[10] = {nullptr};
    int polyCount = 0;
    
    std::cout << "You can't create more than 10 polynomials during the session!\n";
    int choice;
    
    do {
        polynomialMenu();
        while (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter command 0 to 10: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        std::cin.ignore(10000, '\n');

        try {
            switch (choice) {
                case 1: // Create polynomial
                {
                    if (polyCount >= 10) {
                        std::cout << "Maximum number of polynomials (10) reached!\n";
                        break;
                    }
                    std::cout << "\n--- Creating new polynomial ---\n";
                    polynomials[polyCount] = new Polynomial<T>(createPolynomialFromInput<T>());
                    std::cout << "Polynomial #" << polyCount << " created:\n";
                    printPolynomial(*polynomials[polyCount]);
                    polyCount++;
                    break;
                }
                    
                case 2: // Append coefficient
                {
                    if (polyCount == 0) {
                        std::cout << "No polynomials created yet!\n";
                        break;
                    }
                    int idx = selectPolynomialIndex(polyCount, "Select polynomial to append coefficient: ");
                    T coef = readValue<T>("Enter coefficient to append: ");
                    
                    // AppendCoefficient возвращает новый полином
                    Polynomial<T> newPoly = polynomials[idx]->AppendCoefficient(coef);
                    
                    // Если есть место, создаём новый полином, иначе заменяем текущий
                    if (polyCount < 10) {
                        polynomials[polyCount] = new Polynomial<T>(newPoly);
                        std::cout << "New polynomial #" << polyCount << " created with appended coefficient: \n";
                        printPolynomial(*polynomials[polyCount]);
                        polyCount++;
                    } else {
                        *polynomials[idx] = newPoly;
                        std::cout << "Polynomial #" << idx << " replaced with new version: \n";
                        printPolynomial(*polynomials[idx]);
                    }
                    break;
                }
                    
                case 3: // Change coefficient
                {
                    if (polyCount == 0) {
                        std::cout << "No polynomials created yet!\n";
                        break;
                    }
                    int idx = selectPolynomialIndex(polyCount, "Select polynomial to modify: ");
                    int degree = readValue<int>("Enter degree to change: ");
                    if (degree < 0) {
                        std::cout << "Degree must be non-negative!\n";
                        break;
                    }
                    T newCoef = readValue<T>("Enter new coefficient value: ");
                    
                    // SetCoefficient возвращает новый полином
                    Polynomial<T> newPoly = polynomials[idx]->SetCoefficient(degree, newCoef);
                    
                    if (polyCount < 10) {
                        polynomials[polyCount] = new Polynomial<T>(newPoly);
                        std::cout << "New polynomial #" << polyCount << " created with changed coefficient: \n";
                        printPolynomial(*polynomials[polyCount]);
                        polyCount++;
                    } else {
                        *polynomials[idx] = newPoly;
                        std::cout << "Polynomial #" << idx << " replaced: \n";
                        printPolynomial(*polynomials[idx]);
                    }
                    break;
                }
                    
                case 4: // Add two polynomials
                {
                    if (polyCount < 2) {
                        std::cout << "Need at least 2 polynomials for addition!\n";
                        break;
                    }
                    int idx1 = selectPolynomialIndex(polyCount, "Select first polynomial: ");
                    int idx2 = selectPolynomialIndex(polyCount, "Select second polynomial: ");
                    
                    Polynomial<T> result = (*polynomials[idx1]) + (*polynomials[idx2]);
                    
                    std::cout << "Result of addition: \n";
                    printPolynomial(result, "P" + std::to_string(idx1) + " + P" + std::to_string(idx2));
                    
                    if (polyCount < 10) {
                        polynomials[polyCount] = new Polynomial<T>(result);
                        std::cout << "Stored as polynomial #" << polyCount << "\n";
                        polyCount++;
                    }
                    break;
                }
                    
                case 5: // Multiply two polynomials
                {
                    if (polyCount < 2) {
                        std::cout << "Need at least 2 polynomials for multiplication!\n";
                        break;
                    }
                    int idx1 = selectPolynomialIndex(polyCount, "Select first polynomial: ");
                    int idx2 = selectPolynomialIndex(polyCount, "Select second polynomial: ");
                    
                    Polynomial<T> result = (*polynomials[idx1]) * (*polynomials[idx2]);
                    
                    std::cout << "Result of multiplication: \n";
                    printPolynomial(result, "P" + std::to_string(idx1) + " * P" + std::to_string(idx2));
                    
                    if (polyCount < 10) {
                        polynomials[polyCount] = new Polynomial<T>(result);
                        std::cout << "Stored as polynomial #" << polyCount << "\n";
                        polyCount++;
                    }
                    break;
                }
                    
                case 6: // Multiply polynomial by scalar
                {
                    if (polyCount == 0) {
                        std::cout << "No polynomials created yet!\n";
                        break;
                    }
                    int idx = selectPolynomialIndex(polyCount, "Select polynomial: ");
                    T scalar = readValue<T>("Enter scalar value: ");
                    
                    Polynomial<T> result = (*polynomials[idx]) * scalar;
                    
                    std::cout << "Result: \n";
                    printPolynomial(result);
                    
                    if (polyCount < 10) {
                        polynomials[polyCount] = new Polynomial<T>(result);
                        std::cout << "Stored as polynomial #" << polyCount << "\n";
                        polyCount++;
                    }
                    break;
                }
                    
                case 7: // Calculate composition
                {
                    if (polyCount < 2) {
                        std::cout << "Need at least 2 polynomials for composition!\n";
                        break;
                    }
                    int idx1 = selectPolynomialIndex(polyCount, "Select outer polynomial f(x): ");
                    int idx2 = selectPolynomialIndex(polyCount, "Select inner polynomial g(x): ");
                    
                    Polynomial<T> result = polynomials[idx1]->Composition(*polynomials[idx2]);
                    
                    std::cout << "Result of composition f(g(x)): \n";
                    printPolynomial(result, "P" + std::to_string(idx1) + "(P" + std::to_string(idx2) + "(x))");
                    
                    if (polyCount < 10) {
                        polynomials[polyCount] = new Polynomial<T>(result);
                        std::cout << "Stored as polynomial #" << polyCount << "\n";
                        polyCount++;
                    }
                    break;
                }
                    
                case 8: // Delete all polynomials
                {
                    for (int i = 0; i < polyCount; ++i) {
                        delete polynomials[i];
                        polynomials[i] = nullptr;
                    }
                    polyCount = 0;
                    std::cout << "All polynomials deleted.\n";
                    break;
                }
                    
                case 9: // Show all polynomials
                {
                    if (polyCount == 0) {
                        std::cout << "No polynomials stored.\n";
                        break;
                    }
                    std::cout << "\n--- Stored Polynomials ---\n";
                    for (int i = 0; i < polyCount; ++i) {
                        std::cout << "#" << i << ": ";
                        printPolynomial(*polynomials[i]);
                    }
                    break;
                }

                case 10: // Evaluate polynomial at a value
                {
                    if (polyCount == 0) {
                        std::cout << "No polynomials created yet!\n";
                        break;
                    }
                    int idx = selectPolynomialIndex(polyCount, "Select polynomial to evaluate: ");
                    T x = readValue<T>("Enter value of x: ");
                    
                    T result;
                    polynomials[idx]->Evaluate(x, result);
                    
                    std::cout << "P" << idx << "(" << x << ") = " << result << "\n";
                    break;
                }
                    
                case 0: // Back to main menu
                {
                    // Очистка памяти перед выходом
                    for (int i = 0; i < polyCount; ++i) {
                        delete polynomials[i];
                        polynomials[i] = nullptr;
                    }
                    break;
                }
                    
                default:
                    std::cout << "Invalid choice!\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    } while (choice != 0);
}

void PolynomialUI() {
    
    int choice;
    do {
        showMainPolyMenu();
        while (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter command 0 to 4: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        std::cin.ignore(10000, '\n');
        
        try {
            switch (choice) {
                case 1: {
                    runPolyUIForType<int>();
                    break;
                }
                case 2: {
                    runPolyUIForType<double>();
                    break;
                }
                case 3: {
                    std::cout << "Enter all coefficients like a+(-)bi\n";
                    runPolyUIForType<Complex>();
                    break;
                }
                case 4: {
                    runPolyUIForType<SquareMatrix<int, 3>>();
                    break;
                }
                case 0:
                    std::cout << "Leaving polynomials...\n";
                    break;
                default:
                    std::cout << "Invalid choice!\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Unhandled error: " << e.what() << "\n";
        }
    } while (choice != 0);
    
}
    
#endif /*_POLYNOMIAL_UI_H_*/

