
#include <vector>
#include <vulkan/vulkan.h>
#include <cstdint>
#include <optional>
#include <string>
#include <GLFW/glfw3.h>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    bool isComplete();
};

struct SwapChainSupports {
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    std::vector<VkSurfaceFormatKHR> surfaceFormats;
    std::vector<VkPresentModeKHR> presentModes;
};

bool UTILS_checkValidationLayerSupport(const std::vector<const char *> layers);
//Check support for a given extension
bool UTILS_checkDeviceExtensionsSupport(
    const VkPhysicalDevice physicalDevice,
    const std::vector<const char *> requiredExtensions
);
bool UTILS_isDeviceSuitable(
    const VkPhysicalDevice device,
    const VkSurfaceKHR surface,
    const std::vector<const char *> extensions
);
QueueFamilyIndices UTILS_findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
SwapChainSupports UTILS_querySwapChainSupport(
    VkPhysicalDevice physicalDevice,
    VkSurfaceKHR surface
);
VkSurfaceFormatKHR UTILS_chooseSwapChainSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR> &availableFormats
);
VkPresentModeKHR UTILS_chooseSwapChainPresentMode(
    const std::vector<VkPresentModeKHR> &availablePresentModes
);
VkExtent2D UTILS_chooseSwapChainExtent(
    GLFWwindow *window,
    const VkSurfaceCapabilitiesKHR &capabilities
);