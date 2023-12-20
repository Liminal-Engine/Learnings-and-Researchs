#ifndef VULKAN_HDL_LIB__PRIVATE___LOADERS__SWAPCHAIN__LOAD_HPP
#define VULKAN_HDL_LIB__PRIVATE___LOADERS__SWAPCHAIN__LOAD_HPP

/**
 * @file _load.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "liminalvulkanwrapper/include/_queues/_queues.hpp"
#include "liminalvulkanwrapper/include/_swapChain/_supports.hpp"

#include <string>

namespace vulkanhdl {
    namespace _loaders {
        namespace _swapchain {

            /**
             * @brief Load the swap chain
             * 
             * @param logicalDevice [const VkDevice &] The related logical device
             * @param surface [const VkSurfaceKHR &] The related surface
             * @param swapchainSupports [const _swapchain::_SwapChainSupports &] - The device swap chain supports
             * @param queueFamilies [const _queues::_QueueFamilies &] The related queue families
             * @return [VkSwapchainKHR] The created swap chain
             */
            VkSwapchainKHR _load(
                const VkDevice &logicalDevice,
                GLFWwindow *window,
                const VkSurfaceKHR &surface,
                const vulkanhdl::_swapchain::_Supports &swapChainSupports,
                const _queues::_QueueFamilies &queueFamilies,
                const VkSurfaceFormatKHR &swapChainImageFormat,
                const VkExtent2D &swapChainImageExtent
            );
            

        } // _swapchain
    } // _loaders
} // vulkanhdl


#endif // VULKAN_HDL_LIB__PRIVATE___LOADERS__SWAPCHAIN__LOAD_HPP