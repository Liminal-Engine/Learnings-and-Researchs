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
    )},
    _images{this->_createSwapChainImages(deviceWrapper.getLogicalDevice())},
    _imageViews{this->_createImageViews(deviceWrapper.getLogicalDevice())}
    {
        
    }
    
    void SwapChainWrapper::cleanUp(const VkDevice &logicalDevice) {
        for (const VkImageView &imageView : this->_imageViews) {
            vkDestroyImageView(logicalDevice, imageView, nullptr);
        }
        vkDestroySwapchainKHR(logicalDevice, this->_swapChain, nullptr);
    }

    VkExtent2D SwapChainWrapper::getExtent(void) const {return this->_extent;}

    VkFormat SwapChainWrapper::getImageFormat(void) const {
        return this->_imageFormat;
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
        return (VkExtent2D) {//make sure it's between min and max allowed extents
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
        this->_imageFormat = surfaceFormat.format;
        VkPresentModeKHR presentMode = this->_getOptimalPresentMode(
            swapChainSupports.presentModes
        );
        this->_extent = this->_getOptimalSwapExtent(
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
        createInfo.imageExtent = this->_extent;
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

    std::vector<VkImage> SwapChainWrapper::_createSwapChainImages(
        const VkDevice &logicalDevice
    ) {

        uint32_t nImages = 0;

        vkGetSwapchainImagesKHR(
            logicalDevice,
            this->_swapChain,
            &nImages,
            nullptr
        );
        std::vector<VkImage> images(nImages);
        vkGetSwapchainImagesKHR(
            logicalDevice,
            this->_swapChain,
            &nImages,
            images.data()
        );
        return images;
    }

    std::vector<VkImageView> SwapChainWrapper::_createImageViews(
        const VkDevice &logicalDevice
    ) {
        std::vector<VkImageView> imageViews(this->_images.size());
        // this->_imageViews.resize(this->_images.size());

        for (size_t i = 0; i < this->_imageViews.size(); i++) {
            VkImageViewCreateInfo createInfo{};

            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = this->_images[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = this->_imageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(
                logicalDevice, &createInfo, nullptr, &imageViews[i]
            ) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create an image view");
            }
        }
        return imageViews;
    }

} // namespace vulkan::swap_chain
