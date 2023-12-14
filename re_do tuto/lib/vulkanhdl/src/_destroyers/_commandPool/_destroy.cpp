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
#include "vulkanhdl/include/_destroyers/_commandPool/_destroy.hpp"


namespace vulkanhdl {
    namespace _destroyers {
        namespace _commandpool {

            void _destroy(
                const VkDevice &logicalDevice,
                const VkCommandPool &commandPool,
                VkAllocationCallbacks *pAllocator
            ) {
                vkDestroyCommandPool(logicalDevice, commandPool, pAllocator);
            }

        } // _commandpool
    } // _destroy
} // vulkanhdl

