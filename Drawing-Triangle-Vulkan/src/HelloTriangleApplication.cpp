#include "HelloTriangleApplication.hpp"
#include "utils.hpp"

#include <stdexcept>
#include <iostream>
#include <vector>
#include <string.h>

const std::vector<const char *> VALIDATION_LAYERS = {
    "VK_LAYER_KHRONOS_validation" //all of the useful standard validation is bundled into this layer
};

HelloTriangeApplication::HelloTriangeApplication(void)
:
_physicalDevice(VK_NULL_HANDLE),
_logicalDevice(VK_NULL_HANDLE)
{}


void HelloTriangeApplication::run(void) {
    this->_initWindow();
    this->_initVulkan();
    this->_mainLoop();
    this->_cleanUp();
}

void HelloTriangeApplication::_initWindow(void) {
    glfwInit(); //init GLFW lib

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // explicilty tells GLFW to NOT use OpenGL context
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //disable resize because we will need to handle it later

    this->_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr); //4th parameter : choosing the monitor, 5th : specific to OpenGL

}

void HelloTriangeApplication::_initVulkan(void) {
    if (enableValidationLayers && UTILS_checkValidationLayerSupport(VALIDATION_LAYERS) == false) {
        throw std::runtime_error("validation layers requested are not available");
    }
    this->_createInstance();
    this->_pickPhysicalDevice();
    this->_createLogicalDevice();
}

void HelloTriangeApplication::_createInstance(void) {
    VkApplicationInfo appInfo{};

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Liminal";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    appInfo.pNext = nullptr;

    VkInstanceCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    uint32_t glfwExtensionCount = 0;
    //glfw has a method to get the required extensions
    //returns extensions and modify count since we pass a pointer :
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    if (enableValidationLayers == true) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&createInfo, nullptr, &this->_instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vulkan instance");
    }
    //Get the available extensions
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    std::cout << "List of available extensions :" << std::endl;
    for (const auto &extension : extensions) {
        std::cout << "\t" << extension.extensionName << std::endl;
    }
    std::cout << "List of required extensions :" << std::endl;
    for (int i = 0; i < glfwExtensionCount; i++) {
        std::cout << "\t" <<glfwExtensions[i] << std::endl;
    }
}

void HelloTriangeApplication::_pickPhysicalDevice(void) {
    //1. Enumerate compatible devices and allocate a vector to store them
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(this->_instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support");
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(this->_instance, &deviceCount, devices.data());
    //2. Check if at least one of the compatible devices meets the requirements :
    for (const auto &device : devices) {
        if (UTILS_isDeviceSuitable(device)) {
            this->_physicalDevice = device;
            break;
        }
    }
    //3. Throw error if no devices was found
    if (this->_physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("Failed to find a suitable GPU");
    }
}

void HelloTriangeApplication::_createLogicalDevice(void) {
    QueueFamilyIndices indices = UTILS_findQueueFamilies(this->_physicalDevice);

    //1. Specifying which queues to be created
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value(); //we tell the logical device which queue family to use
    queueCreateInfo.queueCount = 1; //we tell the nb of queues we want to use for a single queue family
    float queuePriority = 1.0f; //scheduling command buffer execution
    queueCreateInfo.pQueuePriorities = &queuePriority;

    //2. Specifying features we want to enable for the device
    VkPhysicalDeviceFeatures deviceFeatures{}; //Leave blank for now as we don't anything special

    //3. Creating the logical device;
    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    deviceCreateInfo.enabledExtensionCount = 0;
    deviceCreateInfo.pNext = nullptr;
    //similar to instance creation :
    if (enableValidationLayers) { //if debug mode
        deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        deviceCreateInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    } else {
        deviceCreateInfo.enabledLayerCount = 0;
    }
    //4. Instanciate logical device by binding it to the physical device and store it
    if (vkCreateDevice(this->_physicalDevice, &deviceCreateInfo, nullptr, &this->_logicalDevice) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device");
    }
    //5. Bind our graphics queue handle to the logical device
    //We can use vkGetDeviceQueue to retrieve queue handles for each queue family.
    //We are using 0 because we only creating a single queue from this family (so index is 0)
    vkGetDeviceQueue(this->_logicalDevice, indices.graphicsFamily.value(), 0, &this->_graphicsQueue);

        
}

void HelloTriangeApplication::_mainLoop(void) {
    while (glfwWindowShouldClose(this->_window) == false) { // poll events while window has not been ordered to close by the user
        glfwPollEvents();
    }
}

void HelloTriangeApplication::_cleanUp(void) {
    vkDestroyDevice(this->_logicalDevice, nullptr);
    vkDestroyInstance(this->_instance, nullptr); //destroy vulkan instance    
    glfwDestroyWindow(this->_window); //destroy window
    glfwTerminate(); //terminate GLFW
}