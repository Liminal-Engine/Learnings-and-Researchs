#include "vulkan/default_init/initFuncs.hpp"
#include "vulkan/default_init/getStructs.hpp"

#include <stdexcept>
#include <optional>

namespace vulkan::default_init {

    static bool _isDeviceSuitable(VkPhysicalDevice physicalDevice) {
        //1. Check if the device supports graphics commands
            //1.1 Create a structure that holds the queue families we will need. Queue families are represented with uint32_t by Vulkan
        struct QueueFamilyIndices {
            std::optional<uint32_t> graphicsFamily;
        };

        QueueFamilyIndices queueFamilyIndices;
            //1.2. Call vkGetPhysicalDeviceQueueFamilyProperties() to hold a vector of available queue families
        uint32_t nQueueFamily = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &nQueueFamily, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(nQueueFamily);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &nQueueFamily, queueFamilies.data());
            //1.3. Loop through this found queue families and hold the one that match our requirements
        for (uint32_t i = 0; i < queueFamilies.size(); i++) {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {//Flag to check if it supports graphics commands
                queueFamilyIndices.graphicsFamily = i;
                break;
            }
        }

        return queueFamilyIndices.graphicsFamily.has_value();
    }

    void initInstance(
        const char *applicationName,
        const char *engineName,
        const std::vector<const char *> extensions,
        const uint32_t extensionCount,
        VkInstance *instance
    ) {
        VkApplicationInfo appInfo = getDefaultApplicationInfo(applicationName, engineName);
        VkInstanceCreateInfo instanceCreateInfo = getDefaultVkInstanceCreateInfo(
            extensions.data(),
            extensionCount,
            &appInfo
        );

        if ( vkCreateInstance(&instanceCreateInfo, nullptr, instance) != VK_SUCCESS ) {
            throw std::runtime_error("Failed to create Vulkan instance.");
        }
    }

    void initGLFWSurface(VkInstance instance, GLFWwindow *GLFWWindow, VkSurfaceKHR *surface) {
        if ( glfwCreateWindowSurface(instance, GLFWWindow, nullptr, surface) != VK_SUCCESS ) {
            throw std::runtime_error("Failed to create window surface");
        }
    }

    void choosePhysicalDevice(VkInstance instance, VkPhysicalDevice *physicalDevice) {
        //1. Use the enumerate physical devices function to update our physical device count
        uint32_t nDevice = 0;
        vkEnumeratePhysicalDevices(instance, &nDevice, nullptr);
        if (nDevice == 0) {
            throw std::runtime_error("Vulkan failed to find any GPU");
        }
        //2. Create our device vector and fill it up
        std::vector<VkPhysicalDevice> availableDevices(nDevice);
        vkEnumeratePhysicalDevices(instance, &nDevice, availableDevices.data());
        //3. Loop through all available devices and check if at least one is compatible;
        for (const VkPhysicalDevice &device : availableDevices) {
            if (_isDeviceSuitable(device) == true) { //For now, we choose the first available device
                (*physicalDevice) = device;
                return;
            }
        }
        //4. If no compatible GPU found, throw error
        throw std::runtime_error("Failed to find any compatible GPU");
    }

    void initLogicalDevice(void) {

    }

}