#include "liminalvulkanwrapper/include/_loaders/_swapChainImagesViewsHandlers/_load.hpp"

#include <stdexcept>

namespace vulkanhdl {
    namespace _loaders {
        namespace _swapChainImagesViewsHandlers {

            std::vector<VkImageView> _load(
                const VkDevice &logicalDevice,
                const std::vector<VkImage> &swapChainImagesHandlers,
                const VkFormat &swapChainImagesFormat
            ) {
                std::vector<VkImageView> res(swapChainImagesHandlers.size());

                for (size_t i = 0; i < swapChainImagesHandlers.size(); i++) {
                    VkImageViewCreateInfo createInfo{};
                    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                    createInfo.image = swapChainImagesHandlers[i];
                    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; //treat image as 1D, 2D, 3D textures or else
                    createInfo.format = swapChainImagesFormat;
                    // Allow to swizzle color channels
                    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
                    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
                    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
                    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
                    // subresourceRange describes the purpose of the image and which part of it should be accessed
                    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                    createInfo.subresourceRange.baseMipLevel = 0;
                    createInfo.subresourceRange.levelCount = 1;
                    createInfo.subresourceRange.baseArrayLayer = 0;
                    createInfo.subresourceRange.layerCount = 1;

                    if (vkCreateImageView(logicalDevice, &createInfo, nullptr, &res[i]) != VK_SUCCESS) {
                        throw std::runtime_error("Cannot create image view");
                    }
                }
                return res;
            }

        } // _swapChainImagesViewsHandlers
    } // _loaders
} // vulkanhdl
