#include "vulkanhdl/include/_loaders/_semaphore/_load.hpp"

#include <vulkan/vulkan.h>
#include <stdexcept>

namespace vulkanhdl {
    namespace _loaders {
        namespace _semaphore {

            VkSemaphore _load(const VkDevice &logicalDevice) {
                VkSemaphore res{};
                VkSemaphoreCreateInfo createInfo{};
                createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

                if (vkCreateSemaphore(logicalDevice, &createInfo, nullptr, &res) != VK_SUCCESS) {
                    throw std::runtime_error("Failed to create semaphore");
                }
                return res;
            }

        } // namespace _semaphore
    } // namespace _loaders
} // namespace vulkanhdl


