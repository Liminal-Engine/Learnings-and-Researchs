#ifndef VULKAN_HDL_LIB__PRIVATE___LOADERS__PIPELINE__LOAD_HPP
#define VULKAN_HDL_LIB__PRIVATE___LOADERS__PIPELINE__LOAD_HPP

/**
 * @file _load.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "liminalvulkanwrapper/include/_queues/_queues.hpp"

#include <vulkan/vulkan.h>

namespace vulkanhdl {
    namespace _loaders {
        namespace _pipeline {

            /**
             * @brief Load a pipeline
             * 
             * @param logicalDevice [const VkDevice &] The related logical device
             * @param swapChainExtent [const VkExtent2D &] The related swap chain extent
             * @param pipelineLayout [const VkPipelineLayout &pipelineLayout] The related pipeline layout
             * @param renderPass [const VkRenderPass &] The related render pass
             * @return VkPipelineLayout 
             */
            VkPipeline _load(
                const VkDevice &logicalDevice,
                const VkExtent2D &swapChainExtent,
                const VkPipelineLayout &pipelineLayout,
                const VkRenderPass &renderPass
            );
            
        } // _pipeline
    } // _loaders
} // _vulkanhdl

#endif // VULKAN_HDL_LIB__PRIVATE___LOADERS__PIPELINE__LOAD_HPP
