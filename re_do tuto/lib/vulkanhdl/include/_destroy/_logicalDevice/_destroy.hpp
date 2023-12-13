#ifndef VULKAN_HDL_LIB__PRIVATE___DESTROY__LOGICALDEVICE__DESTROY_HPP
#define VULKAN_HDL_LIB__PRIVATE___DESTROY__LOGICALDEVICE__DESTROY_HPP

/**
 * @file _destroy.hpp
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

            /**
             * @brief Destroy a logical device
             * 
             * @param logicalDevice [const VkDevice &] The logical device to destroy
             * @param pAllocator [VkAllocationCallbacks *] Vulkan allocators
             */
            void _destroy(
                const VkDevice &logicalDevice,
                VkAllocationCallbacks *pAllocator = nullptr
            );
            

        } // _logicaldevice
    } // _destroy
} // vulkanhdl


#endif // VULKAN_HDL_LIB__PRIVATE___DESTROY__LOGICALDEVICE__DESTROY_HPP