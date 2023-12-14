#include "vulkanhdl/include/_destroyers/_swapChain/_destroy.hpp"
/**
 * @file _destroy.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */


namespace vulkanhdl {
    namespace _destroyers {
        namespace _swapchain {

            void _destroy(
                const VkDevice &logicelDevice,
                const VkSwapchainKHR &swapChain,
                VkAllocationCallbacks *pAllocator
            ) {
                vkDestroySwapchainKHR(logicelDevice, swapChain, pAllocator);
            }
            

        } // _swapchain
    } // _destroy
} // vulkanhdl

