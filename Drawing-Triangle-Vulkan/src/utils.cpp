#include "utils.hpp"

#include <GLFW/glfw3.h>
#include <string.h>
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>
#include <limits>

bool QueueFamilyIndices::isComplete(void) {
    return this->graphicsFamily.has_value() && this->presentFamily.has_value();
}

bool UTILS_checkValidationLayerSupport(const std::vector<const char *> VALIDATION_LAYERS) {
    //1. List avialable validation layers
    uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    for (const char *layerName : VALIDATION_LAYERS) { //2. loop through wanted layers
        bool found = false;
        for (const auto &layerProperties : availableLayers) {//3. loop through available layers
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                found = true;
                break;
            }
        }
        if (found == false) {
            return false;
        }
    }
    return true;
}

bool UTILS_isDeviceSuitable(
    const VkPhysicalDevice device,
    const VkSurfaceKHR surface,
    const std::vector<const char *> requiredExtensions
) {
    VkPhysicalDeviceProperties deviceProps;
    VkPhysicalDeviceFeatures deviceFeatures;

    vkGetPhysicalDeviceProperties(device, &deviceProps);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    QueueFamilyIndices indices = UTILS_findQueueFamilies(device, surface);

    const bool extensionsSupported = UTILS_checkDeviceExtensionsSupport(device, requiredExtensions);
    bool swapChainAdequate = false;

    if (extensionsSupported == true) {
        SwapChainSupports SWSupport = UTILS_querySwapChainSupport(device, surface);
        //for the tutorial, we only required at least on format and one present mode
        swapChainAdequate = SWSupport.surfaceFormats.empty() == false && SWSupport.presentModes.empty() == false;
    }
    return indices.isComplete() && extensionsSupported && swapChainAdequate;
    //!!!!KEEP THIS : TO FILTER GPUs
    // return (
    //     deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
    //     deviceFeatures.geometryShader
    // );
}

QueueFamilyIndices UTILS_findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
    QueueFamilyIndices indices;
    //1. get Queue Families
    uint32_t queueFamilyCount = 0;
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
    //2. We must find at least one queue family that supports VK_QUEUE_GRAPHICS_BIT
    for (int i = 0; i < queueFamilies.size(); i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;            
        }
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (presentSupport) {
            indices.presentFamily = i;
        }
    }
    return indices;
}

bool UTILS_checkDeviceExtensionsSupport(
    const VkPhysicalDevice physicalDevice,
    const std::vector<const char *> extensions
) {
    //1. List available extension
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());


    bool found = false;
    for (const char *extension : extensions) {
        found = false;
        for (const auto &availableExtension : availableExtensions) {
            if (strcmp(availableExtension.extensionName, extension) == 0) {
                found = true;
            }
        }
        if (found == false)
            return false;
    }
    // for (const auto &availableExtension : availableExtensions) {
    //     std::cout << "avilable extension" << availableExtension.extensionName << std::endl;
    //     if (std::find(extensions.begin(), extensions.end(), availableExtension.extensionName) == extensions.end()) {
    //         return false;
    //     }
    // }
    return true;
    // //2. Filter available extensions from required extensions
    // std::set requiredExtensions(extensions.begin(), extensions.end());
    // for (const auto &availableExtension : availableExtensions) {
    //     requiredExtensions.erase(availableExtension.extensionName);
    // }
    // //3. return the emptyness of required extensions
    // return requiredExtensions.empty();
}

SwapChainSupports UTILS_querySwapChainSupport(
    VkPhysicalDevice physicalDevice,
    VkSurfaceKHR surface
) {
    SwapChainSupports SWSupports{};

    //1. Populates SWSupports.surfaceCapabilities :
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &SWSupports.surfaceCapabilities);

    //2. Populates SWSupports.surfaceFormats, remember that is is a vector of VkSurfaceFormatKHR,
    // so we must follow the familiar 2 function calls process :
    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    if (formatCount > 0) {
        SWSupports.surfaceFormats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, SWSupports.surfaceFormats.data());
    }
    //3. Populates SWSupports.presentModes (= get the supported presentation modes), works exactly
    // the same as for supported formats
    uint32_t presentModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
    if (presentModeCount > 0) {
        SWSupports.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, SWSupports.presentModes.data());
    }
    return SWSupports;
}

VkSurfaceFormatKHR UTILS_chooseSwapChainSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR> &availableFormats
) {
    /*each entry contains :
    - field "format" : specifies the color channel. ex : VK_FORMAT_B8G8R8A8_SRGB : B,G,R and alpha
    in that order each on 8 bits unsigned int (total = 32 bits per pixel)
    - field "colorSpace": indicates if the sRGB color space is supported or not using the
    VK_COLOR_SPACE_SRGB_NONLINEAR_KHR flag. We will use sRGB if available because it results 
    in more accurate perceived color : http://stackoverflow.com/questions/12524623/ and it is
    also pretty much the standard color space for images. Because of this, we should also use
    an sRGB color format, of which one of the most common ones is VK_FORMAT_B8G8R8A8_SRGB
    */
    //loop through the avaialbe format and see if the preferred combination is available :
    for (const auto &availableFormat : availableFormats) {
        if (
            availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
        ) {
            return availableFormat;
        }
    }
    //else, we can rank the formats based on how "good" they are, in this tutorial,
    //we will only return the first one
    return availableFormats.at(0);
}

