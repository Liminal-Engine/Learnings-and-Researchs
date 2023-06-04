#ifndef VULKAN___PIPELINE___PIPELINEWRAPPER_HPP
#define VULKAN___PIPELINE___PIPELINEWRAPPER_HPP

#include <vulkan/vulkan.h>
#include <string>
#include <unordered_map>
#include <vector>

#include <vulkan/swap_chain/SwapChainWrapper.hpp>

namespace vulkan::pipeline {
    
    class PipelineWrapper {
        public:
            PipelineWrapper(void);
            PipelineWrapper(
                const VkDevice &logicalDevice,
                const std::string &vertexShaderPath,
                const std::string &fragmentShaderPath,
                const vulkan::swap_chain::SwapChainWrapper &swapChainWrapper
            );

            void cleanUp(const VkDevice &logicalDevice);

            void setVertexShader(
                const VkDevice &logicalDevice,
                const std::string &filePath
            );

            void setFragmentShader(
                const VkDevice &logicalDevice,
                const std::string &filePath
            );

        private:
            VkShaderModule _getShaderModule(
                const VkDevice &logicalDevice,
                const std::string &filePath
            ) const;

            VkPipelineShaderStageCreateInfo
            _getVertexShaderStageCreateInfo(void) const;

            VkPipelineShaderStageCreateInfo
            _getFragmentShaderStageCreateInfo(void) const;

           

            VkPipelineDynamicStateCreateInfo
            _createDynamicStateCreateInfo(
                const VkDynamicState *dynamicStates,
                const uint32_t &dynamicStateCount
            ) const;

            VkPipelineVertexInputStateCreateInfo
            _createVertexInputStateCreateInfo(void) const;

            VkPipelineInputAssemblyStateCreateInfo
            _createInputAssemblyCreateInfo(void) const;


            VkViewport _createViewport(const VkExtent2D &swapChainExtent) const;
            VkRect2D _createScissor(const VkExtent2D &swapChainExtent) const;
            VkPipelineViewportStateCreateInfo
            _createViewportStateCreateInfo(
                const VkViewport *viewport,
                const VkRect2D *scissor
            ) const;
        

            VkPipelineRasterizationStateCreateInfo
            _createRasterizationStateCreateInfo(void) const;

            VkPipelineMultisampleStateCreateInfo
            _createMultisampleStateCreateInfo(void) const;


            VkPipelineColorBlendAttachmentState
            _createColorBlendAttachment(void) const;
            VkPipelineColorBlendStateCreateInfo
            _createColorBlendStateCreateInfo(
                const VkPipelineColorBlendAttachmentState *colorBlendAttachment
            ) const;

            VkPipelineLayout _createLayout(const VkDevice &logicalDevice) const;

            VkRenderPass _createRenderPass(
                const VkDevice &logicalDevice,
                const VkFormat &swapChainImageFormat
            ) const;

            VkPipeline _createGraphicsPipeline(
                const VkDevice &logicalDevice,
                const VkExtent2D &swapChainExtent,
                const VkFormat &swapChainImageFormat
            ) const;

            VkShaderModule _vertexShaderModule;
            VkShaderModule _fragmentShaderModule;

            VkPipelineLayout _layout;
            VkRenderPass _renderPass;
            VkPipeline _graphicsPipeline;
    };

}

#endif // VULKAN___PIPELINE___PIPELINEWRAPPER_HPP