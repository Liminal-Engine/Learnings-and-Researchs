#ifndef VULKAN_HDL_LIB__PRIVATE___LOADERS__SWAPCHAINIMAGESVIEWSHANDLERS__LOAD_HPP
#define VULKAN_HDL_LIB__PRIVATE___LOADERS__SWAPCHAINIMAGESVIEWSHANDLERS__LOAD_HPP

#include <vulkan/vulkan.h>

#include <vector>

namespace vulkanhdl {
    namespace _loaders {
        namespace _swapChainImagesViewsHandlers {

            /**
             * @brief Load the swap chain images views handlers
             * 
             * @param logicalDevice [const VkDevice &] - The related logical device
             * @param swapChainImagesHandlers [const std::vector<VkImage> &] - The related swap chain images handlers
             * @return std::vector<VkImage> 
             */
            std::vector<VkImageView> _load(
                const VkDevice &logicalDevice,
                const std::vector<VkImage> &swapChainImagesHandlers,
                const VkFormat &swapChainImagesFormat
            );

        } // _swapChainImagesViewsHandlers
    } // _loaders
} // vulkanhdl

#endif // VULKAN_HDL_LIB__PRIVATE___LOADERS__SWAPCHAINIMAGESVIEWSHANDLERS__LOAD_HPP