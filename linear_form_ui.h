#ifndef _LINEAR_FORM_UI_H_
#define _LINEAR_FORM_UI_H_

#include "linear_form.h"
#include "complex.h"
#include "for_all_ui.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

void showMainLinfMenu() {
    std::cout << "======== WORKING WITH LINEAR FORM ========\n";
    std::cout << "Choose coefficient type:\n";
    std::cout << "1. int\n";
    std::cout << "2. double\n";
    std::cout << "3. Complex\n";
    std::cout << "0. Exit\n";
    std::cout << "Choise: ";
}

void linearFormMenu() {
    std::cout << "\n--- Linear Form Operations ---:\n";
    std::cout << "1. Create linear form\n";
    std::cout << "2. Append coefficient\n";
    std::cout << "3. Change coefficient\n";
    std::cout << "4. Add two linear forms\n";
    std::cout << "5. Multiply linear form by scalar\n";
    std::cout << "6. Delete all linear form\n";
    std::cout << "7. Show all linear form\n";
    std::cout << "8. Evaluate linear form at a point\n";
    std::cout << "0. Back to main menu\n";
    std::cout << "Choice: ";
}

template<typename T>
void printLinearForm(const LinearForm<T> &linf, const std::string &name = "") {
    if (!name.empty()) {
        std::cout << name << ": ";
    }
    if (linf.CoefficientCount() < 0 || (linf.CoefficientCount() == 0 && linf.GetCoefficient(0) == T())) {
        std::cout << "0";
    } else {
        bool first = true;
        for (int i = 0; i < linf.CoefficientCount(); i++) {
            T coef = linf.GetCoefficient(i);
            if (coef == T()) continue;
            
            if (!first) std::cout << " + ";
            
            if (first || coef != T(1) || i == 0) {
                std::cout << coef;
            }
            if (i > 0) std::cout << "x(" << i << ")";
            first = false;
        }
        std::cout << "\n";
        if (first) std::cout << "0"; // все коэффициенты нулевые
    }
}

