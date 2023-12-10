#ifndef VULKAN_HDL_LIB__PRIVATE___LOADERS__QUEUEHANDLERS__LOAD_HPP
#define VULKAN_HDL_LIB__PRIVATE___LOADERS__QUEUEHANDLERS__LOAD_HPP

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

#include "vulkanhdl/include/_queues/_queues.hpp"

#include <vulkan/vulkan.h>

namespace vulkanhdl {
    namespace _loaders {
        namespace _queueHandlers {

            /**
             * @brief Bind and get the queue handlers of a device
             * 
             * @param logicalDevice [const VkDevice &] The related logical device
             * @param queueFamilies [const _queues::_QueueFamilies &] The queue families
             * @return [_queues::_QueueHandlers_t] The queue handlers
             */
            _queues::_QueueHandlers_t _load(
                const VkDevice &logicalDevice,
                const _queues::_QueueFamilies &queueFamilies
            );
        } // _queueHandlers
    } // _loaders
} // _vulkanhdl

#endif // VULKAN_HDL_LIB__PRIVATE___LOADERS__QUEUEHANDLERS__LOAD_HPP