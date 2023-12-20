/**
 * @file _VkPhysicalDevice_wrappers.cpp
 * @author DE VITA Matteo (matteo.devita7@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-15
 * 
 * 
 */

#include "_physical_device/_VkPhysicalDevice_wrappers.hpp"

#include <stdexcept>

namespace vulkan_wrapper {
    namespace _physical_device {

        VkPhysicalDevice _pick(
            const VkInstance &instance,
            const VkSurfaceKHR &surface,
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

    } // _physical_device
} // vulkan_wrapper