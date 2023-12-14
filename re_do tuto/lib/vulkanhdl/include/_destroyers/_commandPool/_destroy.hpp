#ifndef VULKAN_HDL_LIB__PRIVATE___DESTROY__COMMANDPOOL__DESTROY_HPP
#define VULKAN_HDL_LIB__PRIVATE___DESTROY__COMMANDPOOL__DESTROY_HPP

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
        namespace _commandpool {

            /**
             * @brief Destroy a command pool
             * 
             * @param logicalDevice [const VkDevice &] The logical device to destroy
             * @param commandPool [const VkCommandPool &] The command pool to destroy
             * @param pAllocator [VkAllocationCallbacks *] Vulkan allocator
             */
            void _destroy(
                const VkDevice &logicalDevice,
                const VkCommandPool &commandPool,
                VkAllocationCallbacks *pAllocator = nullptr
            );

        } // _commandpool
    } // _destroy
} // vulkanhdl


#endif // VULKAN_HDL_LIB__PRIVATE___DESTROY__COMMANDPOOL__DESTROY_HPP