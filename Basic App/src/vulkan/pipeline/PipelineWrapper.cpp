#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <string>
#include <sstream>

#include "vulkan/pipeline/PipelineWrapper.hpp"

namespace vulkan::pipeline {
    PipelineWrapper::PipelineWrapper(void)
    :
    _vertexShaderModule{VK_NULL_HANDLE},
    _fragmentShaderModule{VK_NULL_HANDLE},
    _layout{VK_NULL_HANDLE},
    _renderPass{VK_NULL_HANDLE},
    _graphicsPipeline{VK_NULL_HANDLE}
    {
    }

    PipelineWrapper::PipelineWrapper(
        const VkDevice &logicalDevice,
        const std::string &vertexShaderPath,
        const std::string &fragmentShaderPath,
        const vulkan::swap_chain::SwapChainWrapper &swapChainWrapper
    )
    :
    _vertexShaderModule{this->_getShaderModule(
        logicalDevice, vertexShaderPath
    )},
    _fragmentShaderModule{this->_getShaderModule(
        logicalDevice, fragmentShaderPath
    )},
    _layout{this->_createLayout(logicalDevice)},
    _renderPass{this->_createRenderPass(
        logicalDevice,
        swapChainWrapper.getImageFormat()
    )},
    _graphicsPipeline{this->_createGraphicsPipeline(
        logicalDevice,
        swapChainWrapper.getExtent(),
        swapChainWrapper.getImageFormat()
    )}
    {
    }

    void PipelineWrapper::cleanUp(const VkDevice &logicalDevice) {
        vkDestroyPipeline(logicalDevice, this->_graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(logicalDevice, this->_layout, nullptr);
        vkDestroyRenderPass(logicalDevice, this->_renderPass, nullptr);
        if (this->_vertexShaderModule != VK_NULL_HANDLE) {
            vkDestroyShaderModule(
                logicalDevice, this->_vertexShaderModule, nullptr
            );
        }
        if (this->_fragmentShaderModule != VK_NULL_HANDLE) {
            vkDestroyShaderModule(
                logicalDevice, this->_fragmentShaderModule, nullptr
            );
        }
    }

    void PipelineWrapper::setVertexShader(
        const VkDevice &logicalDevice,
        const std::string &filePath
    ) {
        this->_vertexShaderModule = this->_getShaderModule(
            logicalDevice, filePath
        );
    }

    void PipelineWrapper::setFragmentShader(
        const VkDevice &logicalDevice,
        const std::string &filePath
    ) {
        this->_fragmentShaderModule = this->_getShaderModule(
            logicalDevice, filePath
        );
    }

    //private methods
    VkShaderModule PipelineWrapper::_getShaderModule(
        const VkDevice &logicalDevice,
        const std::string &filePath
    ) const {
        //1. Check file name formatting
        // if (!filePath.ends_with(".vert.spv") || !filePath.ends_with(".frag.spv")) {
        //     throw std::runtime_error("Can't load this shader file : " + filePath);
        // }
        //2. Read shader file as binary
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);

        if (file.is_open() == false) {
            throw std::runtime_error("Failed to load file : " + filePath);
        }
        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        //3. Create shader module from shader code
        VkShaderModuleCreateInfo createInfo {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize = buffer.size(),
            .pCode = reinterpret_cast<const uint32_t *>(buffer.data()) // The bytecode pointer is a uint32_t pointer rather than a char pointer. Therefore we will need to cast the pointer with reinterpret_cast as shown below. When you perform a cast like this, you also need to ensure that the data satisfies the alignment requirements of uint32_t. Lucky for us, the data is stored in an std::vector where the default allocator already ensures that the data satisfies the worst case alignment requirements.                        
        };
        VkShaderModule shaderModule{};
        if (vkCreateShaderModule(
            logicalDevice, &createInfo, nullptr, &shaderModule
        ) != VK_SUCCESS) {
            throw std::runtime_error(
                "Failed to create shader module from" + filePath
            );
        }
        return shaderModule;
    }

    VkPipelineShaderStageCreateInfo
    PipelineWrapper::_getVertexShaderStageCreateInfo(void) const {
        return (VkPipelineShaderStageCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_VERTEX_BIT,
            .module = this->_vertexShaderModule,
            .pName = "main"
        };
    }

