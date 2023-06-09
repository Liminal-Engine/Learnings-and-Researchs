
#include <vector>
#include <stdexcept>

#include "vulkan/VulkanWrapper.hpp"
#include <GLFW/glfw3.h>

namespace vulkan {

    VulkanWrapper::VulkanWrapper(void)
    :
    _instance(VK_NULL_HANDLE),
    _surface(VK_NULL_HANDLE),
    _deviceWrapper(),
    _pipelineWrapper(),
    _frameRenderer()
    {
    }

    VulkanWrapper::VulkanWrapper(const window::WindowWrapper &windowWrapper)
    :
    _instance{this->_STATIC_createInstance(
        "Basic App",
        "Liminal",
        windowWrapper
    )},
    _surface{this->_STATIC_createSurface(
        this->_instance,
        windowWrapper.getGLFWWindow())
    },
    _deviceWrapper{device::DeviceWrapper(this->_instance, this->_surface)},
    _swapChainWrapper{swap_chain::SwapChainWrapper(
        this->_deviceWrapper,
        windowWrapper.getFrameBufferSize(),
        this->_surface,
        this->_deviceWrapper.getDeviceQueueFamilies()
    )},
    _pipelineWrapper{pipeline::PipelineWrapper(
        this->_deviceWrapper.getLogicalDevice(),
        "./shaders/shader.vert.spv",
        "./shaders/shader.frag.spv",
        this->_swapChainWrapper
    )},
    _frameRenderer{FrameRenderer(
        this->_deviceWrapper,
        this->_swapChainWrapper,
        this->_pipelineWrapper.getRenderPass()
    )}
    {
        //We MUST set the framebuffers before drawing anything
        // //1. Init Vulkan instance
        //     //1.1 Get the required extensions
        // const std::vector<const char *> GLFWExtensions = windowWrapper.getRequiredVulkanInstanceExtensions();
        //     //1.2 Call the instance init func with the required extensions
        // default_init::initInstance("Basic App", "Liminal", GLFWExtensions, GLFWExtensions.size(), &this->_instance);
        // //2. Create GLFW window surface
        // default_init::initGLFWSurface(this->_instance, windowWrapper.getGLFWWindow(), &this->_surface);
        // //3. Choose a compatible physical device
        // default_init::choosePhysicalDevice(this->_instance, &this->_physicalDevice);
    }

    VulkanWrapper::~VulkanWrapper(void) {
        this->_frameRenderer.cleanUp(this->_deviceWrapper.getLogicalDevice());
        this->_pipelineWrapper.cleanUp(this->_deviceWrapper.getLogicalDevice());
        this->_swapChainWrapper.cleanUp(this->_deviceWrapper.getLogicalDevice());
        this->_deviceWrapper.cleanUp();
        vkDestroySurfaceKHR(this->_instance, this->_surface, nullptr);
        vkDestroyInstance(this->_instance, nullptr);
    }

    void VulkanWrapper::drawFrame(void) const {
        this->_frameRenderer.drawFrame(
            this->_deviceWrapper,
            this->_pipelineWrapper,
            this->_swapChainWrapper
        );
    }

    void VulkanWrapper::waitForDeviceToFinish(void) const {
        vkDeviceWaitIdle(this->_deviceWrapper.getLogicalDevice());
    }

    // void VulkanWrapper::cleanUp(void) {
        
    // }

    // Private
    //Static method
    VkInstance VulkanWrapper::_STATIC_createInstance(
        const char *appName,
        const char *engineName,
        const window::WindowWrapper &windowWrapper
    ) {
        
        const std::vector<const char *> extensions =
        windowWrapper.getRequiredVulkanInstanceExtensions();
        VkInstance instance;

        VkApplicationInfo appInfo = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = appName,
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = engineName,
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = VK_API_VERSION_1_0,
        };
        VkInstanceCreateInfo instanceCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .pApplicationInfo = &appInfo,
            .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data()
        };
        if (ENABLE_VALIDATION_LAYERS == true) {
            instanceCreateInfo.enabledLayerCount = 1;
            instanceCreateInfo.ppEnabledLayerNames =
            (const char * const []) {"VK_LAYER_KHRONOS_validation"};
        }
        if ( vkCreateInstance(
            &instanceCreateInfo,
            nullptr,
            &instance
        ) != VK_SUCCESS ) {
            throw std::runtime_error("Failed to create Vulkan instance.");
        }
        return instance;
    }

    //Static method
    VkSurfaceKHR VulkanWrapper::_STATIC_createSurface(
        const VkInstance &instance,
        GLFWwindow *GLFWWindow
    ) {
        VkSurfaceKHR surface;

        if (glfwCreateWindowSurface(
            instance,
            GLFWWindow,
            nullptr,
            &surface
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create window surface");
        }
        return surface;
    }
}
