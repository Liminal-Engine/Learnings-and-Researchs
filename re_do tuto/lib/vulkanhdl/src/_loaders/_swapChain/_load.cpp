#include "vulkanhdl/include/_loaders/_swapChain/_load.hpp"

#include <stdexcept>

namespace vulkanhdl {
    namespace _loaders {
        namespace _swapchain {

            VkSwapchainKHR _load(
                const VkPhysicalDevice &physicalDevice,
                const VkSurfaceKHR &surface,
                const vulkanhdl::_swapchain::_SwapChainSupports &swapchainSupports,
                const _queues::_QueueFamilies &queueFamilies
            ) {
                
            }

        } // _swapchain
    } // _loaders
} // vulkanhdl
