#ifndef VULKAN_HDL_LIB__PRIVATE___DESTROY__SWAPCHAINIMAGESVIEWSHANDLERS__DESTROY_HPP
#define VULKAN_HDL_LIB__PRIVATE___DESTROY__SWAPCHAINIMAGESVIEWSHANDLERS__DESTROY_HPP

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
#include <vector>

namespace vulkanhdl {
    namespace _destroyers {
        namespace _swapchainimagesviewshandlers {

            /**
             * @brief Destroy a swawp chain
             * 
             * @param logicalDevice [const VkDevice &] The related logical device
             * @param imagesViewsHandlers [const std::vector<VkImageView> &] The related swap chain images views handlers
             * @param ppAllocator [VkAllocationCallbacks **] Vulkan allocation callbacks array pointers
             */
            void _destroy(
                const VkDevice &logicalDevice,
                const std::vector<VkImageView> &imagesViewsHandlers,
                VkAllocationCallbacks **ppAllocator = nullptr
            );
            

        } // _swapchainimagesviewshandlers
    } // _destroy
} // vulkanhdl


#endif // VULKAN_HDL_LIB__PRIVATE___DESTROY__SWAPCHAINIMAGESVIEWSHANDLERS__DESTROY_HPP