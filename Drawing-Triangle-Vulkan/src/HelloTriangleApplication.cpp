#include "HelloTriangleApplication.hpp"

#include <stdexcept>
#include <iostream>
#include <vector>
#include <string.h>

const std::vector<const char *> VALIDATION_LAYERS = {
    "VK_LAYER_KHRONOS_validation" //all of the useful standard validation is bundled into this layer
};

bool checkValidationLayerSupport(void) {
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
    if (enableValidationLayers && checkValidationLayerSupport() == false) {
        throw std::runtime_error("validation layers requested are not available");
    }
    this->_createInstance();
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


void HelloTriangeApplication::_mainLoop(void) {
    while (glfwWindowShouldClose(this->_window) == false) { // poll events while window has not been ordered to close by the user
        glfwPollEvents();
    }
}

void HelloTriangeApplication::_cleanUp(void) {
    vkDestroyInstance(this->_instance, nullptr); //destroy vulkan instance
    glfwDestroyWindow(this->_window); //destroy window
    glfwTerminate(); //terminate GLFW
}