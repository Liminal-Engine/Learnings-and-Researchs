
#include <vector>
#include <vulkan/vulkan.h>
#include <cstdint>
#include <optional>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    bool isComplete();
};

bool UTILS_checkValidationLayerSupport(const std::vector<const char *> layers);
bool UTILS_isDeviceSuitable(VkPhysicalDevice device);
QueueFamilyIndices UTILS_findQueueFamilies(VkPhysicalDevice device);