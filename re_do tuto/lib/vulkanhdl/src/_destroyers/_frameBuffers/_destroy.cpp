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
#include "vulkanhdl/include/_destroyers/_frameBuffers/_destroy.hpp"

namespace vulkanhdl {
    namespace _destroyers {
        namespace _framebuffers {

            void _destroy(
                const VkDevice &logicalDevice,
                const std::vector<VkFramebuffer> &frameBuffers,
                VkAllocationCallbacks **ppAllocator
            ) {
                VkAllocationCallbacks **tmpCallback = ppAllocator;
                for (const VkFramebuffer &frameBuffer : frameBuffers) {
                    vkDestroyFramebuffer(logicalDevice, frameBuffer, ppAllocator != nullptr ? *tmpCallback : nullptr);
                    tmpCallback++;
                }
            }
            
        } // _framebuffers
    } // _destroy
} // vulkanhdl

