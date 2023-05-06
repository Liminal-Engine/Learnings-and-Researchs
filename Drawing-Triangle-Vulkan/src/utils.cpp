#include "utils.hpp"

#include <GLFW/glfw3.h>
#include <string.h>

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

bool UTILS_isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
    VkPhysicalDeviceProperties deviceProps;
    VkPhysicalDeviceFeatures deviceFeatures;

    vkGetPhysicalDeviceProperties(device, &deviceProps);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    QueueFamilyIndices indices = UTILS_findQueueFamilies(device, surface);
    return indices.isComplete();

    
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