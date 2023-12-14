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
#include "vulkanhdl/include/_destroyers/_pipeline/_destroy.hpp"

namespace vulkanhdl {
    namespace _destroyers {
        namespace _pipeline {

            void _destroy(
                const VkDevice &logicalDevice,
                const VkPipeline &pipeline,
                VkAllocationCallbacks *pAllocators
            ) {
                vkDestroyPipeline(logicalDevice, pipeline, pAllocators);
            }
            

        } // _pipeline
    } // _destroy
} // vulkanhdl
