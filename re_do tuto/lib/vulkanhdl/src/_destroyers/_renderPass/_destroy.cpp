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

#include "vulkanhdl/include/_destroyers/_renderPass/_destroy.hpp"


namespace vulkanhdl {
    namespace _destroyers {
        namespace _renderpass {

            void _destroy(
                const VkDevice &logicalDevice,
                const VkRenderPass &renderPass,
                VkAllocationCallbacks *pAllocators
            ) {
                vkDestroyRenderPass(logicalDevice, renderPass, pAllocators);
            }
            

        } // _renderpass
    } // _destroy
} // vulkanhdl

