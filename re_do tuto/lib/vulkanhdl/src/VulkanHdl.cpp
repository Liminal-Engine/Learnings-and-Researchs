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
    _swapChainSupports{_swapchain::_getSwapChainSupports(this->_physicalDevice, this->_surface)},
    _queueFamilies{_loaders::_queueFamilies::_load(this->_physicalDevice, this->_surface)},
    _logicalDevice{_loaders::_logicalDevice::_load(this->_physicalDevice, this->_surface, this->_queueFamilies._toSet(), DEVICE_EXTENSIONS)},
    _queueHandlers{_loaders::_queueHandlers::_load(this->_logicalDevice, this->_queueFamilies)},
    _swapChain{_loaders::_swapchain::_load(this->_physicalDevice, this->_surface, this->_swapChainSupports, this->_queueFamilies)}
    {

    }
    
}