#pragma once

#include "window/WindowWrapper.hpp"
#include "vulkan/VulkanWrapper.hpp"

class App {
    public:
        App(void);
        void run(void);

    private:
        window::WindowWrapper _window;
        vulkan::VulkanWrapper _createVulkanWrapper(void);
        vulkan::VulkanWrapper _vulkan;
};