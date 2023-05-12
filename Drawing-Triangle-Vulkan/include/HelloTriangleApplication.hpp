#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vector>
#include <string>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

extern const std::vector<const char *> VALIDATION_LAYERS;
extern const std::vector<const char *> DEVICE_EXTENSIONS;


#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

class HelloTriangleApplication {

    public:
        void run(void);
        HelloTriangleApplication(void);

    private:
        void _initWindow(void);

        void _initVulkan(void);
        //called from _initVulkan :
            void _createInstance(void);
            void _createSurface(void);
            void _pickPhysicalDevice(void);
            void _createLogicalDevice(void);
            void _createSwapChain(void);
            void _createImageViews(void);
            void _createGraphicsPipeline(void);


        void _mainLoop(void);
        void _cleanUp(void);

        GLFWwindow *_window;
        VkInstance _instance;
        VkSurfaceKHR _surface;
        // _physicalDevice will be implicitly destroyed when _instance is destroyed, so don't need to destroy it in the destructor
        VkPhysicalDevice _physicalDevice; //default value = VK_NULL_HANDLE
        VkDevice _logicalDevice; //default value = VK_NULL_HANDLE
        // _graphicsQueue will be implicitly destroyed when _logicalDevice is destroyed, so don't need to destroy it in the destructor
        VkQueue _graphicsQueue; //Graphics queue handle = "drawing"
        VkQueue _presentQueue; //Presentation queue command = "rendering final image, after having draw everything"

        VkSwapchainKHR _swapChain;
        std::vector<VkImage> _swapChainImages;
        VkFormat _swapChainImageFormat;
        VkExtent2D _swapChainExtent;
        std::vector<VkImageView> _swapChainImageViews;
};