#ifndef VULKAN___PIPELINE___PIPELINEWRAPPER_HPP
#define VULKAN___PIPELINE___PIPELINEWRAPPER_HPP

#include <vulkan/vulkan.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace vulkan::pipeline {
    
    class PipelineWrapper {
        public:
            PipelineWrapper(void);
            PipelineWrapper(
                const VkDevice &logicalDevice,
                const std::string &shaderDirectory
            );

            void cleanUp(const VkDevice &logicalDevice);

            void addShadersFromDirectory(
                const VkDevice &logicalDevice,
                const std::string &shaderDirectory
            );

            void addVertexShader(
                const VkDevice &logicalDevice,
                const std::string &name,
                const std::string &filePath
            );

            void addFragmentShader(
                const VkDevice &logicalDevice,
                const std::string &name,
                const std::string &filePath
            );

            void deleteVertexShader(
                const VkDevice &logicalDevice, const std::string &name
            );

            void deleteFragmentShader(
                const VkDevice &logicalDevice, const std::string &name
            );

        private:
            VkShaderModule _getShaderModule(
                const VkDevice &logicalDevice,
                const std::string &filePath
            );
            void _addShaderModule(
                const std::pair<std::string, VkShaderModule> &pair,
                std::unordered_map<std::string, VkShaderModule> &unorderedMap
            );
            void _deleteShaderModule(
                const VkDevice &logicalDevice,
                const std::string &name,
                std::unordered_map<std::string, VkShaderModule> &unorderedMap
            );

            std::unordered_map<std::string, VkShaderModule>
            _vertexShaderModules;

            std::unordered_map<std::string, VkShaderModule>
            _fragmentShaderModules;
    };

}

#endif // VULKAN___PIPELINE___PIPELINEWRAPPER_HPP