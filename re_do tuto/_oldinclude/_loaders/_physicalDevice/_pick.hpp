#ifndef VULKAN_HDL_LIB__PRIVATE___LOADERS__PHYSICALDEVICE__PICK_HPP
#define VULKAN_HDL_LIB__PRIVATE___LOADERS__PHYSICALDEVICE__PICK_HPP

/**
 * @file _pick.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vector>


namespace vulkanhdl {
    namespace _loaders {
        namespace _physicalDevice {

            /**
             * @brief Pick the best physical device on the computer based on user preferences
             * 
             * @param instance [VkInstance &] The Vulkan instance
             * @param surface [VkSurfaceKHR &] The surface
             * @param required_extensions [const std::vector<const char *> &] A std::vector representing the physical
             * device required extensions. TODO : change this to be a smaters user preferences
             * 
             * @return VkPhysicalDevice - The physical device found
             */
            VkPhysicalDevice _pick(
                VkInstance &instance,
                VkSurfaceKHR &surface,
                const std::vector<const char *> &required_extensions
            );

        } // namespace _physicalDevice
    } // namespace _loaders
} // namespace vulkanhdl

#endif // VULKAN_HDL_LIB__PRIVATE___LOADERS__PHYSICALDEVICE__PICK_HPP
