#ifndef VULKAN_HDL_LIB__PRIVATE___DESTROY__PIPELINELAYOUT__DESTROY_HPP
#define VULKAN_HDL_LIB__PRIVATE___DESTROY__PIPELINELAYOUT__DESTROY_HPP

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
        namespace _pipelineLayout {

            /**
             * @brief Destroy a pipeline layout
             * 
             * @param logicalDevice [const VkDevice &] The related logical device
             * @param pipelineLayout [const VkPipelineLayout &] The pipeline layout to destroy
             * @param pAllocator [VkAllocationCallbacks *] Vulkan allocation callbacks 
             */
            void _destroy(
                const VkDevice &logicalDevice,
                const VkPipelineLayout &pipelineLayout,
                VkAllocationCallbacks *pAllocators = nullptr
            );
            

        } // _pipelineLayout
    } // _destroy
} // vulkanhdl


#endif // VULKAN_HDL_LIB__PRIVATE___DESTROY__PIPELINELAYOUT__DESTROY_HPP