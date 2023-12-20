#include "liminalvulkanwrapper/include/_loaders/_swapChain/_load.hpp"
#include "liminalvulkanwrapper/include/_swapChain/_image.hpp"

#include <stdexcept>
#include <vector>
#include <limits>

namespace vulkanhdl {
    namespace _loaders {
        namespace _swapchain {

            VkSwapchainKHR _load(
                const VkDevice &logicalDevice,
                GLFWwindow *window,
                const VkSurfaceKHR &surface,
                const vulkanhdl::_swapchain::_Supports &swapChainSupports,
                const _queues::_QueueFamilies &queueFamilies,
                const VkSurfaceFormatKHR &swapChainImageFormat,
                const VkExtent2D &swapChainImageExtent
            ) {
                VkPresentModeKHR bestPresentationMode = vulkanhdl::_swapchain::_getBestPresentationMode(swapChainSupports._presentationModes);

                // + 1 to avoid wait for the driver to complete internal ops before we can acquire anoter image to render to
                uint32_t nImages = swapChainSupports._surfaceCapabilities.minImageCount + 1;
                // make sure we do not exceed
                if (swapChainSupports._surfaceCapabilities.maxImageCount > 0 && nImages > swapChainSupports._surfaceCapabilities.maxImageCount) {
                    nImages = swapChainSupports._surfaceCapabilities.maxImageCount;
                }
                VkSwapchainCreateInfoKHR createInfo{};
                createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
                createInfo.surface = surface;
                createInfo.minImageCount = nImages;
                createInfo.imageFormat = swapChainImageFormat.format;
                createInfo.imageColorSpace = swapChainImageFormat.colorSpace;
                createInfo.imageExtent = swapChainImageExtent;
                createInfo.imageArrayLayers = 1; // Nb of layer each image consist of
                createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
                createInfo.preTransform = swapChainSupports._surfaceCapabilities.currentTransform;
                createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
                createInfo.presentMode = bestPresentationMode;
                createInfo.clipped = VK_TRUE;
                createInfo.oldSwapchain = VK_NULL_HANDLE;

                if (queueFamilies._graphicsFamily != queueFamilies._presentFamily) {
                    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT; // Image is owned by one queue families at a time
                    createInfo.queueFamilyIndexCount = 2;
                } else {
                    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // Image can be owned by queue families
                    createInfo.queueFamilyIndexCount = 0;
                }
                VkSwapchainKHR res{};
                createInfo.pQueueFamilyIndices = queueFamilies._toVector().data();
                if (vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &res) != VK_SUCCESS) {
                    throw std::runtime_error("Failed to create swap chain");
                }
                return res;
            }

        } // _swapchain
    } // _loaders
} // vulkanhdl
