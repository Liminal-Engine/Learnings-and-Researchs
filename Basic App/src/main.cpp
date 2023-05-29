#include "App.hpp"

#include <stdexcept>
#include <iostream>

int main() {
    try {
        App app;

        app.run();
    } catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}