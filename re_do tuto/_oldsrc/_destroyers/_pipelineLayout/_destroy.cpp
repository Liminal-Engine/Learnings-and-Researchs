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
#include "liminalvulkanwrapper/include/_destroyers/_pipelineLayout/_destroy.hpp"


namespace vulkanhdl {
    namespace _destroyers {
        namespace _pipelineLayout {

            void _destroy(
                const VkDevice &logicalDevice,
                const VkPipelineLayout &pipelineLayout,
                VkAllocationCallbacks *pAllocators
            ) {
                vkDestroyPipelineLayout(logicalDevice, pipelineLayout, pAllocators);
            }
            

        } // _pipelineLayout
    } // _destroy
} // vulkanhdl

