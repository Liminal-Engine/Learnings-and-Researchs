#ifndef VULKAN_HDL_LIB__PRIVATE___LOADERS__SHADERS__LOAD_HPP
#define VULKAN_HDL_LIB__PRIVATE___LOADERS__SHADERS__LOAD_HPP

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

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <string>

namespace vulkanhdl {
    namespace _loaders {
        namespace _shaders {

            /**
             * @brief Create a shader module from the given path
             * 
             * @param logicalDevice [const VkDevice &] The related device
             * @param shaderFilePath [const std::string &] The shader file path
             * @return VkShaderModule The create shader module
             */
            VkShaderModule _load(const VkDevice &logicalDevice, const std::string &shaderFilePath);

        } // namespace _shaders
    } // namespace _loaders
} // namespace vulkanhdl


#endif // VULKAN_HDL_LIB__PRIVATE___LOADERS__SHADERS__LOAD_HPP
