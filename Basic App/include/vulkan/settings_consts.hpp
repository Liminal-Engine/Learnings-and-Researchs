#include <vector>
#include <vulkan/vulkan.h>

#ifdef NDEBUG //if NO DEBUG
    
    const bool ENABLE_VALIDATION_LAYERS = false

#else //if DEBUG
    #include <stdint.h>

    const bool ENABLE_VALIDATION_LAYERS = true;
    const uint32_t LAYER_COUNT = 1;
    const char *const LAYER_NAMES[] = {
        "VK_LAYER_KHRONOS_validation"
    };

#endif

const std::vector<const char *> REQUIRED_DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
