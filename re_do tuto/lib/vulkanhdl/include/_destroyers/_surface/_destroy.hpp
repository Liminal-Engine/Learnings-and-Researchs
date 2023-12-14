#ifndef VULKAN_HDL_LIB__PRIVATE___DESTROY__SURFACE__DESTROY_HPP
#define VULKAN_HDL_LIB__PRIVATE___DESTROY__SURFACE__DESTROY_HPP

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
        namespace _surface {

            /**
             * @brief Destroy a surface
             * 
             * @param instance [const VkInstance &] The related Vulkan instance
             * @param surface [const VkSurfaceKHR &] The related surface
             * @param pAllocator [VkAllocationCallbacks *] Vulkan allocation callbacks 
             */
            void _destroy(
                const VkInstance &instance,
                const VkSurfaceKHR &surface, 
                VkAllocationCallbacks *pAllocator = nullptr
            );
            

        } // _surface
    } // _destroy
} // vulkanhdl


#endif // VULKAN_HDL_LIB__PRIVATE___DESTROY__SURFACE__DESTROY_HPP