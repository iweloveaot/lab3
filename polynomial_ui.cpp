#include "polynomial_ui.h"

void showMainPolyMenu() {
    std::cout << "\n======== WORKING WITH POLYNOMIALS ========\n";
    std::cout << "Choose coefficient type or operation:\n";
    std::cout << "1. int\n";
    std::cout << "2. double\n";
    std::cout << "3. Complex\n";
    std::cout << "4. SquareMatrix<int, 3>  \n";
    std::cout << "5. Polynomial interpolation;\n";
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
    std::cout << "11. Plot polynomial to SVG file\n";
    std::cout << "0. Back to main menu\n";
    std::cout << "Choice: ";
}

int selectPolynomialIndex(int count, const std::string& prompt) {
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

void interpolationUI() {
    int dots = readValue<int>("Enter number of points: ");
    // Используем вектор вместо VLA для стандартного C++
    std::vector<double> x(dots), y(dots);
    
    for (int i = 0; i < dots; i++) {
        x[i] = readValue<double>("Enter x" + std::to_string(i+1) + ": ");
        y[i] = readValue<double>("Enter y" + std::to_string(i+1) + ": ");
    }

    int inter_choice = readValue<int>("Enter 1 - basic interpolation (slau solution) or 2 - interpolation with Lagrange formula: ");
    while (inter_choice != 1 && inter_choice != 2) {
        std::cout << "Invalid input! Enter only 1 or 2\n";
        inter_choice = readValue<int>("Enter 1 - basic interpolation (slau solution) or 2 - interpolation with Lagrange formula: ");
    }

    Polynomial<double> interpolated;
    if (inter_choice == 1) interpolated = BaseInterpolation(x.data(), y.data(), dots);
    else if (inter_choice == 2) interpolated = Lagrange(x.data(), y.data(), dots);

    std::cout << "Result of interpolation: \n";
    printPolynomial(interpolated, "");

    int graph_choice = readValue<int>("Do you want to see the plot? (1 - yes/ 0 - no): ");
    while (graph_choice != 1 && graph_choice != 0) {
        std::cout << "Invalid input! Enter only 1 or 0\n";
        graph_choice = readValue<int>("Do you want to see the plot? (1 - yes/ 0 - no): ");
    }

    if (graph_choice) {
        double x_min = readValue<double>("Enter X min (default -10): ");
        double x_max = readValue<double>("Enter X max (default 10): ");
        if (x_min >= x_max) {
            std::cout << "Invalid range! Using default [-10, 10]\n";
            x_min = -10;
            x_max = 10;
        }
                    
        std::string filename;
        std::cout << "Enter output filename (default: plot.svg): ";
        std::getline(std::cin, filename);
        if (filename.empty()) filename = "plot.svg";
        if (filename.find(".svg") == std::string::npos) {
            filename += ".svg";
        }
                    
        try {
            plotPolynomialToSVG(interpolated, filename, x_min, x_max);
            std::cout << "SVG graph generated successfully!\n";
        } catch (const std::exception& e) {
            std::cerr << "Error plotting: " << e.what() << "\n";
        }
    }
}

void PolynomialUI() {
    int choice;
    do {
        showMainPolyMenu();
        while (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter command 0 to 5: ";
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
                case 5: {
                    interpolationUI();
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