    VkPipelineShaderStageCreateInfo
    PipelineWrapper::_getFragmentShaderStageCreateInfo(void) const {
        return (VkPipelineShaderStageCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = this->_fragmentShaderModule,
            .pName = "main"
        };
    }

    VkPipelineDynamicStateCreateInfo
    PipelineWrapper::_createDynamicStateCreateInfo(
        const VkDynamicState *dynamicStates,
        const uint32_t &dynamicStateCount
    ) const {
        return (VkPipelineDynamicStateCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            .dynamicStateCount = dynamicStateCount,
            .pDynamicStates = dynamicStates
        };
    }

    VkPipelineVertexInputStateCreateInfo
    PipelineWrapper::_createVertexInputStateCreateInfo(void) const {
        return (VkPipelineVertexInputStateCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .vertexBindingDescriptionCount = 0,
            .pVertexBindingDescriptions = nullptr, //Optional
            .vertexAttributeDescriptionCount = 0,
            .pVertexAttributeDescriptions = nullptr, //Optional            
        };
    }

    VkPipelineInputAssemblyStateCreateInfo
    PipelineWrapper::_createInputAssemblyCreateInfo(void) const {
        return (VkPipelineInputAssemblyStateCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = VK_FALSE
        };
    }

    VkViewport PipelineWrapper::_createViewport(
        const VkExtent2D &swapChainExtent
    ) const {
        return (VkViewport) {
            .x = 0.0f,
            .y = 0.0f,
            .width = (float) swapChainExtent.width,
            .height = (float) swapChainExtent.height,
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        };
    }

    VkRect2D PipelineWrapper::_createScissor(
        const VkExtent2D &swapChainExtent
    ) const {
        return (VkRect2D) {
            .offset = {0, 0},
            .extent = swapChainExtent
        };
    }

    VkPipelineViewportStateCreateInfo
    PipelineWrapper::_createViewportStateCreateInfo(
        const VkViewport *viewport,
        const VkRect2D *scissor
    ) const {
        return (VkPipelineViewportStateCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .viewportCount = 1,
            .pViewports = viewport,
            .scissorCount = 1,
            .pScissors = scissor
        };
    }

    VkPipelineRasterizationStateCreateInfo
    PipelineWrapper::_createRasterizationStateCreateInfo(void) const {
        return (VkPipelineRasterizationStateCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .depthClampEnable = VK_FALSE,
            .rasterizerDiscardEnable = VK_FALSE,
            .polygonMode = VK_POLYGON_MODE_FILL,
            .cullMode = VK_CULL_MODE_BACK_BIT,
            .frontFace = VK_FRONT_FACE_CLOCKWISE,
            .depthBiasEnable = VK_FALSE,
            .depthBiasConstantFactor = 0.0f, //Optional
            .depthBiasClamp = 0.0f, //Optional
            .depthBiasSlopeFactor = 0.0f, //Optional
            .lineWidth = 1.0f,
        };
    }

    VkPipelineMultisampleStateCreateInfo
    PipelineWrapper::_createMultisampleStateCreateInfo(void) const {
        return (VkPipelineMultisampleStateCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
            .sampleShadingEnable = VK_FALSE,
            .minSampleShading = 1.0f, //Optional
            .pSampleMask = nullptr, //Optional
            .alphaToCoverageEnable = VK_FALSE, //Optional
            .alphaToOneEnable = VK_FALSE //Optional
        };
    }

    VkPipelineColorBlendAttachmentState
    PipelineWrapper::_createColorBlendAttachment(void) const {
        return (VkPipelineColorBlendAttachmentState) {
            .blendEnable = VK_FALSE,
            .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
            .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
            .colorBlendOp = VK_BLEND_OP_ADD,
            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
            .alphaBlendOp = VK_BLEND_OP_ADD,
            .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | 
            VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT
        };
    }

    VkPipelineColorBlendStateCreateInfo
    PipelineWrapper::_createColorBlendStateCreateInfo(
        const VkPipelineColorBlendAttachmentState *colorBlendAttachment
    ) const {

        return (VkPipelineColorBlendStateCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .logicOpEnable = VK_FALSE,
            .logicOp = VK_LOGIC_OP_COPY, // Optional
            .attachmentCount = 1,
            .pAttachments = colorBlendAttachment,
            .blendConstants = {0.0f, 0.0f, 0.0f, 0.0f}   // Optional
        };
    }

