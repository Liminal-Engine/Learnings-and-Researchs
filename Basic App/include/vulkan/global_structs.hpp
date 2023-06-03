#ifndef VULKAN___GLOBAL_STRUCTS_H
#define VULKAN___GLOBAL_STRUCTS_H

#include <vulkan/vulkan.h>
#include <vector>
#include <optional>
#include <cstdint>

namespace vulkan {

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct QueueFamilies {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete(void) const;
    };

}


#endif // VULKAN___GLOBAL_STRUCTS_H