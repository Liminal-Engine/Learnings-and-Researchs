#include "liminalvulkanwrapper/include/_loaders/_queueHandlers/_load.hpp"

namespace vulkanhdl {
    namespace _loaders {
        namespace _queueHandlers {

            _queues::_QueueHandlers_t _load(
                const VkDevice &logicalDevice,
                const _queues::_QueueFamilies &queueFamilies
            ) {
                _queues::_QueueHandlers_t res;

                res["graphics"] = VkQueue{};
                res["presentation"] = VkQueue{};
                // Bind the logical device to the queues object
                // We are using 0 because we only create a single queue from this family (so index is 0)
                vkGetDeviceQueue(logicalDevice, queueFamilies._graphicsFamily.value(), 0, &res["graphics"]);
                vkGetDeviceQueue(logicalDevice, queueFamilies._presentFamily.value(), 0, &res["presentation"]);
                return res;
            }

        } // _queueHandlers
    } // _loaders
} // _vulkanhdl