#include "vulkanhdl/include/_loaders/_shaders/_load.hpp"
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

#include <fstream>
#include <vector>
#include <stdexcept>
#include <stdint.h>

namespace vulkanhdl {
    namespace _loaders {
        namespace _shaders {

            static std::vector<char> __getShaderCode(const std::string &shaderFilePath) {
                // 1. Seak at the end and read as binary to avoid text transformation. Increase performances
                std::ifstream file(shaderFilePath, std::ios::ate | std::ios::binary);
                if (file.is_open() == false) {
                    throw std::runtime_error("Failed to open shader file : " + shaderFilePath);
                }
                // 2. Get file size and allocate vector
                size_t fileSize = file.tellg();
                std::vector<char> res(fileSize);
                // 3. Seek at the begining and read all at once
                file.seekg(0);
                file.read(res.data(), fileSize);
                file.close();
                return res;
            }

            VkShaderModule _load(const VkDevice &logicalDevice, const std::string &shaderFilePath) {
                const std::vector<char> code = __getShaderCode(shaderFilePath);
                
                VkShaderModuleCreateInfo createInfo{};
                createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
                createInfo.codeSize = code.size();
                createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data()); //code is formated in uint32_t *

                VkShaderModule res{};
                if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &res) != VK_SUCCESS) {
                    throw std::runtime_error("Failed to create shader module : " + shaderFilePath);
                }
                return res;
            }

        } // namespace _shaders
    } // namespace _loaders
} // namespace vulkanhdl

