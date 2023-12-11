#include "vulkanhdl/include/_loaders/_swapChainImagesHandlers/_load.hpp"

#include <stdint.h>
#include <stdexcept>

namespace vulkanhdl {
    namespace _loaders {
        namespace _swapChainImagesHandlers {

            std::vector<VkImage> _load(const VkDevice &logicalDevice, const VkSwapchainKHR &swapChain) {
                // 1. Get the swapChain images count
                uint32_t swapChainNImages = 0;
                vkGetSwapchainImagesKHR(logicalDevice, swapChain, &swapChainNImages, nullptr);
                if (swapChainNImages == 0) {
                    throw std::runtime_error("Error : the swap chain has been loaded with 0 images");
                }
                // 2. Create the res with the found size
                std::vector<VkImage> res(swapChainNImages);
                vkGetSwapchainImagesKHR(logicalDevice, swapChain, &swapChainNImages, res.data());
                return res;
            }
        } // _swapChainImagesHandlers
    } // _loaders
} // vulkanhdl
