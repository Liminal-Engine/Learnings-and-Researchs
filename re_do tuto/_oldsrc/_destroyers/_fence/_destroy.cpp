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
#include "liminalvulkanwrapper/include/_destroyers/_fence/_destroy.hpp"

namespace vulkanhdl {
    namespace _destroyers {
        namespace _fence {

            void _destroy(
                const VkDevice &logicalDevice,
                const VkFence &fence,
                VkAllocationCallbacks *pAllocator
            ) {
                vkDestroyFence(logicalDevice, fence, pAllocator);
            }

        } // _fence
    } // _destroy
} // vulkanhdl

