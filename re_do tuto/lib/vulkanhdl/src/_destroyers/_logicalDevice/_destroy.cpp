#include "vulkanhdl/include/_destroy/_logicalDevice/_destroy.hpp"

/**
 * @file _destroy.cpp
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
    namespace _destroyers {
        namespace _logicaldevice {

            void _destroy(
                const VkDevice &logicalDevice,
                VkAllocationCallbacks *pAllocator
            ) {
                vkDestroyDevice(logicalDevice, pAllocator);
            }
            

        } // _logicaldevice
    } // _destroy
} // vulkanhdl

