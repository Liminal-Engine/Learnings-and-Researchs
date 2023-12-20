#ifndef VULKAN_HDL_LIB__PRIVATE___LOADERS__SURFACE__LOAD_HPP
#define VULKAN_HDL_LIB__PRIVATE___LOADERS__SURFACE__LOAD_HPP

/**
 * @file _load.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace vulkanhdl {
    namespace _loaders {
        namespace _surface {

            /**
             * @brief Pick the best physical device on the computer based on user preferences
             * 
             * @param instance - The Vulkan instance
             * 
             * @return VkPhysicalDevice - The physical device found
             */
            VkSurfaceKHR _load(VkInstance &instance, GLFWwindow *window);

        } // namespace _surface
    } // namespace _loaders
} // namespace vulkanhdl


#endif // VULKAN_HDL_LIB__PRIVATE___LOADERS__SURFACE__LOAD_HPP
