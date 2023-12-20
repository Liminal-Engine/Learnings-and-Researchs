#ifndef VULKAN_HDL_LIB__PRIVATE___DESTROY__FRAMEBUFFERS__DESTROY_HPP
#define VULKAN_HDL_LIB__PRIVATE___DESTROY__FRAMEBUFFERS__DESTROY_HPP

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
#include <vector>

namespace vulkanhdl {
    namespace _destroyers {
        namespace _framebuffers {

            /**
             * @brief Destroy a frame buffers vector
             * 
             * @param logicalDevice [const VkDevice &] The logical device to destroy
             * @param ppAllocator [VkAllocationCallbacks **] Vulkan allocators array pointers
             */
            void _destroy(
                const VkDevice &logicalDevice,
                const std::vector<VkFramebuffer> &frameBuffers,
                VkAllocationCallbacks **ppAllocator = nullptr
            );
            

        } // _framebuffers
    } // _destroy
} // vulkanhdl


#endif // VULKAN_HDL_LIB__PRIVATE___DESTROY__FRAMEBUFFERS__DESTROY_HPP