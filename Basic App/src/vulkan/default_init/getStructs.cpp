#include "vulkan/default_init/getStructs.hpp"

#include <iostream>

namespace vulkan::default_init {
    VkApplicationInfo getDefaultApplicationInfo(
        const char *applicationName,
        const char *engineName
    ) {
        VkApplicationInfo appInfo{};

        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = applicationName;
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = engineName;
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;
        appInfo.pNext = nullptr;

        return appInfo;
    }

    VkInstanceCreateInfo getDefaultVkInstanceCreateInfo(
        const char * const* extensions,
        const uint32_t extensionCount,
        const VkApplicationInfo *applicationInfo
    ) {
        VkInstanceCreateInfo instanceCreateInfo{};

        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = applicationInfo;
        instanceCreateInfo.enabledExtensionCount = extensionCount;
        instanceCreateInfo.ppEnabledExtensionNames = extensions;
        if (ENABLE_VALIDATION_LAYERS == true) {
            instanceCreateInfo.enabledLayerCount = 1;
            instanceCreateInfo.ppEnabledLayerNames = (const char * const []) {"VK_LAYER_KHRONOS_validation"};
        }
        return instanceCreateInfo;
    }
}