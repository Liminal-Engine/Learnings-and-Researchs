#ifndef VULKAN_HDL_LIB__PRIVATE___LOADERS__RENDERPASS__LOAD_HPP
#define VULKAN_HDL_LIB__PRIVATE___LOADERS__RENDERPASS__LOAD_HPP

/**
 * @file _load.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <vulkan/vulkan.h>

namespace vulkanhdl {
    namespace _loaders {
        namespace _renderpass {

            /**
             * @brief 
             * 
             * @param logicalDevice [const VkDevice &] The related logcial device
             * @param swapChainImagesFormat [const VkFormat &] The related swap chain images format
             * @return VkRenderPass The created render pass
             */
            VkRenderPass _load(const VkDevice &logicalDevice, const VkFormat &swapChainImagesFormat);
           
        } // namespace _renderpass
    } // namespace _loaders
} // namespace vulkanhdl


#endif // VULKAN_HDL_LIB__PRIVATE___LOADERS__RENDERPASS__LOAD_HPP
