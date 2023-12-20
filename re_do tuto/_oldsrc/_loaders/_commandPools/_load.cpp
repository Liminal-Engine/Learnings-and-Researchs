#include "liminalvulkanwrapper/include/_loaders/_commandPool/_load.hpp"

#include <stdexcept>

namespace vulkanhdl {
    namespace _loaders {
        namespace _commandpool {

            VkCommandPool _load(
                const VkDevice &logicalDevice,
                const _queues::_QueueFamilies &queueFamilies
            ) {
                VkCommandPool res{};
                VkCommandPoolCreateInfo createInfo{};

                createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
                createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
                createInfo.queueFamilyIndex = queueFamilies._graphicsFamily.value();
                if (vkCreateCommandPool(logicalDevice, &createInfo, nullptr, &res) != VK_SUCCESS) {
                    throw std::runtime_error("Failed to create command pool");
                }
                return res;
            }

        } // _commandpool
    } // vulkanhdl
} // _loaders
