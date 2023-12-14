#include "vulkanhdl/include/_loaders/_pipeline/_load.hpp"
#include "vulkanhdl/include/_loaders/_shaders/_load.hpp"

/**
 * @file _load.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <vulkan/vulkan.h>
#include <vector>
#include <stdint.h>
#include <stdexcept>
#include <iostream>

namespace vulkanhdl {
    namespace _loaders {
        namespace _pipeline {

            static VkPipelineShaderStageCreateInfo __getVertexShaderCreateInfo(const VkShaderModule &vertexModule) {
                VkPipelineShaderStageCreateInfo res{};
                res.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                res.stage = VK_SHADER_STAGE_VERTEX_BIT; // Telling which pipeline stage it is
                res.module = vertexModule;
                res.pName = "main";
                return res;
            }

            static VkPipelineShaderStageCreateInfo __getFragmentShaderModuleCreateInfo(const VkShaderModule &fragmentModule) {
                VkPipelineShaderStageCreateInfo res{};
                res.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                res.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
                res.module = fragmentModule;
                res.pName = "main";
                return res;
            }

            static VkPipelineVertexInputStateCreateInfo __getVertexInputStateCreateInfo(void) {
                VkPipelineVertexInputStateCreateInfo res{};
                res.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
                res.vertexBindingDescriptionCount = 0;
                res.pVertexBindingDescriptions = nullptr; // Optional
                res.vertexAttributeDescriptionCount = 0;
                res.pVertexAttributeDescriptions = nullptr; // Optional
                return res;
            }

            static VkPipelineInputAssemblyStateCreateInfo __getInputAssemblyStateCreateInfo(void) {
                VkPipelineInputAssemblyStateCreateInfo res{};
                res.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
                res.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
                res.primitiveRestartEnable = VK_FALSE;
                return res;
            }

            static VkViewport __getViewport(const VkExtent2D &swapChainExtent) {
                VkViewport res{};
                res.x = 0.0f;
                res.y = 0.0f;
                res.width = (float)swapChainExtent.width;
                res.height = (float)swapChainExtent.height;
                res.minDepth = 0.0f; //Specify range of depth values for the framebuffer
                res.maxDepth = 1.0f;
                return res;
            }

            static VkRect2D __getScissor(const VkExtent2D &swapChainExtent) {
                VkRect2D res{};
                res.offset = {0, 0};
                res.extent = swapChainExtent;
                return res;
            }

            static VkPipelineViewportStateCreateInfo __getViewportStateCreateInfo(VkViewport *viewport, VkRect2D *scissor) {
                VkPipelineViewportStateCreateInfo res{};
                res.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
                res.viewportCount = 1;
                res.pViewports = viewport;
                res.scissorCount = 1;
                res.pScissors = scissor;
                return res;
            }

            static VkPipelineRasterizationStateCreateInfo __getRasterizationStateCreateInfo(void) {
                VkPipelineRasterizationStateCreateInfo res{};
                res.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
                res.depthClampEnable = VK_FALSE;
                res.rasterizerDiscardEnable = VK_FALSE;
                res.polygonMode = VK_POLYGON_MODE_FILL;
                res.lineWidth = 1.0f;
                res.cullMode = VK_CULL_MODE_BACK_BIT;
                res.frontFace = VK_FRONT_FACE_CLOCKWISE;
                res.depthBiasEnable = VK_FALSE;
                res.depthBiasConstantFactor = 0.0f; // Optional
                res.depthBiasClamp = 0.0f; // Optional
                res.depthBiasSlopeFactor = 0.0f; // Optional
                return res;

            }

            static VkPipelineMultisampleStateCreateInfo __getMultisampleStateCreateInfo(void) {
                VkPipelineMultisampleStateCreateInfo res{};
                res.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
                res.sampleShadingEnable = VK_FALSE;
                res.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
                res.minSampleShading = 1.0f; // Optional
                res.pSampleMask = nullptr; // Optional
                res.alphaToCoverageEnable = VK_FALSE; // Optional
                res.alphaToOneEnable = VK_FALSE; // Optional
                return res;
            }

            static VkPipelineColorBlendAttachmentState __getColorBlendAttachmentState(void) {
                VkPipelineColorBlendAttachmentState res{};
                res.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
                res.blendEnable = VK_TRUE; // Optional
                res.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA; // Optional
                res.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA; // Optional
                res.colorBlendOp = VK_BLEND_OP_ADD; // Optional
                res.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
                res.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
                res.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
                return res;
            }

            static VkPipelineColorBlendStateCreateInfo __getColorBlendStateCreateInfo(VkPipelineColorBlendAttachmentState *colorBlendAttachmentState) {
                VkPipelineColorBlendStateCreateInfo res{};
                res.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
                res.logicOpEnable = VK_FALSE;
                res.logicOp = VK_LOGIC_OP_COPY; // Optional
                res.attachmentCount = 1;
                res.pAttachments = colorBlendAttachmentState;
                res.blendConstants[0] = 0.0f; // Optional
                res.blendConstants[1] = 0.0f; // Optional
                res.blendConstants[2] = 0.0f; // Optional
                res.blendConstants[3] = 0.0f; // Optional
                /*If you preffer to use the second method of blending (bitwise combination), you must set "logicOpEnable" to
                VK_TRUE and set "logicOp". Doing so will automatically disable the first method.*/
                return res;
            }

            static std::vector<VkDynamicState> __getDynamicStates(void) {
                return std::vector<VkDynamicState> {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
            }

            static VkPipelineDynamicStateCreateInfo __getDynamicStateCreateInfo(const std::vector<VkDynamicState> &dynamicSates) {
                VkPipelineDynamicStateCreateInfo res{};
                res.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
                res.dynamicStateCount = static_cast<uint32_t>(dynamicSates.size());
                res.pDynamicStates = dynamicSates.data();
                return res;
            }


            VkPipeline _load(
                const VkDevice &logicalDevice,
                const VkExtent2D &swapChainExtent,
                const VkPipelineLayout &pipelineLayout,
                const VkRenderPass &renderPass
            ) {
                VkPipeline res;

                // Can be united in one function call
                VkShaderModule vertexModule = _shaders::_load(logicalDevice, "./shaders/shader.vert.spv");
                VkShaderModule fragmentModule = _shaders::_load(logicalDevice, "./shaders/shader.frag.spv");
                VkPipelineShaderStageCreateInfo vertexShaderModule = __getVertexShaderCreateInfo(vertexModule);
                VkPipelineShaderStageCreateInfo fragmentShaderModule = __getFragmentShaderModuleCreateInfo(fragmentModule);
                VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderModule, fragmentShaderModule };
                
                VkPipelineVertexInputStateCreateInfo vertexInputeStateCreateInfo = __getVertexInputStateCreateInfo();
                VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = __getInputAssemblyStateCreateInfo();
                VkViewport viewport = __getViewport(swapChainExtent);
                VkRect2D scissor = __getScissor(swapChainExtent); // Describes in which region pixels will actually be stored
                VkPipelineViewportStateCreateInfo viewportStateCreateInfo = __getViewportStateCreateInfo(&viewport, &scissor); // Contains viewport and scissor
                VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = __getRasterizationStateCreateInfo(); //takes geometry by vertices and turns it into fragment to be colored by the fragment shader
                VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = __getMultisampleStateCreateInfo();
                VkPipelineColorBlendAttachmentState colorBlendAttachmentState = __getColorBlendAttachmentState();
                VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = __getColorBlendStateCreateInfo(&colorBlendAttachmentState);
                std::vector<VkDynamicState> dynamicStates = __getDynamicStates();
                VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = __getDynamicStateCreateInfo(dynamicStates);

                VkGraphicsPipelineCreateInfo createInfo{};
                createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
                createInfo.stageCount = 2;
                createInfo.pStages = shaderStages;
                createInfo.pVertexInputState = &vertexInputeStateCreateInfo;
                createInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
                createInfo.pViewportState = &viewportStateCreateInfo;
                createInfo.pRasterizationState = &rasterizationStateCreateInfo;
                createInfo.pMultisampleState = &multisampleStateCreateInfo;
                createInfo.pDepthStencilState = nullptr; // Optional
                createInfo.pColorBlendState = &colorBlendStateCreateInfo;
                createInfo.pDynamicState = &dynamicStateCreateInfo;
                createInfo.layout = pipelineLayout;
                createInfo.renderPass = renderPass;
                createInfo.subpass = 0;
                createInfo.basePipelineHandle = VK_NULL_HANDLE;
                createInfo.basePipelineIndex = -1;
                if (vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &createInfo, nullptr, &res) != VK_SUCCESS) {
                    throw std::runtime_error("Failed to create graphics pipeline");
                }
                vkDestroyShaderModule(logicalDevice, vertexModule, nullptr);
                vkDestroyShaderModule(logicalDevice, fragmentModule, nullptr);
                return res;
            }

        } // _pipeline
    } // _loaders
} // _vulkanhdl

