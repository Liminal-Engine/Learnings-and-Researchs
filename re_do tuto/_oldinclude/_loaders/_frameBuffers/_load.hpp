#ifndef VULKAN_HDL_LIB__PRIVATE___LOADERS__FRAMEBUFFERS__LOAD_HPP
#define VULKAN_HDL_LIB__PRIVATE___LOADERS__FRAMEBUFFERS__LOAD_HPP

/**
 * @file _load.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <vulkan/vulkan.h>
#include <vector>

namespace vulkanhdl {
    namespace _loaders {
        namespace _framebuffers {

            /**
             * @brief Load the frame buffers
             * 
             * @param logicalDevice [const VkDevice &] The related logical device
             * @param swapChainImageView [const std::vector<VkImageView> &] The related swap chain image views
             * @param swapChainExtent [const VkExtent2D &] The related swap chain extent
             * @param renderPass [const VkRenderPass &] The related render pass
             * @return std::vector<VkFramebuffer> 
             */
            std::vector<VkFramebuffer> _load(
                const VkDevice &logicalDevice,
                const std::vector<VkImageView> &swapChainImageView,
                const VkExtent2D &swapChainExtent,
                const VkRenderPass &renderPass
            );

        } // namespace _framebuffers
    } // namespace _loaders
} // namespace vulkanhdl


#endif // VULKAN_HDL_LIB__PRIVATE___LOADERS__FRAMEBUFFERS__LOAD_HPP
