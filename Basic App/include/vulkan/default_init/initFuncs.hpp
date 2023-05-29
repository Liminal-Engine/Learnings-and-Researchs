#pragma once

#include <cstdint> //for uint32_t usage
#include <vector>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace vulkan::default_init {

    void initInstance(
        const char *applicationName,
        const char *engineName,
        const std::vector<const char *> extensions,
        const uint32_t extensionCount,
        VkInstance *instance
    );

    void initGLFWSurface(
        VkInstance instance,
        GLFWwindow *GLFWWindow,
        VkSurfaceKHR *surface
    );

    void choosePhysicalDevice(
        VkInstance instance,
        VkPhysicalDevice *physicalDevice
    );

    void initLogicalDevice(void);

}