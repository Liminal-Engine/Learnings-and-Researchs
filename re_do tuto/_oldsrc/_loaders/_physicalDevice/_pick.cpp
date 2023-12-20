#include "lib/liminalvulkanwrapper/include/_loaders/_physicalDevice/_pick.hpp"

#include <GLFW/glfw3.h>
#include <stdint.h>
#include <vector>
#include <stdexcept>

namespace vulkanhdl {
    namespace _loaders {
        namespace _physicalDevice {

            VkPhysicalDevice _pick(
                VkInstance &instance,
                VkSurfaceKHR &surface,
                const std::vector<const char *> &required_extensions
            ) {
                // 1. First, get the number of physical device on the computer
                uint32_t nDevice = 0;
                if (vkEnumeratePhysicalDevices(instance, &nDevice, nullptr) != VK_SUCCESS) {
                    throw std::runtime_error("Failed to enumerate physical device on computer");
                }
                if (nDevice == 0) {
                    throw std::runtime_error("No physical device found on computer");
                }
                // 2. Create vector with found size and fill it
                std::vector<VkPhysicalDevice> physical_devices(nDevice);
                if (vkEnumeratePhysicalDevices(instance, &nDevice, physical_devices.data()) != VK_SUCCESS) {
                    throw std::runtime_error("Failed to enumerate physical device on computer");
                }
                // 3. For each found physical device, check if it is suitable and return the first suitable found. TODO : system of score
                return physical_devices.at(0);
                throw std::runtime_error("Failed to find a valid GPU with the given preferences");
            }
            
        } // namespace _physicalDevice
    } // namespace _loaders
} // namespace vulkanhdl