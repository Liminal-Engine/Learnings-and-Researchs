#include "vulkan/swap_chain/SwapChainWrapper.hpp"

#include <limits>
#include <algorithm>
#include <stdexcept>

namespace vulkan::swap_chain {

    SwapChainWrapper::SwapChainWrapper(void)
    :
    _swapChain{VK_NULL_HANDLE}
    {};

    SwapChainWrapper::SwapChainWrapper(
        const vulkan::device::DeviceWrapper &deviceWrapper,
        const VkExtent2D &windowFrameBufferSize,
        const VkSurfaceKHR &surface,
        vulkan::QueueFamilies deviceQueueFamilies
    )
    :
    _swapChain{this->_createSwapChain(
        deviceWrapper,
        windowFrameBufferSize,
        surface,
        deviceQueueFamilies
    )}
    {
        
    }
    
    void SwapChainWrapper::cleanUp(const VkDevice &logicalDevice) {
        vkDestroySwapchainKHR(logicalDevice, this->_swapChain, nullptr);
    }


    vulkan::SwapChainSupportDetails
    SwapChainWrapper::STATIC_querySupport(
        const VkPhysicalDevice &device,
        const VkSurfaceKHR &surface
    ) {
        SwapChainSupportDetails details;

        //1. Query surface capabilites
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            device,
            surface,
            &details.capabilities
        );
        //2. Query list of supported surface formats
        uint32_t nFormats = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(
            device, surface, &nFormats, nullptr            
        );
        if (nFormats != 0) {
            details.formats.resize(nFormats);
            vkGetPhysicalDeviceSurfaceFormatsKHR(
                device, surface, &nFormats, details.formats.data()            
            );
        }
        //3. Query the supported presentation modes
        uint32_t nPresentModes = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(
            device, surface, &nPresentModes, nullptr
        );
        if (nPresentModes != 0) {
            details.presentModes.resize(nPresentModes);
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                device, surface, &nPresentModes, details.presentModes.data()
            );  
        }
        return details;
    }

    //private
    VkSurfaceFormatKHR SwapChainWrapper::_getOptimalSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR> &availableFormats
    ) {
        for (const VkSurfaceFormatKHR &format : availableFormats) {
            if (
                format.format == VK_FORMAT_B8G8R8A8_SRGB &&
                format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
            ) {
                return format;
            }
        }
        return availableFormats[0];
    }

    VkPresentModeKHR SwapChainWrapper::_getOptimalPresentMode(
        const std::vector<VkPresentModeKHR> &availablePresentModes
    ) {
        for (const VkPresentModeKHR &presentMode : availablePresentModes) {
            if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return presentMode;
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D SwapChainWrapper::_getOptimalSwapExtent(
        VkSurfaceCapabilitiesKHR capabilities,
        const VkExtent2D &windowFrameBufferSize
    ) {
        if (
            capabilities.currentExtent.width !=
            std::numeric_limits<uint32_t>::max()
        ) {
            return capabilities.currentExtent;
        }
        return (VkExtent2D) {//make sure it's between min and max allowed externts
            .width = std::clamp(
                windowFrameBufferSize.width,
                capabilities.minImageExtent.width,
                capabilities.maxImageExtent.width
            ),
            .height = std::clamp(
                windowFrameBufferSize.height,
                capabilities.minImageExtent.height,
                capabilities.maxImageExtent.height
            )
        };
    }

    VkSwapchainKHR SwapChainWrapper::_createSwapChain(
        const vulkan::device::DeviceWrapper &deviceWrapper,
        const VkExtent2D &windowFrameBufferSize,
        const VkSurfaceKHR &surface,
        vulkan::QueueFamilies deviceQueueFamilies
    ) {
        if (deviceQueueFamilies.isComplete() != true) {
            throw std::runtime_error("Failed to create the device swap chain,\
 all queue families are not present");
        }

        const SwapChainSupportDetails swapChainSupports =
        deviceWrapper.getDeviceSwapChainSupports();
        const vulkan::QueueFamilies queueFamilies = 
        deviceWrapper.getDeviceQueueFamilies();

        VkSurfaceFormatKHR surfaceFormat = this->_getOptimalSurfaceFormat(
            swapChainSupports.formats
        );
        VkPresentModeKHR presentMode = this->_getOptimalPresentMode(
            swapChainSupports.presentModes
        );
        VkExtent2D extent = this->_getOptimalSwapExtent(
            swapChainSupports.capabilities,
            windowFrameBufferSize
        );

        uint32_t nImages = swapChainSupports.capabilities
        .minImageCount + 1; //Request at leat 1 more image no avoid waiting on the driver to complete internal operations before we can acquire another image to render to
        //Make sure do not exceed the max allowed number of images. 0 here is a special value meaning no maximum value
        if (
            swapChainSupports.capabilities.maxImageCount > 0 &&
            nImages > swapChainSupports.capabilities.maxImageCount
        ) {
            nImages = swapChainSupports.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface;
        createInfo.minImageCount = nImages;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1; //AMount of layers each image is composed of. Always unless 3D stereostopic 3D app
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        //If you checked your devices correctly, they should all have a value
        uint32_t queueFamilyIndices[] = {
            deviceQueueFamilies.graphicsFamily.value(),
            deviceQueueFamilies.presentFamily.value()
        };
        if (
            deviceQueueFamilies.graphicsFamily !=
            deviceQueueFamilies.presentFamily
        ) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; //Best performances
            createInfo.queueFamilyIndexCount = 0; //Optional
            createInfo.pQueueFamilyIndices = nullptr; //Optional
        }
        createInfo.preTransform = swapChainSupports.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        VkSwapchainKHR swapChain;

        if (vkCreateSwapchainKHR(
            deviceWrapper.getLogicalDevice(), &createInfo, nullptr, &swapChain
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create the logical device's\
 swap chain");
        }
        return swapChain;
    }

} // namespace vulkan::swap_chain 