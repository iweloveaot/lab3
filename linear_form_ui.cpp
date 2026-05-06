#include "linear_form_ui.h"

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

int selectLinearFormIndex(int count, const std::string& prompt) {
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
                    std::cout << "Leaving linear forms...\n";
                    break;
                default:
                    std::cout << "Invalid choice!\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Unhandled error: " << e.what() << "\n";
        }
    } while (choice != 0);
}