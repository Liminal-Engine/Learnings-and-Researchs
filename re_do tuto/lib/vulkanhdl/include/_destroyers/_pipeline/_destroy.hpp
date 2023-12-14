#ifndef VULKAN_HDL_LIB__PRIVATE___DESTROY__PIPELINE__DESTROY_HPP
#define VULKAN_HDL_LIB__PRIVATE___DESTROY__PIPELINE__DESTROY_HPP

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
        namespace _pipeline {

            /**
             * @brief Destroy a pipeline
             * 
             * @param logicalDevice [const VkDevice &] The related logical device
             * @param pipeline [const VkPipeline &] The pipeline
             * @param pAllocator [VkAllocationCallbacks *] Vulkan allocation callbacks 
             */
            void _destroy(
                const VkDevice &logicalDevice,
                const VkPipeline &pipeline,
                VkAllocationCallbacks *pAllocators = nullptr
            );
            

        } // _pipeline
    } // _destroy
} // vulkanhdl


#endif // VULKAN_HDL_LIB__PRIVATE___DESTROY__PIPELINE__DESTROY_HPP