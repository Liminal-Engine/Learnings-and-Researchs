#pragma once

#include "window/WindowWrapper.hpp"
#include "vulkan/device/DeviceWrapper.hpp"
#include "vulkan/swap_chain/SwapChainWrapper.hpp"
#include "vulkan/pipeline/PipelineWrapper.hpp"
#include "vulkan/FrameRenderer.hpp"

#include <vector>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


#ifdef NDEBUG //if NO DEBUG
    const bool ENABLE_VALIDATION_LAYERS = false
#else //if DEBUG
    const bool ENABLE_VALIDATION_LAYERS = true;
#endif

namespace vulkan {
    //Vulkan wrapper class
    class VulkanWrapper {
        public:
            VulkanWrapper(void);
            VulkanWrapper(const window::WindowWrapper &windowWrapper);
            ~VulkanWrapper();

            void drawFrame(void) const;

            void waitForDeviceToFinish(void) const;

        private:
            static VkInstance _STATIC_createInstance(
                const char *appName,
                const char *engineName,
                const window::WindowWrapper &windowWrapper
            );
            static VkSurfaceKHR _STATIC_createSurface(
                const VkInstance &instance,
                GLFWwindow *GLFWWindow
            );
            VkInstance _instance;
            VkSurfaceKHR _surface;

            device::DeviceWrapper _deviceWrapper;
            swap_chain::SwapChainWrapper _swapChainWrapper;
            pipeline::PipelineWrapper _pipelineWrapper;
            FrameRenderer _frameRenderer;
    };

}
