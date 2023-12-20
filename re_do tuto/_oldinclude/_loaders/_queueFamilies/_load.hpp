#ifndef VULKAN_HDL_LIB__PRIVATE___LOADERS__QUEUEFAMILIES__LOAD_HPP
#define VULKAN_HDL_LIB__PRIVATE___LOADERS__QUEUEFAMILIES__LOAD_HPP

/**
 * @file _load.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "liminalvulkanwrapper/include/_queues/_queues.hpp"

#include <vulkan/vulkan.h>

namespace vulkanhdl {
    namespace _loaders {
        namespace _queueFamilies {

            /**
             * @brief Finds the queue families for the given physical device and surface
             * 
             * @param physicalDevice [VkPhysicalDevice] The physical device
             * @param surface [VkSurfaceKHR] The surface
             * @return [_queues::_QueueFamilies] The loaded queue families
             */
            _queues::_QueueFamilies _load(
                const VkPhysicalDevice &physicalDevice,
                const VkSurfaceKHR &surface
            );
        } // _queueFamilies
    } // _loaders
} // _vulkanhdl

#endif // VULKAN_HDL_LIB__PRIVATE___LOADERS__QUEUEFAMILIES__LOAD_HPP
