#include "liminalvulkanwrapper/include/_swapChain/_supports.hpp"

#include <stdexcept>

namespace vulkanhdl {
    namespace _swapchain {

        _Supports _getSupports(
            const VkPhysicalDevice &physicalDevice,
            const VkSurfaceKHR &surface
        ) {
            _Supports res{};

            // 1. Get surface capabilities
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &res._surfaceCapabilities);
            // 2. Get the available surface formats
            // 2.1 Get the total number of available surface formats first
            uint32_t nFormat = 0;
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &nFormat, nullptr);
            if (nFormat == 0)
                throw std::runtime_error("No surface format found for the given device while creating the swap chain");
            // 2.2 Then, create the memory space for the vector of surface formats
            res._surfaceFormats.resize(nFormat);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &nFormat, res._surfaceFormats.data());
            // 3. Get the available swap chain presentation mode
            // 3.1 Get the total number of available wap chain presentation mode first
            uint32_t nPresentationMode = 0;
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &nPresentationMode, nullptr);
            if (nPresentationMode == 0)
                throw std::runtime_error("No presentation mode for the given device while creating the swap chain");
            // 3.2 TThen, create the memory space for the vector of presentation mode
            res._presentationModes.resize(nPresentationMode);
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &nPresentationMode, res._presentationModes.data());
            return res;
        }

    } // namespace _swapchain
} // namespace vulkanhdl