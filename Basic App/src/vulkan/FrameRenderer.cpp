#include "vulkan/FrameRenderer.hpp"

#include <stdexcept>


namespace vulkan {

    FrameRenderer::FrameRenderer(void)
    {}

    FrameRenderer::FrameRenderer(
        const vulkan::device::DeviceWrapper &deviceWrapper,
        const vulkan::swap_chain::SwapChainWrapper &swapChainWrapper,
        const VkRenderPass &renderPass
    )
    :
    _frameBuffers{this->_createFrameBuffers(
        deviceWrapper.getLogicalDevice(),
        swapChainWrapper.getImageViews(),
        swapChainWrapper.getExtent(),
        renderPass
    )},
    _commandPool{this->_createCommandPool(
        deviceWrapper.getLogicalDevice(),
        deviceWrapper.getDeviceQueueFamilies()
    )},
    _commandBuffer{this->_createCommandBuffer(
        deviceWrapper.getLogicalDevice(),
        this->_commandPool
    )},
    _imageAvailableSemaphore{
        this->_createSemaphore(deviceWrapper.getLogicalDevice())
    },
    _renderFinishedSemaphore{
        this->_createSemaphore(deviceWrapper.getLogicalDevice())
    },
    _inFlightFence{this->_createFence(deviceWrapper.getLogicalDevice())}
    {}

    void FrameRenderer::cleanUp(const VkDevice &logicalDevice) {
        vkDestroySemaphore(
            logicalDevice, this->_imageAvailableSemaphore, nullptr
        );
        vkDestroySemaphore(
            logicalDevice, this->_renderFinishedSemaphore, nullptr
        );
        vkDestroyFence(logicalDevice, this->_inFlightFence, nullptr);
        vkDestroyCommandPool(logicalDevice, this->_commandPool, nullptr);
        for (const VkFramebuffer &frameBuffer : this->_frameBuffers) {
            vkDestroyFramebuffer(logicalDevice, frameBuffer, nullptr);
        }
    }

    void FrameRenderer::drawFrame(
        const vulkan::device::DeviceWrapper &deviceWrapper,
        const pipeline::PipelineWrapper &pipelineWrapper,
        const vulkan::swap_chain::SwapChainWrapper &swapChainWrapper
    ) const {
        vkWaitForFences(
            deviceWrapper.getLogicalDevice(),
            1,
            &this->_inFlightFence,
            VK_TRUE, UINT64_MAX
        );
        vkResetFences(deviceWrapper.getLogicalDevice(), 1, &this->_inFlightFence);
        uint32_t imageIndex;
        vkAcquireNextImageKHR(
            deviceWrapper.getLogicalDevice(),
            swapChainWrapper.getSwapChain(),
            UINT64_MAX,
            this->_imageAvailableSemaphore,
            VK_NULL_HANDLE,
            &imageIndex
        );
        vkResetCommandBuffer(this->_commandBuffer, 0);
        this->_recordCommandBuffer(
            imageIndex, pipelineWrapper, swapChainWrapper.getExtent()
        );
        //Submitting the command buffer
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        VkSemaphore waitSemaphores[] = { this->_imageAvailableSemaphore };
        VkPipelineStageFlags waitStages[] = {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
        };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &this->_commandBuffer;
        VkSemaphore signalSemaphores[] = { this->_renderFinishedSemaphore };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(
            deviceWrapper.getGraphicsQueue(),
            1, 
            &submitInfo,
            this->_inFlightFence
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to submit draw command buffer");
        }
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        VkSwapchainKHR swapChains[] = { swapChainWrapper.getSwapChain() };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional
        vkQueuePresentKHR(deviceWrapper.getPresentQueue(), &presentInfo);
    }

    //private
    void FrameRenderer::_recordCommandBuffer(
        const uint32_t &imageIndex,
        const pipeline::PipelineWrapper &pipelineWrapper,
        const VkExtent2D &swapChainExtent
    ) const {
        //1. Begin command buffer
        VkCommandBufferBeginInfo beginInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags = 0, //Optional
            .pInheritanceInfo = nullptr // Optional
        };
        if (vkBeginCommandBuffer(
            this->_commandBuffer, &beginInfo
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to begin recording command buffer");
        }
        //2. Start a render pass
        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};

