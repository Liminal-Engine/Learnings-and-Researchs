#ifndef VULKAN_HDL_LIB__PRIVATE___LOADERS__PIPELINELAYOUT__LOAD_HPP
#define VULKAN_HDL_LIB__PRIVATE___LOADERS__PIPELINELAYOUT__LOAD_HPP

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
        namespace _pipelinelayout {

            /**
             * @brief Load a pipeline layout
             * 
             * @param logicalDevice [const VkDevice &] The related logical device
             * @return VkPipelineLayout 
             */
            VkPipelineLayout _load(const VkDevice &logicalDevice);
            
        } // _pipelinelayout
    } // _loaders
} // _vulkanhdl

#endif // VULKAN_HDL_LIB__PRIVATE___LOADERS__PIPELINELAYOUT__LOAD_HPP
