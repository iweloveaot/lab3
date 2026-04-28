#include <iostream>
#include "polynomial.h"

int main() {
    int data[] = {5, 2, 3};
    Polynomial<int> pol(data, 3);
    
    int data2[] = {2, 1};
    Polynomial<int> pol2(data2, 2);

    Polynomial<int> compose = pol.Composition(pol2);

    for (int i=0; i<=compose.Degree(); i++) {
        std::cout << compose.GetCoefficient(i) << " ";
    }
}
