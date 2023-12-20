#ifndef VULKAN_HDL_LIB__PRIVATE___QUEUES_HPP
#define VULKAN_HDL_LIB__PRIVATE___QUEUES_HPP

/**
 * @file _queues.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <optional>
#include <stdint.h>
#include <set>
#include <unordered_map>
#include <string>
#include <vector>

namespace vulkanhdl {
    namespace _queues {

        typedef uint32_t _QueueFamilyIndex_t; ///< _QueueFamilyIndex_t type
        typedef std::unordered_map<std::string, VkQueue> _QueueHandlers_t; ///< _QueueHandlers_t type = queue handlers for a specific device

        /**
         * @brief _QueueFamilies Store the queue families of a device
         */
        struct _QueueFamilies {
            std::optional<_QueueFamilyIndex_t> _graphicsFamily; ///< The graphics family
            std::optional<_QueueFamilyIndex_t> _presentFamily; ///< The presentation family
            /**
             * @brief Check if the queue family meets all requirements
             * 
             * @return true if the queue family meets all requirements
             * @return false if the queue family does not meet all requirements
             */
            bool _isComplete(void) const;
            /**
             * @brief Get the indices set of the queue families indices
             * 
             * @return std::set<_QueueFamilyIndex_t> The set of queue familie indices
             */
            std::set<_QueueFamilyIndex_t> _toSet(void) const;
            /**
             * @brief Get the indices vector of the queue families indices
             * 
             * @return std::vector <_QueueFamilyIndex_t> 
             */
            std::vector <_QueueFamilyIndex_t> _toVector(void) const;
        };

    } // namesapce _queues


} // namespace vulkanhdl

#endif // VULKAN_HDL_LIB__PRIVATE___QUEUES_HPP
