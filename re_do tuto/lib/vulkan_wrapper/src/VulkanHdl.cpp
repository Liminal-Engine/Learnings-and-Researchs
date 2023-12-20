#include "liminalvulkanwrapper/include/VulkanHdl.hpp"

// Include loaders
#include "liminalvulkanwrapper/include/_loaders/_instance/_load.hpp"
#include "liminalvulkanwrapper/include/_loaders/_window/_load.hpp"
#include "liminalvulkanwrapper/include/_loaders/_surface/_load.hpp"
#include "liminalvulkanwrapper/include/_loaders/_physicalDevice/_pick.hpp"
#include "liminalvulkanwrapper/include/_loaders/_logicalDevice/_load.hpp"
#include "liminalvulkanwrapper/include/_loaders/_queueFamilies/_load.hpp"
#include "liminalvulkanwrapper/include/_loaders/_queueHandlers/_load.hpp"
#include "liminalvulkanwrapper/include/_loaders/_swapChain/_load.hpp"
#include "liminalvulkanwrapper/include/_loaders/_swapChainImagesHandlers/_load.hpp"
#include "liminalvulkanwrapper/include/_loaders/_swapChainImagesViewsHandlers/_load.hpp"
#include "liminalvulkanwrapper/include/_loaders/_renderPass/_load.hpp"
#include "liminalvulkanwrapper/include/_loaders/_pipelineLayout/_load.hpp"
#include "liminalvulkanwrapper/include/_loaders/_pipeline/_load.hpp"
#include "liminalvulkanwrapper/include/_loaders/_frameBuffers/_load.hpp"
#include "liminalvulkanwrapper/include/_loaders/_commandPool/_load.hpp"
#include "liminalvulkanwrapper/include/_loaders/_commandBuffer/_load.hpp"
#include "liminalvulkanwrapper/include/_loaders/_semaphore/_load.hpp"
#include "liminalvulkanwrapper/include/_loaders/_fence/_load.hpp"

//Include destroyers
#include "liminalvulkanwrapper/include/_destroyers/_logicalDevice/_destroy.hpp"
#include "liminalvulkanwrapper/include/_destroyers/_surface/_destroy.hpp"
#include "liminalvulkanwrapper/include/_destroyers/_instance/_destroy.hpp"
#include "liminalvulkanwrapper/include/_destroyers/_window/_destroy.hpp"
#include "liminalvulkanwrapper/include/_destroyers/_swapChain/_destroy.hpp"
#include "liminalvulkanwrapper/include/_destroyers/_swapChainImagesViewsHandlers/_destroy.hpp"
#include "liminalvulkanwrapper/include/_destroyers/_renderPass/_destroy.hpp"
#include "liminalvulkanwrapper/include/_destroyers/_pipelineLayout/_destroy.hpp"
#include "liminalvulkanwrapper/include/_destroyers/_pipeline/_destroy.hpp"
#include "liminalvulkanwrapper/include/_destroyers/_frameBuffers/_destroy.hpp"
#include "liminalvulkanwrapper/include/_destroyers/_commandPool/_destroy.hpp"
#include "liminalvulkanwrapper/include/_destroyers/_fence/_destroy.hpp"
#include "liminalvulkanwrapper/include/_destroyers/_semaphore/_destroy.hpp"


#include "liminalvulkanwrapper/include/_swapChain/_image.hpp"

#include <vector>

const std::vector<const char *> DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

namespace vulkanhdl {

