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

#include "vulkanhdl/include/_queues/_queues.hpp"

#include <vulkan/vulkan.h>

namespace vulkanhdl {
    namespace _loaders {
        namespace _pipeline {

            /**
             * @brief Load a pipeline
             * 
             * @param logicalDevice [const VkDevice &] The related logical device
             * @return VkPipelineLayout 
             */
            VkPipeline _load(const VkDevice &logicalDevice);
            
        } // _pipeline
    } // _loaders
} // _vulkanhdl

#endif // VULKAN_HDL_LIB__PRIVATE___LOADERS__PIPELINE__LOAD_HPP
