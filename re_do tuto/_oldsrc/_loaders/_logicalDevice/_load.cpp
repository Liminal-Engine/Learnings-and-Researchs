#include "lib/liminalvulkanwrapper/include/_loaders/_logicalDevice/_load.hpp"
#include "liminalvulkanwrapper/include/_queues/_queues.hpp"

#include <vulkan/vulkan.h>
#include <stdexcept>

namespace vulkanhdl {
    namespace _loaders {
        namespace _logicalDevice {

            static std::vector<VkDeviceQueueCreateInfo> __getQueueCreateInfos(
                const VkPhysicalDevice &physicalDevice,
                const VkSurfaceKHR &surface,
                const std::set<_queues::_QueueFamilyIndex_t> &families_indices
            ) {
                std::vector<VkDeviceQueueCreateInfo> res;
                float queuePriorities[] = {1.0f}; // Scheduling command buffer execution. We must create one float per each queue index

                for (_queues::_QueueFamilyIndex_t index : families_indices) {
                    VkDeviceQueueCreateInfo queueCreateInfo{};
                    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                    queueCreateInfo.queueFamilyIndex = index;
                    queueCreateInfo.queueCount = 1;
                    queueCreateInfo.pQueuePriorities = queuePriorities; //TODO new float{x} but we need to free it

                    res.push_back(queueCreateInfo);
                }
                return res;
            }

            VkDevice _load(
                const VkPhysicalDevice &physicalDevice,
                const VkSurfaceKHR &surface,
                const std::set<_queues::_QueueFamilyIndex_t> &families_indices,
                const std::vector<const char *> &device_extensions
            ) {
                // 1. Get the createInfo struct for all logical device queue families
                std::vector<VkDeviceQueueCreateInfo> queueCreateInfos = __getQueueCreateInfos(physicalDevice, surface, families_indices);
                // 2. Get the features we want to enable for the device
                VkPhysicalDeviceFeatures deviceFeatures{}; //Leave blank for now as we don't anything special
                // 3. Create the logical device
                VkDeviceCreateInfo createInfo{};
                createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
                createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
                createInfo.pQueueCreateInfos = queueCreateInfos.data();
                createInfo.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
                createInfo.ppEnabledExtensionNames = device_extensions.data();
                createInfo.pEnabledFeatures = &deviceFeatures;
                createInfo.pNext = nullptr;

                VkDevice res{};
                if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &res) != VK_SUCCESS) {
                    throw std::runtime_error("Failed to create logical device");
                }
                return res;
            }

        } // namespace _logicalDevice
    } // namespace _loaders
} // namespace vulkanhdl