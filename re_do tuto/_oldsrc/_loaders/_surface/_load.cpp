#include "lib/liminalvulkanwrapper/include/_loaders/_surface/_load.hpp"

#include <stdexcept>

namespace vulkanhdl {
    namespace _loaders {
        namespace _surface {

            VkSurfaceKHR _load(VkInstance &instance, GLFWwindow *window) {
                VkSurfaceKHR surface;

                if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
                    std::runtime_error("Error while creating window surface");
                }
                return surface;
            }

        } // namespace _surface
    } // namespace _loaders
} // namespace vulkanhdl