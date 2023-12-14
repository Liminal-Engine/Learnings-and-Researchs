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
#include "vulkanhdl/include/_destroyers/_swapChainImagesViewsHandlers/_destroy.hpp"

namespace vulkanhdl {
    namespace _destroyers {
        namespace _swapchainimagesviewshandlers {

            void _destroy(
                const VkDevice &logicalDevice,
                const std::vector<VkImageView> &imagesViewsHandlers,
                VkAllocationCallbacks **ppAllocator
            ) {
                VkAllocationCallbacks **tmpCallback = ppAllocator;
                for (const VkImageView &imageView : imagesViewsHandlers) {
                    vkDestroyImageView(logicalDevice, imageView, ppAllocator != nullptr ? *tmpCallback : nullptr);
                    tmpCallback++;
                }
            }
            
        } // _swapchainimagesviewshandlers
    } // _destroy
} // vulkanhdl

