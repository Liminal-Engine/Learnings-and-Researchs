
// #include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "HelloTriangleApplication.hpp"

#include <cstdlib>
#include <stdexcept>
#include <iostream>

int main() {
    HelloTriangeApplication app;

    try {
        app.run();
    } catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}