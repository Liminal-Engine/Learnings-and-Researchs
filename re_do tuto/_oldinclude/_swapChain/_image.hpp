#ifndef VULKAN_HDL_LIB__PRIVATE___SWAPCHAIN__IMAGE_HPP
#define VULKAN_HDL_LIB__PRIVATE___SWAPCHAIN__IMAGE_HPP

/**
 * @file _image.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>

namespace vulkanhdl {
    namespace _swapchain {
       
        /**
         * @brief Get the best possible swap chain presentation modes
         * 
         * @param availablePresentationModes [const std::vector<VkPresentModeKHR> &] - The available presentation modes
         * @return VkPresentModeKHR The best presentation mode found
         */
        VkPresentModeKHR _getBestPresentationMode(const std::vector<VkPresentModeKHR> &availablePresentationModes);

        /**
         * @brief Get the best possible swap chain surface format
         * 
         * @param supportedFormats [const std::vector<VkSurfaceFormatKHR> &] The supported formats
         * @return VkSurfaceFormatKHR The best surface format
         */
        VkSurfaceFormatKHR _getBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &supportedFormats);

        /**
         * @brief Get the best possible swap chain image extent
         * 
         * @param window [GLFWwindow *] -  The related window
         * @param surfaceCapabilities [const VkSurfaceCapabilitiesKHR &surfaceCapabilities] - The supported surface capabilities
         * @return VkExtent2D The best extent found
         */
        VkExtent2D _getBestExtent(GLFWwindow *window, const VkSurfaceCapabilitiesKHR &surfaceCapabilities);

    } // _swapchain
} // namespace vulkanhdl

#endif // VULKAN_HDL_LIB__PRIVATE___SWAPCHAIN__IMAGE_HPP
