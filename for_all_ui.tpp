#ifndef _FOR_ALL_UI_TPP_
#define _FOR_ALL_UI_TPP_

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

#endif /*_FOR_ALL_UI_TPP_*/