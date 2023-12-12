#include "vulkanhdl/include/_loaders/_pipeline/_load.hpp"
#include "vulkanhdl/include/_loaders/_shaders/_load.hpp"

/**
 * @file _load.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <vulkan/vulkan.h>

namespace vulkanhdl {
    namespace _loaders {
        namespace _pipeline {

            VkPipeline _load(const VkDevice &logicalDevice) {
                VkPipeline res{};

                VkShaderModule vertModule = _shaders::_load(logicalDevice, "./shaders/shader.vert.spv");
                VkShaderModule fragModule = _shaders::_load(logicalDevice, "./shaders/shader.vert.spv");

                return res;
            }
            
        } // _pipeline
    } // _loaders
} // _vulkanhdl

