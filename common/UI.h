#pragma once
#include <iostream>
#include <string>

namespace aura {

class UI {
public:
    static void line(char ch = '=') {
        std::cout << std::string(86, ch) << "\n";
    }

    static void title(const std::string& text) {
        line('=');
        std::cout << " " << text << "\n";
        line('=');
    }

    static void section(const std::string& text) {
        std::cout << "\n";
        line('-');
        std::cout << " " << text << "\n";
        line('-');
    }
};

} // namespace aura
