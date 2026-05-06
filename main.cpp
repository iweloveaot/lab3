#include "polynomial_ui.h"
#include "linear_form_ui.h"
#include "polynomial_and_linearform_tests.h"

int main() {
    int choice;
    do {
        std::cout << "\n ---  POLYNOMIALS & LINFORMS  --- \n";
        std::cout << "1. work with Polynomials \n";
        std::cout << "2. work with Linear forms \n";
        std::cout << "3. run tests \n";
        std::cout << "0. Exit \n";
        std::cout << "Choice: ";

        while (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter command 0 to 3: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        std::cin.ignore(10000, '\n');
        
        try {
            switch (choice) {
                case 1: {
                    PolynomialUI();
                    break;
                }
                case 2: {
                    LinearFormUI();
                    break;
                }
                case 3: {
                    runPolynomialAndLinearFormTests();
                    break;
                }
                case 0:
                    std::cout << "Bye-bye!\n";
                    break;
                default:
                    std::cout << "Invalid choice!\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Unhandled error: " << e.what() << "\n";
        }
    } while (choice != 0);
    
    return 0;
}