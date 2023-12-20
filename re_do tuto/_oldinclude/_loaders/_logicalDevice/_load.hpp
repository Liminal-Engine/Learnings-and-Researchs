#ifndef VULKAN_HDL_LIB__PRIVATE___LOADERS__LOGICALDEVICE__LOAD_HPP
#define VULKAN_HDL_LIB__PRIVATE___LOADERS__LOGICALDEVICE__LOAD_HPP

#include "liminalvulkanwrapper/include/_queues/_queues.hpp"

#include <vulkan/vulkan.h>
#include <vector>
#include <set>

namespace vulkanhdl {
    namespace _loaders {
        namespace _logicalDevice {

            /**
             * @brief Load a logical device
             * 
             * @param physicalDevice [const VkPhysicalDevice &] The physical device
             * @param surface [const VkSurfaceKHR &] The surface
             * @param families_indices [const std::set<_queues::_QueueFamilyIndex_t> &] A set of the indices of the device families
             * @param device_extensions [const std::vector<const char *> &] Device extensions
             * @return [VkDevice] The created logical device
             */
            VkDevice _load(
                const VkPhysicalDevice &physicalDevice,
                const VkSurfaceKHR &surface,
                const std::set<_queues::_QueueFamilyIndex_t> &families_indices,
                const std::vector<const char *> &device_extensions
            );

        } // namespace _logicalDevice
    } // namespace _loaders
} // namespace vulkanhdl


#endif // VULKAN_HDL_LIB__PRIVATE___LOADERS__LOGICALDEVICE__LOAD_HPP