VkPresentModeKHR UTILS_chooseSwapChainPresentMode(
    const std::vector<VkPresentModeKHR> &availablePresentModes
) {
    /*
    The presentation mode is aruably the MOST IMPORTANT SETTING for the swap chain, because
    it represents the actual conditions for showing images to the screen. In Vulkan, 4 modes : 
    1. VK_PRESENT_MODE_IMMEDIATE_KHR: Imageas submitted by your application are transferred to
    the screen right away, which may result in tearing.
    2. VK_PRESENT_MDOE_FIFO_KHR: FIFO = First In, First Out. : The swap chain is a queue where
    the display takes an image from the front of the queue when the display is refreshed and
    the program inserts rendred images at the back of the queue. If the queue is full then the
    program has to wait. Most similar to V-Sync. The moment when the display is refreshed is called
    "vertical blank"
    3. VK_PRESENT_MODE_FIFO_RELAXED_KHR: This mode differs only from the previous one if the application
    is late and the queue was empty at the last vertical blank. Instead of waiting for the next vertical
    blank, the image is transferred right away when it finally arrives. This may result in visible
    tearing
    4. VK_PRESENT_MODE_MAILBOX_KHR: This is another variation of the second mode VK_PRESENT_MDOE_FIFO_KHR.
    Instead of blocking the app when queue is full, images that are already queues are replaced with the
    newer ones. This mode can be used to render frames as fast as possible while still avoiding tearinng,
    resulting in fewer latency issues than sstandard vertuical sync. This is commonly known as "triple 
    buffering" , although the existence of 3 buffers alone does not necessarily mean that the framerate
    is unblocked.
    */
   /*
    The author thing that mailbox is the best mode because offer nice treade-off if energy usage is not
    a concern because it allows to avoid tearing while maintaining a fairly low latency by rendering new images
    that are as up-to-date as possible right until the vertical blank., On mobile devices, we may want to use 
    FIFO_KHR cause energy usage is more important to take into consideration.
   */
    for (const auto &availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D UTILS_chooseSwapChainExtent(
    GLFWwindow *window,
    const VkSurfaceCapabilitiesKHR &capabilities
) {
    /*
        Swap extent = resolution of the images in the swap chain. Almost always exact to the resolution of
        the window we are drawing in to in PIXELS.
        The range of possible resolutions is defined in the VkSurfaceCapabilitiesKHR struct. Vulkan tells us to
        match the resolution of the window by setting the width and height in the currentExtent member. However,
        some window managers do allow us to differ here by setting the width and height in the currentExtent member
        to a special value : the max value of a uint32_t. In this case, it will pick the best resolution that 
        match the window within minImageExtent and maxImageExtent bounds. But to do this we must specify the
        resolution in the correct unit.
        GLFW uses 2 units when measuring sizes : pixels and screen coordinates (https://www.glfw.org/docs/latest/intro_guide.html#coordinate_systems)
        The width and height we specified when creating the GLFW Windows are in screen coodrinates. But Vulkan
        works with pixels only so the swap chain extent must be specified in pixel as well.
        With GLFW we can use glfwGetFrameBufferSize to query the resolution of the window in pixel before matching
        it against the min and max image swap extent.
    */
    if (capabilities.currentExtent.width !=  std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width = 0;
        int height = 0;
        VkExtent2D actualExtent{};

        //1. Get the width and height;
        glfwGetFramebufferSize(window, &width, &height);

        //2. Set values and cast to uint32_t
        actualExtent.width = static_cast<uint32_t>(width);
        actualExtent.height = static_cast<uint32_t>(width);
        
        //3. Make sur width and height are contained in min and max posssible values,
        //otherwise, set to min if < or max if >
        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        return actualExtent;        
    }
}













// void pickPhysicalDevice() {
//     ...

//     // Use an ordered map to automatically sort candidates by increasing score
//     std::multimap<int, VkPhysicalDevice> candidates;

//     for (const auto& device : devices) {
//         int score = rateDeviceSuitability(device);
//         candidates.insert(std::make_pair(score, device));
//     }

//     // Check if the best candidate is suitable at all
//     if (candidates.rbegin()->first > 0) {
//         physicalDevice = candidates.rbegin()->second;
//     } else {
//         throw std::runtime_error("failed to find a suitable GPU!");
//     }
// }

// int rateDeviceSuitability(VkPhysicalDevice device) {
//     ...

//     int score = 0;

//     // Discrete GPUs have a significant performance advantage
//     if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
//         score += 1000;
//     }

//     // Maximum possible size of textures affects graphics quality
//     score += deviceProperties.limits.maxImageDimension2D;

//     // Application can't function without geometry shaders
//     if (!deviceFeatures.geometryShader) {
//         return 0;
//     }

//     return score;
// }