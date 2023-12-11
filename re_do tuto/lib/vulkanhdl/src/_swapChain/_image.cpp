#include "vulkanhdl/include/_swapChain/_image.hpp"

#include <limits>

namespace vulkanhdl {
    namespace _swapchain {

        VkPresentModeKHR _getBestPresentationMode(const std::vector<VkPresentModeKHR> &availablePresentationModes) {
                for (const auto &presentationMode : availablePresentationModes) {
                    if (presentationMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                        return presentationMode;
                    }
                }
                return VK_PRESENT_MODE_FIFO_KHR;
        }

        VkSurfaceFormatKHR _getBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &supportedFormats) {
            for (const auto &format : supportedFormats) {
                if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                    return format;
                }
            }
            return supportedFormats.at(0);
        }

        VkExtent2D _getBestExtent(GLFWwindow *window, const VkSurfaceCapabilitiesKHR &surfaceCapabilities) {
            if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
                return surfaceCapabilities.currentExtent;
            }
            int width = 0;
            int height = 0;
            VkExtent2D extent{};

            // 1. Get the width and height
            glfwGetFramebufferSize(window, &width, &height);
            // 2. Set values to max uint32_t and cast
            extent.width = static_cast<uint32_t>(width);
            extent.height = static_cast<uint32_t>(height);
            // 3. We make sure values are contained in min and max possible values
            extent.width = std::clamp(extent.width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
            extent.height = std::clamp(extent.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);
            return extent;
        }

    } // _swapchain
} // namespace vulkanhdl