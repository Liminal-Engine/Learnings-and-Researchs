#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vector>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

extern const std::vector<const char *> VALIDATION_LAYERS;

bool checkValidationLayerSupport(void);

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

class HelloTriangeApplication {

    public:
        void run(void);

    private:
        void _initWindow(void);

        void _initVulkan(void);
        void _createInstance(void);

        void _mainLoop(void);
        void _cleanUp(void);

        GLFWwindow *_window;
        VkInstance _instance;
};