    VkPipelineLayout PipelineWrapper::_createLayout(
        const VkDevice &logicalDevice
    ) const {
        VkPipelineLayoutCreateInfo createInfo {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount = 0, // Optional
            .pSetLayouts = nullptr, // Optional
            .pushConstantRangeCount = 0, // Optional
            .pPushConstantRanges = nullptr // Optional
        };
        VkPipelineLayout layout{};

        if (vkCreatePipelineLayout(
            logicalDevice, &createInfo, nullptr, &layout
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline layout");
        }
        return layout;
    }

    VkRenderPass PipelineWrapper::_createRenderPass(
        const VkDevice &logicalDevice,
        const VkFormat &swapChainImageFormat
    ) const {
        VkAttachmentDescription colorAttachment {
            .format = swapChainImageFormat,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        };
        VkAttachmentReference colorAttachmentRef {
            .attachment = 0,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };
        VkSubpassDescription subpass {
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .colorAttachmentCount = 1,
            .pColorAttachments = &colorAttachmentRef
        };
        VkRenderPassCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .attachmentCount = 1,
            .pAttachments = &colorAttachment,
            .subpassCount = 1,
            .pSubpasses = &subpass
        };
        VkRenderPass renderPass{};

        if ( vkCreateRenderPass(
            logicalDevice, &createInfo, nullptr, &renderPass
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create the render pass.");
        }
        return renderPass;
    }

    VkPipeline PipelineWrapper::_createGraphicsPipeline(
        const VkDevice &logicalDevice,
        const VkExtent2D &swapChainExtent,
        const VkFormat &swapChainImageFormat
    ) const {

        const VkPipelineShaderStageCreateInfo *shaderStagesCreateInfo =
        new VkPipelineShaderStageCreateInfo[2] {
            this->_getVertexShaderStageCreateInfo(),
            this->_getFragmentShaderStageCreateInfo()
        };

        const VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo =
        this->_createVertexInputStateCreateInfo();

        const VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo =
        this->_createInputAssemblyCreateInfo();

        const VkViewport viewport = this->_createViewport(swapChainExtent);
        const VkRect2D scissor = this->_createScissor(swapChainExtent);
        const VkPipelineViewportStateCreateInfo viewportCreateInfo =
        this->_createViewportStateCreateInfo(&viewport, &scissor);

        const VkPipelineRasterizationStateCreateInfo rasterizationCreateInfo =
        this->_createRasterizationStateCreateInfo();

        const VkPipelineMultisampleStateCreateInfo multisampleCreateInfo =
        this->_createMultisampleStateCreateInfo();
        
        const VkDynamicState *dynamicStates = new VkDynamicState[2]
        {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};        
        const VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo =
        this->_createDynamicStateCreateInfo(dynamicStates, 2);

        
        const VkPipelineColorBlendAttachmentState colorBlendAttachment =
        this->_createColorBlendAttachment();
        const VkPipelineColorBlendStateCreateInfo colorBlendCreateInfo =
        this->_createColorBlendStateCreateInfo(&colorBlendAttachment);


        VkGraphicsPipelineCreateInfo createInfo {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .stageCount = 2,
            .pStages = shaderStagesCreateInfo,
            .pVertexInputState = &vertexInputCreateInfo,
            .pInputAssemblyState = &inputAssemblyCreateInfo,
            .pViewportState = &viewportCreateInfo,
            .pRasterizationState = &rasterizationCreateInfo,
            .pMultisampleState = &multisampleCreateInfo,
            .pDepthStencilState = nullptr, // Optional
            .pColorBlendState = &colorBlendCreateInfo,
            .pDynamicState = &dynamicStateCreateInfo,
            .layout = this->_layout,
            .renderPass = this->_renderPass,
            .subpass = 0,
            .basePipelineHandle = VK_NULL_HANDLE, // Optional
            .basePipelineIndex = -1 // Optional
        };
        VkPipeline pipeline{};

        if (vkCreateGraphicsPipelines(
            logicalDevice, VK_NULL_HANDLE, 1, &createInfo, nullptr, &pipeline
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline");
        }
        delete[] shaderStagesCreateInfo;
        delete[] dynamicStates;
        return pipeline;
    }

}