        VkRenderPassBeginInfo renderPassBeginInfo = {};
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass = pipelineWrapper.getRenderPass();
        renderPassBeginInfo.framebuffer = this->_frameBuffers[imageIndex];
        renderPassBeginInfo.renderArea.offset = {0, 0};
        renderPassBeginInfo.renderArea.extent = swapChainExtent;
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &clearColor;
        vkCmdBeginRenderPass(
            this->_commandBuffer,
            &renderPassBeginInfo,
            VK_SUBPASS_CONTENTS_INLINE
        );
        vkCmdBindPipeline(
            this->_commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            pipelineWrapper.getGraphicsPipeline()
        );
        VkViewport viewport = pipelineWrapper.getViewport();
        VkRect2D scissor = pipelineWrapper.getScissor();
        vkCmdSetViewport(this->_commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(this->_commandBuffer, 0, 1, &scissor);
        vkCmdDraw(this->_commandBuffer, 3, 1, 0, 0);
        vkCmdEndRenderPass(this->_commandBuffer);
        if (vkEndCommandBuffer(this->_commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("Failed to record command buffer");
        }
    }

    std::vector<VkFramebuffer> FrameRenderer::_createFrameBuffers(
        const VkDevice &logicalDevice,
        const std::vector<VkImageView> &imageViews,
        const VkExtent2D &swapChainExtent,
        const VkRenderPass &renderPass
    ) const {
        std::vector<VkFramebuffer> frameBuffers(imageViews.size());


        for (size_t i = 0; i < imageViews.size(); i++) {
            VkImageView attachments[] = { imageViews[i] };
            VkFramebufferCreateInfo createInfo = {
                .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .renderPass = renderPass,
                .attachmentCount = 1,
                .pAttachments = attachments,
                .width = swapChainExtent.width,
                .height = swapChainExtent.height,
                .layers = 1
            };
            if (vkCreateFramebuffer(
                logicalDevice, &createInfo, nullptr, &frameBuffers[i]
            ) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create a framebuffer");
            }
        }
        return frameBuffers;
    }

    VkCommandPool FrameRenderer::_createCommandPool(
        const VkDevice &logicalDevice,
        const vulkan::QueueFamilies &deviceQueueFamilies
    ) const {
        VkCommandPoolCreateInfo createInfo {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = deviceQueueFamilies.graphicsFamily.value()
        };
        VkCommandPool commandPool{};

        if (vkCreateCommandPool(
            logicalDevice, &createInfo, nullptr, &commandPool
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create the command pool");
        }
        return commandPool;
    }

    VkCommandBuffer FrameRenderer::_createCommandBuffer(
        const VkDevice &logicalDevice,
        const VkCommandPool &commandPool
    ) const {
        const VkCommandBufferAllocateInfo allocInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool = commandPool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1
        };
        VkCommandBuffer commandBuffer{};

        if (vkAllocateCommandBuffers(
            logicalDevice, &allocInfo, &commandBuffer
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate command buffers");
        }
        return commandBuffer;
    }

    VkSemaphore FrameRenderer::_createSemaphore(
        const VkDevice &logicalDevice
    ) const {
        VkSemaphoreCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
        };
        VkSemaphore semaphore{};
        if (vkCreateSemaphore(
            logicalDevice, &createInfo, nullptr, &semaphore
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create semaphore");
        }
        return semaphore;
    }

    VkFence FrameRenderer::_createFence(
        const VkDevice &logicalDevice
    ) const {
        VkFenceCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .flags = VK_FENCE_CREATE_SIGNALED_BIT
        };
        VkFence fence{};
        if (vkCreateFence(
            logicalDevice, &createInfo, nullptr, &fence
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create fence");
        }
        return fence;
    }

}