    bool TMP_checkValidationLayerSupport() {
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
        
    VulkanHdl::VulkanHdl(void) :
    _TMP{TMP_checkValidationLayerSupport()},
    _appName{"Vulkan Best Tutorial"},
    _engineName{"Liminal Engine"},
    _window{_loaders::_window::_load("Vulkan App")},
    _instance{_loaders::_instance::_load(this->_appName, this->_engineName)},
    _surface{_loaders::_surface::_load(this->_instance, this->_window)},
    _physicalDevice{_loaders::_physicalDevice::_pick(this->_instance, this->_surface, std::vector<const char *>())},
    _swapChainSupports{_swapchain::_getSupports(this->_physicalDevice, this->_surface)},
    _queueFamilies{_loaders::_queueFamilies::_load(this->_physicalDevice, this->_surface)},
    _logicalDevice{_loaders::_logicalDevice::_load(this->_physicalDevice, this->_surface, this->_queueFamilies._toSet(), DEVICE_EXTENSIONS)},
    _queueHandlers{_loaders::_queueHandlers::_load(this->_logicalDevice, this->_queueFamilies)},
    _swapChainImageFormat{_swapchain::_getBestSurfaceFormat(this->_swapChainSupports._surfaceFormats)},
    _swapChainExtent{_swapchain::_getBestExtent(this->_window, this->_swapChainSupports._surfaceCapabilities)},
    _swapChain{_loaders::_swapchain::_load(this->_logicalDevice, this->_window, this->_surface, this->_swapChainSupports, this->_queueFamilies, this->_swapChainImageFormat, this->_swapChainExtent)},
    _swapChainImagesHandlers{_loaders::_swapChainImagesHandlers::_load(this->_logicalDevice, this->_swapChain)},
    _swapChainImagesViewsHandlers{_loaders::_swapChainImagesViewsHandlers::_load(this->_logicalDevice, this->_swapChainImagesHandlers, this->_swapChainImageFormat.format)},
    _renderPass{_loaders::_renderpass::_load(this->_logicalDevice, this->_swapChainImageFormat.format)},
    _pipelineLayout{_loaders::_pipelinelayout::_load(this->_logicalDevice)},
    _graphicsPipeline{_loaders::_pipeline::_load(this->_logicalDevice, this->_swapChainExtent, this->_pipelineLayout, this->_renderPass)},
    _frameBuffers{_loaders::_framebuffers::_load(this->_logicalDevice, this->_swapChainImagesViewsHandlers, this->_swapChainExtent, this->_renderPass)},
    _commandPool{_loaders::_commandpool::_load(this->_logicalDevice, this->_queueFamilies)},
    _commandBuffer{_loaders::_commandbuffer::_load(this->_logicalDevice, this->_commandPool)},
    _imageAvailableSemaphore{_loaders::_semaphore::_load(this->_logicalDevice)},
    _renderFinishedSemaphore{_loaders::_semaphore::_load(this->_logicalDevice)},
    _inFlightFence{_loaders::_fence::_load(this->_logicalDevice)}
    {

    }
    
    VulkanHdl::~VulkanHdl() {
        _destroyers::_semaphore::_destroy(this->_logicalDevice, this->_imageAvailableSemaphore, nullptr);
        _destroyers::_semaphore::_destroy(this->_logicalDevice, this->_renderFinishedSemaphore, nullptr);
        _destroyers::_fence::_destroy(this->_logicalDevice, this->_inFlightFence, nullptr);
        _destroyers::_commandpool::_destroy(this->_logicalDevice, this->_commandPool, nullptr);
        _destroyers::_framebuffers::_destroy(this->_logicalDevice, this->_frameBuffers, nullptr);
        _destroyers::_pipeline::_destroy(this->_logicalDevice, this->_graphicsPipeline, nullptr);
        _destroyers::_pipelineLayout::_destroy(this->_logicalDevice, this->_pipelineLayout, nullptr);
        _destroyers::_renderpass::_destroy(this->_logicalDevice, this->_renderPass, nullptr);
        _destroyers::_swapchainimagesviewshandlers::_destroy(this->_logicalDevice, this->_swapChainImagesViewsHandlers, nullptr);
        _destroyers::_swapchain::_destroy(this->_logicalDevice, this->_swapChain, nullptr);
        _destroyers::_logicaldevice::_destroy(this->_logicalDevice, nullptr);
        _destroyers::_surface::_destroy(this->_instance, this->_surface, nullptr);
        _destroyers::_instance::_destroy(this->_instance, nullptr);
        _destroyers::_window::_destroy(this->_window);
    }
}