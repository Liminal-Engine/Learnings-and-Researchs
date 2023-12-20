#ifndef VULKAN_HDL_LIB__PRIVATE___LOADERS__COMMANDPOOL__LOAD_HPP
#define VULKAN_HDL_LIB__PRIVATE___LOADERS__COMMANDPOOL__LOAD_HPP

/**
 * @file _load.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "liminalvulkanwrapper/include/_queues/_queues.hpp"

#include <vulkan/vulkan.h>

namespace vulkanhdl {
    namespace _loaders {
        namespace _commandpool {

            /**
             * @brief Load a command pool
             * 
             * @param logicalDevice [const VkDevice &] The related logical device
             * @param queueFamilies [const _queues::_QueueFamilies &] The related queue families
             * @return VkCommandPool 
             */
            VkCommandPool _load(
                const VkDevice &logicalDevice,
                const _queues::_QueueFamilies &queueFamilies
            );
            

        } // _commandpool
    } // vulkanhdl
} // _loaders


#endif // VULKAN_HDL_LIB__PRIVATE___LOADERS__COMMANDPOOLS__LOAD_HPP