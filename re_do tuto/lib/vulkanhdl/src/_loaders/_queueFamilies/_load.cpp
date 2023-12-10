#include "vulkanhdl/include/_queues/_queues.hpp"

#include <vector>
#include <stdexcept>

namespace vulkanhdl {
    namespace _loaders {
        namespace _queueFamilies {

            _queues::_QueueFamilies _load(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface) {
                _queues::_QueueFamilies queueFamilies = {};
                VkBool32 presentationSupport = false;

                // 1. Get the physical device queue family properties count
                uint32_t nQueueFamily = 0;
                vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &nQueueFamily, nullptr);
                if (nQueueFamily == 0) {
                    throw std::runtime_error("No queue family for the given device");
                }
                // 2. Allocate vector and fill it up
                std::vector<VkQueueFamilyProperties> queueFamilyProperties(nQueueFamily);
                vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &nQueueFamily, queueFamilyProperties.data());

                // 3. Loop through the physical device found queue families and get the indices in the _QueueFamilies struct
                for (_queues::_QueueFamilyIndex_t i = 0; i < static_cast<_queues::_QueueFamilyIndex_t>(queueFamilyProperties.size()); i++) {
                    if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                        queueFamilies._graphicsFamily = i;
                    }
                    // 3.1 Check if this queue family supports presentation
                    vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentationSupport);
                    if (presentationSupport == true) {
                        queueFamilies._presentFamily = i;
                    }
                }
                return queueFamilies;
            }

        } // _queueFamilies
    } // _loaders
} // vulkanhdl
