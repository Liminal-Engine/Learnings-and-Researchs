#pragma once

#include <vector>
#include <vulkan/vulkan.h>


#ifdef NDEBUG //if NO DEBUG
    const bool ENABLE_VALIDATION_LAYERS = false
#else //if DEBUG
    const bool ENABLE_VALIDATION_LAYERS = true;
#endif

namespace vulkan::default_init {
    VkApplicationInfo getDefaultApplicationInfo(
        const char *applicationName,
        const char *engineName
    );

    VkInstanceCreateInfo getDefaultVkInstanceCreateInfo(
        const char * const* extensions,
        const uint32_t extensionCount,
        const VkApplicationInfo *applicationInfo
    );
}