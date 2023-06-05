#include "App.hpp"

#include <vector>
#include <iostream>
#include <stdexcept>
// #include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

App::App(void)
:
_window{window::WindowWrapper(800, 600, "Basic App")},
_vulkan{vulkan::VulkanWrapper(this->_window)}
{
    
}

void App::run(void) {
    while (this->_window.shouldClose() == false) {
        this->_window.pollEvents();
        this->_vulkan.drawFrame();
    }
    this->_vulkan.waitForDeviceToFinish();
}

