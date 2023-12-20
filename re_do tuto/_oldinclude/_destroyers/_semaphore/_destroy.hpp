#ifndef VULKAN_HDL_LIB__PRIVATE___DESTROY__SEMAPHORE__DESTROY_HPP
#define VULKAN_HDL_LIB__PRIVATE___DESTROY__SEMAPHORE__DESTROY_HPP

/**
 * @file _destroy.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <vulkan/vulkan.h>

namespace vulkanhdl {
    namespace _destroyers {
        namespace _semaphore {

            /**
             * @brief Destroy a sempahore
             * 
             * @param logicalDevice [const VkDevice &] The related logical device
             * @param semaphore [const VkSemaphore &] The semaphore to destroy
             * @param pAllocator [VkAllocationCallbacks *] Vulkan allocation callbacks 
             */
            void _destroy(
                const VkDevice &logicalDevice,
                const VkSemaphore &semaphore,
                VkAllocationCallbacks *pAllocator = nullptr
            );
            

        } // _semaphore
    } // _destroy
} // vulkanhdl


#endif // VULKAN_HDL_LIB__PRIVATE___DESTROY__SEMAPHORE__DESTROY_HPP