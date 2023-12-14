#ifndef VULKAN_HDL_LIB__PRIVATE___DESTROY__SWAPCHAIN__DESTROY_HPP
#define VULKAN_HDL_LIB__PRIVATE___DESTROY__SWAPCHAIN__DESTROY_HPP

/**
 * @file _destroy.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <vulkan/vulkan.h>

namespace vulkanhdl {
    namespace _destroyers {
        namespace _swapchain {

            /**
             * @brief Destroy a swawp chain
             * 
             * @param instance [const VkDevice &] The related logical device
             * @param swapChain [const VkSwapchainKHR &] The related swap chain
             * @param pAllocator [VkAllocationCallbacks *] Vulkan allocation callbacks 
             */
            void _destroy(
                const VkDevice &logicelDevice,
                const VkSwapchainKHR &swapChain,
                VkAllocationCallbacks *pAllocator = nullptr
            );
            

        } // _swapchain
    } // _destroy
} // vulkanhdl


#endif // VULKAN_HDL_LIB__PRIVATE___DESTROY__SWAPCHAIN__DESTROY_HPP