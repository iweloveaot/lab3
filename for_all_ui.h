#ifndef _FOR_ALL_UI_H_
#define _FOR_ALL_UI_H_

#include <iostream>
#include <string>
#include <sstream>

template<typename T>
T readValue(const std::string& prompt) {
    T value;
    std::cout << prompt;
    while (!(std::cin >> value)) {
        std::cout << "Invalid input. " << prompt;
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    std::cin.ignore(10000, '\n');
    return value;
}

#endif /*_FOR_ALL_UI_H_*/