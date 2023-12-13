#include "vulkanhdl/include/_loaders/_fence/_load.hpp"

#include <vulkan/vulkan.h>
#include <stdexcept>

namespace vulkanhdl {
    namespace _loaders {
        namespace _fence {


            VkFence _load(const VkDevice &logicalDevice) {
                VkFence res{};
                VkFenceCreateInfo createInfo{};
                createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
                createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
                if (vkCreateFence(logicalDevice, &createInfo, nullptr, &res) != VK_SUCCESS) {
                    throw std::runtime_error("Failed to create fence");
                }
                return res;
            }

        } // namespace _fence
    } // namespace _loaders
} // namespace vulkanhdl


