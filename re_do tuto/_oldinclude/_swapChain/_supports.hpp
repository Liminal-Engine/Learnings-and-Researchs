#ifndef VULKAN_HDL_LIB__PRIVATE___SWAPCHAIN__SWAPCHAINSUPPORT_HPP
#define VULKAN_HDL_LIB__PRIVATE___SWAPCHAIN__SWAPCHAINSUPPORT_HPP

/**
 * @file _swapchainsupports.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @date 2023-12-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <optional>
#include <stdint.h>
#include <set>
#include <unordered_map>
#include <string>
#include <vector>

namespace vulkanhdl {
    namespace _swapchain {
        /**
         * @brief _SwapChainSupports Stores the swap chain supported features
        */
        struct _Supports {
            VkSurfaceCapabilitiesKHR _surfaceCapabilities; ///< Various paremeters about related to the surface such as min/max number of images in the swap chain, min/max width and height of the image
            std::vector<VkSurfaceFormatKHR> _surfaceFormats; ///< Surface formats = pixel format and color space (does'nt include iomage width and height since already in VkSurfaceCapabilitiesKHR)
            std::vector<VkPresentModeKHR> _presentationModes; ///< Represents the mode in which the image are presented in the swap chain (immediate, FIFO, mailbox, etc...) Each mode has its own way of working
        };

        /**
         * @brief Get the swap chain supports for a given physical device
         * 
         * @param physicalDevice 
         * @param surface 
         * @return _SwapChainSupports 
         */
        _Supports _getSupports(
            const VkPhysicalDevice &physicalDevice,
            const VkSurfaceKHR &surface
        );
    } // _swapchain
} // namespace vulkanhdl

#endif // VULKAN_HDL_LIB__PRIVATE___SWAPCHAIN__SWAPCHAINSUPPORT_HPP
