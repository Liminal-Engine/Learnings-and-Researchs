#ifndef VULKAN_HDL_LIB__PRIVATE___LOADERS__SEMAPHORE__LOAD_HPP
#define VULKAN_HDL_LIB__PRIVATE___LOADERS__SEMAPHORE__LOAD_HPP

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

#include <vulkan/vulkan.h>

namespace vulkanhdl {
    namespace _loaders {
        namespace _semaphore {

            /**
             * @brief Load a semaphore
             * 
             * @param logicalDevice [const VkDevice &] The related logical device
             * @return VkSemaphore 
             */
            VkSemaphore _load(const VkDevice &logicalDevice);

        } // namespace _semaphore
    } // namespace _loaders
} // namespace vulkanhdl


#endif // VULKAN_HDL_LIB__PRIVATE___LOADERS__SEMAPHORE__LOAD_HPP
