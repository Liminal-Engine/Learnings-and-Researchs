#ifndef VULKAN___FRAME_RENDERER_HPP
#define VULKAN___FRAME_RENDERER_HPP

#include <vulkan/vulkan.h>
#include <vector>

#include "vulkan/swap_chain/SwapChainWrapper.hpp"
#include "vulkan/device/DeviceWrapper.hpp"
#include "vulkan/pipeline/PipelineWrapper.hpp"

namespace vulkan {
    
    class FrameRenderer {
        public:
            FrameRenderer(void);
            FrameRenderer(
                const vulkan::device::DeviceWrapper &deviceWrapper,
                const vulkan::swap_chain::SwapChainWrapper &swapChainWrapper,
                const VkRenderPass &renderPass
            );
            void cleanUp(const VkDevice &logicalDevice);

            void drawFrame(
                const vulkan::device::DeviceWrapper &deviceWrapper,
                const pipeline::PipelineWrapper &pipelineWrapper,
                const vulkan::swap_chain::SwapChainWrapper &swapChainWrapper
            ) const;


        private:
            
            void _recordCommandBuffer(
                const uint32_t &imageIndex,
                const pipeline::PipelineWrapper &pipelineWrapper,
                const VkExtent2D &swapChainExtent
            ) const;
        
            std::vector<VkFramebuffer> _createFrameBuffers(
                const VkDevice &logicalDevice,
                const std::vector<VkImageView> &imageViews,
                const VkExtent2D &swapChainExtent,
                const VkRenderPass &renderPass
            ) const;
            std::vector<VkFramebuffer> _frameBuffers;

            VkCommandPool _createCommandPool(
                const VkDevice &logicalDevice,
                const vulkan::QueueFamilies &deviceQueueFamilies
            ) const;
            VkCommandPool _commandPool;

            VkCommandBuffer _createCommandBuffer(
                const VkDevice &logicalDevice,
                const VkCommandPool &commandPool
            ) const;
            VkCommandBuffer _commandBuffer;

            VkSemaphore _createSemaphore(const VkDevice &logicalDevice) const;
            VkSemaphore _imageAvailableSemaphore;
            VkSemaphore _renderFinishedSemaphore;

            VkFence _createFence(const VkDevice &logicalDevice) const;
            VkFence _inFlightFence;

    };

}


#endif // VULKAN___FRAME_RENDERER_HPP