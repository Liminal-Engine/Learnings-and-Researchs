#ifndef VULKAN___SWAP_CHAIN___SWAPCHAINWRAPPER_H
#define VULKAN___SWAP_CHAIN___SWAPCHAINWRAPPER_H

#include <vulkan/vulkan.h>
#include <vector>

#include "vulkan/device/DeviceWrapper.hpp"

namespace vulkan::swap_chain {
  
    class SwapChainWrapper {

        public:
            //Struct containing details about swap chain supports available.
            //Is used to check compatibilites of swap chain

            SwapChainWrapper(void);

            SwapChainWrapper(
                const vulkan::device::DeviceWrapper &deviceWrapper,
                const VkExtent2D &windowFrameBufferSize,
                const VkSurfaceKHR &surface,
                vulkan::QueueFamilies deviceQueueFamilies
            );
            void cleanUp(const VkDevice &logicalDevice);

            static SwapChainSupportDetails STATIC_querySupport(
                const VkPhysicalDevice &device,
                const VkSurfaceKHR &surface
            );

        private:

            virtual VkSurfaceFormatKHR _getOptimalSurfaceFormat(
                const std::vector<VkSurfaceFormatKHR> &availableFormats
            ); //create an interface where you must rewrite all methods or a parent class with at least this one = 0
            virtual VkPresentModeKHR _getOptimalPresentMode(
                const std::vector<VkPresentModeKHR> &availablePresentModes
            ); // = 0
            virtual VkExtent2D _getOptimalSwapExtent(
                VkSurfaceCapabilitiesKHR capabilities,
                const VkExtent2D &windowFrameBufferSize
            ); //=0

            VkSwapchainKHR _createSwapChain(
                const vulkan::device::DeviceWrapper &deviceWrapper,
                const VkExtent2D &windowFrameBufferSize,
                const VkSurfaceKHR &surface,
                vulkan::QueueFamilies deviceQueueFamilies
            );

            std::vector<VkImage> _createSwapChainImages(
                const VkDevice &logicalDevice
            );

            std::vector<VkImageView> _createImageViews(
                const VkDevice &logicalDevice
            );

            VkFormat _imageFormat;
            VkExtent2D _extent;
            VkSwapchainKHR _swapChain;
            std::vector<VkImage> _images;
            std::vector<VkImageView> _imageViews;
    };

}

#endif /*VULKAN___SWAP_CHAIN___SWAPCHAINWRAPPER_H*/