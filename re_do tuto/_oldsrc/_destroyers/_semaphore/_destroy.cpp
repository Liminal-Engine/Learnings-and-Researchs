/**
 * @file _destroy.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "liminalvulkanwrapper/include/_destroyers/_semaphore/_destroy.hpp"

namespace vulkanhdl {
    namespace _destroyers {
        namespace _semaphore {

            void _destroy(
                const VkDevice &logicalDevice,
                const VkSemaphore &semaphore,
                VkAllocationCallbacks *pAllocator
            ) {
                vkDestroySemaphore(logicalDevice, semaphore, pAllocator);
            }
            

        } // _semaphore
    } // _destroy
} // vulkanhdl