template<typename T>
LinearForm<T> createLinearFormFromInput() {
    std::cout << "Enter number of veriables: ";
    int num;
    while (!(std::cin >> num) || num < 0) {
        std::cout << "Invalid number. Enter non-negative integer: ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    std::cin.ignore(10000, '\n');
    
    std::vector<T> coeffs(num + 1);
    std::cout << "Enter " << (num + 1) << " coefficients (from a0 to a" << num << "):\n";
    for (int i = 0; i <= num; i++) {
        coeffs[i] = readValue<T>("  a[" + std::to_string(i) + "] = ");
    }
    
    return LinearForm<T>(coeffs.data(), num + 1);
}

int selectLinearFormIndex(int count, const std::string& prompt = "Select linear form (0-" ) {
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
void runUIForType() {
    LinearForm<T>* linforms[10] = {nullptr};
    int linfCount = 0;
    
    std::cout << "You can't create more than 10 linear forms during the session!\n";
    int choice;
    
    do {
        linearFormMenu();
        while (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter command 0 to 8: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        std::cin.ignore(10000, '\n');

        try {
            switch (choice) {
                case 1: // Create linear form
                {
                    if (linfCount >= 10) {
                        std::cout << "Maximum number of linear forms (10) reached!\n";
                        break;
                    }
                    std::cout << "\n--- Creating new linear form ---\n";
                    linforms[linfCount] = new LinearForm<T>(createLinearFormFromInput<T>());
                    std::cout << "Linear Form #" << linfCount << " created:\n";
                    printLinearForm(*linforms[linfCount]);
                    linfCount++;
                    break;
                }
                    
                case 2: // Append coefficient
                {
                    if (linfCount == 0) {
                        std::cout << "No linear forms created yet!\n";
                        break;
                    }
                    int idx = selectLinearFormIndex(linfCount, "Select linear form to append coefficient (0-");
                    T coef = readValue<T>("Enter coefficient to append: ");
                    
                    LinearForm<T> newLinf = linforms[idx]->AppendCoefficient(coef);
                    
                    if (linfCount < 10) {
                        linforms[linfCount] = new LinearForm<T>(newLinf);
                        std::cout << "New linear form #" << linfCount << " created with appended coefficient: \n";
                        printLinearForm(*linforms[linfCount]);
                        linfCount++;
                    } else {
                        *linforms[idx] = newLinf;
                        std::cout << "Linear Form #" << idx << " replaced with new version: \n";
                        printLinearForm(*linforms[idx]);
                    }
                    break;
                }
                    
                case 3: // Change coefficient
                {
                    if (linfCount == 0) {
                        std::cout << "No linear forms created yet!\n";
                        break;
                    }
                    int idx = selectLinearFormIndex(linfCount, "Select linear form to modify (0-");
                    int num = readValue<int>("Enter coefficient number to change: ");
                    if (num < 0) {
                        std::cout << "Number must be non-negative!\n";
                        break;
                    }
                    T newCoef = readValue<T>("Enter new coefficient value: ");
                    
                    LinearForm<T> newLinf = linforms[idx]->SetCoefficient(num, newCoef);
                    
                    if (linfCount < 10) {
                        linforms[linfCount] = new LinearForm<T>(newLinf);
                        std::cout << "New linear form #" << linfCount << " created with changed coefficient: \n";
                        printLinearForm(*linforms[linfCount]);
                        linfCount++;
                    } else {
                        *linforms[idx] = newLinf;
                        std::cout << "Linear Form #" << idx << " replaced: \n";
                        printLinearForm(*linforms[idx]);
                    }
                    break;
                }
                    
                case 4: // Add two linear forms
                {
                    if (linfCount < 2) {
                        std::cout << "Need at least 2 linear forms for addition!\n";
                        break;
                    }
                    int idx1 = selectLinearFormIndex(linfCount, "Select first linear form (0-");
                    int idx2 = selectLinearFormIndex(linfCount, "Select second linear form (0-");
                    
                    LinearForm<T> result = (*linforms[idx1]) + (*linforms[idx2]);
                    
                    std::cout << "Result of addition: \n";
                    printLinearForm(result, "F" + std::to_string(idx1) + " + F" + std::to_string(idx2));
                    
                    if (linfCount < 10) {
                        linforms[linfCount] = new LinearForm<T>(result);
                        std::cout << "Stored as linear form #" << linfCount << "\n";
                        linfCount++;
                    }
                    break;
                }
                    
                case 5: // Multiply polynomial by scalar
                {
                    if (linfCount == 0) {
                        std::cout << "No linear forms created yet!\n";
                        break;
                    }
                    int idx = selectLinearFormIndex(linfCount, "Select linera form (0-");
                    T scalar = readValue<T>("Enter scalar value: ");
                    
                    LinearForm<T> result = (*linforms[idx]) * scalar;
                    
                    std::cout << "Result: \n";
                    printLinearForm(result);
                    
                    if (linfCount < 10) {
                        linforms[linfCount] = new LinearForm<T>(result);
                        std::cout << "Stored as linear form #" << linfCount << "\n";
                        linfCount++;
                    }
                    break;
                }
                    
                case 6: // Delete all linear forms
                {
                    for (int i = 0; i < linfCount; ++i) {
                        delete linforms[i];
                        linforms[i] = nullptr;
                    }
                    linfCount = 0;
                    std::cout << "All linear forms deleted.\n";
                    break;
                }
                    
                case 7: // Show all linear forms
                {
                    if (linfCount == 0) {
                        std::cout << "No linear forms stored.\n";
                        break;
                    }
                    std::cout << "\n--- Stored Linear Forms ---\n";
                    for (int i = 0; i < linfCount; ++i) {
                        std::cout << "#" << i << ": ";
                        printLinearForm(*linforms[i]);
                    }
                    break;
                }

                case 8: // Evaluate linear form at a point
                {
                    if (linfCount == 0) {
                        std::cout << "No linear forms created yet!\n";
                        break;
                    }
                    int idx = selectLinearFormIndex(linfCount, "Select linear form to evaluate (0-");
                    int l = linforms[idx]->CoefficientCount();
                    T x[l];
                    for (int i = 0; i < l-1; i++) { 
                        std::cout << "Enter value of x(" << i+1 << "): "; 
                        x[i] = readValue<T>("");
                    }                    
                    T result;
                    linforms[idx]->Evaluate(x, result);
                    
                    std::cout << "F" << idx << "(";
                    for (int i = 0; i < l-1; i++) {
                        std::cout << x[i] << ", ";
                    }
                    std::cout << ") = " << result << "\n";
                    break;
                }
                    
                case 0: // Back to main menu
                {
                    // Очистка памяти перед выходом
                    for (int i = 0; i < linfCount; ++i) {
                        delete linforms[i];
                        linforms[i] = nullptr;
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

void LinearFormUI() {
    
    int choice;
    do {
        showMainLinfMenu();
        while (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter command 0 to 3: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        std::cin.ignore(10000, '\n');
        
        try {
            switch (choice) {
                case 1: {
                    runUIForType<int>();
                    break;
                }
                case 2: {
                    runUIForType<double>();
                    break;
                }
                case 3: {
                    std::cout << "Enter all coefficients like a+(-)bi\n";
                    runUIForType<Complex>();
                    break;
                }
                case 0:
                    std::cout << "Leavind linear forms...\n";
                    break;
                default:
                    std::cout << "Invalid choice!\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Unhandled error: " << e.what() << "\n";
        }
    } while (choice != 0);
    
}
    
#endif /*_LINEAR_FORM_UI_H_*/

