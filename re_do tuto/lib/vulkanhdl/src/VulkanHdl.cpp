#include "vulkanhdl/include/VulkanHdl.hpp"

// Include loaders
#include "vulkanhdl/include/_loaders/_instance/_load.hpp"
#include "vulkanhdl/include/_loaders/_window/_load.hpp"
#include "vulkanhdl/include/_loaders/_surface/_load.hpp"
#include "vulkanhdl/include/_loaders/_physicalDevice/_pick.hpp"
#include "vulkanhdl/include/_loaders/_logicalDevice/_load.hpp"
#include "vulkanhdl/include/_loaders/_queueFamilies/_load.hpp"
#include "vulkanhdl/include/_loaders/_queueHandlers/_load.hpp"
#include "vulkanhdl/include/_loaders/_swapChain/_load.hpp"
#include "vulkanhdl/include/_loaders/_swapChainImagesHandlers/_load.hpp"
#include "vulkanhdl/include/_loaders/_swapChainImagesViewsHandlers/_load.hpp"

#include "vulkanhdl/include/_swapChain/_image.hpp"

#include <vector>

const std::vector<const char *> DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

namespace vulkanhdl {
        
    VulkanHdl::VulkanHdl(void) :
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
    _swapChainImagesViewsHandlers{_loaders::_swapChainImagesViewsHandlers::_load(this->_logicalDevice, this->_swapChainImagesHandlers, this->_swapChainImageFormat.format)}
    {

    }
    
}