#include "vulkanhdl/include/_loaders/_pipelineLayout/_load.hpp"

#include <vulkan/vulkan.h>
#include <stdexcept>

namespace vulkanhdl {
    namespace _loaders {
        namespace _pipelinelayout {

            VkPipelineLayout _load(const VkDevice &logicalDevice) {
                VkPipelineLayout res{};
                VkPipelineLayoutCreateInfo createInfo{};

                createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
                createInfo.setLayoutCount = 0; // Optional
                createInfo.pSetLayouts = nullptr; // Optional
                createInfo.pushConstantRangeCount = 0; // Optional, other way of passing dynamic values
                createInfo.pPushConstantRanges = nullptr; // Optional

                if (vkCreatePipelineLayout(logicalDevice, &createInfo, nullptr, &res) != VK_SUCCESS) {
                    throw std::runtime_error("Failed to create pipeline layout");
                }
                return res;
            }
            
        } // _pipelinelayout
    } // _loaders
} // _vulkanhdl
