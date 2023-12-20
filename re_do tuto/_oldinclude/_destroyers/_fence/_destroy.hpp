#ifndef VULKAN_HDL_LIB__PRIVATE___DESTROY__FENCE__DESTROY_HPP
#define VULKAN_HDL_LIB__PRIVATE___DESTROY__FENCE__DESTROY_HPP

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
        namespace _fence {

            /**
             * @brief Destroy a command pool
             * 
             * @param logicalDevice [const VkDevice &] The logical device to destroy
             * @param fence [const VkFence &] The fence to destroy
             * @param pAllocator [VkAllocationCallbacks *] Vulkan allocator
             */
            void _destroy(
                const VkDevice &logicalDevice,
                const VkFence &fence,
                VkAllocationCallbacks *pAllocator = nullptr
            );

        } // _fence
    } // _destroy
} // vulkanhdl


#endif // VULKAN_HDL_LIB__PRIVATE___DESTROY__FENCE__DESTROY_HPP