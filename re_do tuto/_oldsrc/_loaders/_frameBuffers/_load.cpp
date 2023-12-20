#include "liminalvulkanwrapper/include/_loaders/_frameBuffers/_load.hpp"

#include <vulkan/vulkan.hpp>
#include <stdexcept>


namespace vulkanhdl {
    namespace _loaders {
        namespace _framebuffers {

            std::vector<VkFramebuffer> _load(
                const VkDevice &logicalDevice,
                const std::vector<VkImageView> &swapChainImageView,
                const VkExtent2D &swapChainExtent,
                const VkRenderPass &renderPass
            ) {
                std::vector<VkFramebuffer> res(swapChainImageView.size());

                for (size_t i = 0; i < swapChainImageView.size(); i++) {
                    VkImageView attachments[] = { swapChainImageView[i] };
                    VkFramebufferCreateInfo createInfo{};
                    createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                    createInfo.renderPass = renderPass;
                    //Specify the "VkImageView" objects that should be bound to the respective attachment descriptions in the render pass "pAttachment" array
                    createInfo.attachmentCount = 1;
                    createInfo.pAttachments = attachments;
                    createInfo.width = swapChainExtent.width;
                    createInfo.height = swapChainExtent.height;
                    createInfo.layers = 1; // Number of layers in the image arrays (in the VkImageView attachments ?)
                    if (vkCreateFramebuffer(logicalDevice, &createInfo, nullptr, &res[i]) != VK_SUCCESS) {
                        throw std::runtime_error("Failed to create frame buffer");
                    }
                }
                return res;
            }

        } // namespace _framebuffers
    } // namespace _loaders
} // namespace vulkanhdl


