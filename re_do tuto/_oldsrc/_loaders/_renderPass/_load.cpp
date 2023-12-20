#include "liminalvulkanwrapper/include/_loaders/_renderPass/_load.hpp"

#include <vulkan/vulkan.h>
#include <stdexcept>

namespace vulkanhdl {
    namespace _loaders {
        namespace _renderpass {

            static VkAttachmentDescription __getColorAttachment(const VkFormat &swapChainImagesFormat) {
                VkAttachmentDescription res{};

                res.format = swapChainImagesFormat; //color attachment format must match the swap chain format
                res.samples = VK_SAMPLE_COUNT_1_BIT; // we are not multisampling yet
                res.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // Preservers the exisintg content of the attachment before rendering
                res.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // Rendered contents will be sstored in memory and can be read later
                res.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; // We don't use stencil for now
                res.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; // We don't use stencil for now
                res.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; // ?
                res.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // ?
                return res;
            }

            static VkAttachmentReference __getColorAttachmentRef(void) {
                VkAttachmentReference res{};

                res.attachment = 0;
                res.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                return res;
            }

            static VkSubpassDescription __getSubpass(VkAttachmentReference *colorAttachmentRefPtr) {
                VkSubpassDescription res{};

                res.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                res.colorAttachmentCount = 1;
                res.pColorAttachments = colorAttachmentRefPtr;
                return res;
            }

            static VkSubpassDependency __getSubpassDependency(void) {
                VkSubpassDependency res{};

                res.srcSubpass = VK_SUBPASS_EXTERNAL; // ?
                res.dstSubpass = 0; // ?
                res.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                res.srcAccessMask = 0;
                res.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                res.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                return res;
            }
           
            VkRenderPass _load(const VkDevice &logicalDevice, const VkFormat &swapChainImagesFormat) {
                VkRenderPass res{};
                VkAttachmentDescription colorAttachment = __getColorAttachment(swapChainImagesFormat); // We'll only have 1 color buffer attachment represented by one of the images from the swap chain. attachments are the images that the redenring process will read from or read to
                VkAttachmentReference colorAttachmentRef = __getColorAttachmentRef();
                VkSubpassDescription subpass = __getSubpass(&colorAttachmentRef); // specifies ordering constraints between subpasses
                VkSubpassDependency subpassDependency = __getSubpassDependency();

                VkRenderPassCreateInfo createInfo{};
                createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
                createInfo.attachmentCount = 1;
                createInfo.pAttachments = &colorAttachment;
                createInfo.subpassCount = 1;
                createInfo.pSubpasses = &subpass;
                createInfo.dependencyCount = 1;
                createInfo.pDependencies = &subpassDependency;

                if (vkCreateRenderPass(logicalDevice, &createInfo, nullptr, &res) != VK_SUCCESS) {
                    throw std::runtime_error("Failed to create render pass");
                }
                return res;
            }
           
        } // namespace _renderpass
    } // namespace _loaders
} // namespace vulkanhdl


