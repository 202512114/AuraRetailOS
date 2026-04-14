#pragma once
#include <iostream>
#include <limits>
#include <string>

namespace aura {

class Input {
public:
    static int getInt(const std::string& prompt, int low, int high) {
        int x;
        while (true) {
            std::cout << prompt;
            if (std::cin >> x && x >= low && x <= high) return x;
            std::cout << "Invalid input. Please try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    static int getNonNegative(const std::string& prompt) {
        int x;
        while (true) {
            std::cout << prompt;
            if (std::cin >> x && x >= 0) return x;
            std::cout << "Please enter a non-negative number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    static std::string getLine(const std::string& prompt) {
        std::string s;
        std::cout << prompt;
        std::getline(std::cin >> std::ws, s);
        return s;
    }
};

} // namespace aura
