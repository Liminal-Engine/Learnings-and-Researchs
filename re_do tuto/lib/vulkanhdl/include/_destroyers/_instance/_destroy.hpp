#ifndef VULKAN_HDL_LIB__PRIVATE___DESTROY__INSTANCE__DESTROY_HPP
#define VULKAN_HDL_LIB__PRIVATE___DESTROY__INSTANCE__DESTROY_HPP

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
        namespace _instance {

            /**
             * @brief Destroy a Vulkan instance
             * 
             * @param instance [const VkInstance &] The instance to destroy
             * @param pAllocator [VkAllocationCallbacks *] Vulkan allocators
             */
            void _destroy(
                const VkInstance &instance,
                VkAllocationCallbacks *pAllocator = nullptr
            );
            

        } // _instance
    } // _destroy
} // vulkanhdl


#endif // VULKAN_HDL_LIB__PRIVATE___DESTROY__INSTANCE__DESTROY_HPP