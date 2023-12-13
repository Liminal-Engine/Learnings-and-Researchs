#include "vulkanhdl/include/_loaders/_commandBuffer/_load.hpp"

#include <vulkan/vulkan.h>
#include <stdexcept>

namespace vulkanhdl {
    namespace _loaders {
        namespace _commandbuffer {

          
            VkCommandBuffer _load(
                const VkDevice &logicalDevice,
                const VkCommandPool &commandPool
            ) {
                VkCommandBuffer res{};
                VkCommandBufferAllocateInfo commandBufferAllocInfo{};
                commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
                commandBufferAllocInfo.commandPool = commandPool;
                commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
                commandBufferAllocInfo.commandBufferCount = 1;
                if (vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocInfo, &res) != VK_SUCCESS) {
                    throw std::runtime_error("Failed to allocate memory for command buffers");
                }
                return res;
            }

        } // _commandbuffer
    } // _loaders
} // vulkanhdl

