#ifndef VULKAN_HDL_LIB__PRIVATE___LOADERS__SWAPCHAINIMAGESHANDLERS__LOAD_HPP
#define VULKAN_HDL_LIB__PRIVATE___LOADERS__SWAPCHAINIMAGESHANDLERS__LOAD_HPP

#include <vulkan/vulkan.h>

#include <vector>
#include <stdint.h>

namespace vulkanhdl {
    namespace _loaders {
        namespace _swapChainImagesHandlers {

            /**
             * @brief Load the swap chain images handlers
             * 
             * @param logicalDevice [const VkDevice &] - The related logical device
             * @param swapChain [const VkSwapchainKHR &] - The related swap chain
             * @return std::vector<VkImage> 
             */
            std::vector<VkImage> _load(const VkDevice &logicalDevice, const VkSwapchainKHR &swapChain);

        } // _swapChainImagesHandlers
    } // _loaders
} // vulkanhdl

#endif // VULKAN_HDL_LIB__PRIVATE___LOADERS__SWAPCHAINIMAGESHANDLERS__LOAD_HPP