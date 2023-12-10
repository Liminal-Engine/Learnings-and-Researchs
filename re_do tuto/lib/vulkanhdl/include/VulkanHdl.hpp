#ifndef VULKAN_HDL_LIB_VULKANHDL_HPP
#define VULKAN_HDL_LIB_VULKANHDL_HPP

/**
 * @file VulkanHdl.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "vulkanhdl/include/_queues/_queues.hpp"
#include "vulkanhdl/include/_swapChain/_swapchainsupports.hpp"


#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <string>

namespace vulkanhdl {

    /**
     * @brief Handler for teh Vulkan instance of the applcation
     * 
     */
    class VulkanHdl {
        public:
            /**
             * @brief Default constructor
            */
            VulkanHdl(void);

        private:
            const std::string _appName; ///< Name of the application
            const std::string _engineName; ///< Name of the engine
            GLFWwindow *_window; ///< The GLFW window related to the vulkan instace. This will be taken out later in a separate lib
            VkInstance _instance; ///< The Vulkan instance
            VkSurfaceKHR _surface; ///< A surface is an interface between Vulkan and the windowing system
            VkPhysicalDevice _physicalDevice; ///< The choosen physical device
            _swapchain::_SwapChainSupports _swapChainSupports; ///< swap chain supported features for the related physical device
            _queues::_QueueFamilies _queueFamilies; ///< The physical device queue families
            VkDevice _logicalDevice; ///< Interface between Vulkan and a physical device
            _queues::_QueueHandlers_t _queueHandlers; ///< Queue handlers as specified in the config file
            VkSwapchainKHR _swapChain; ///< The swap chain (e.g.) basically the queue for the images to draw

    };
}

#endif // VULKAN_HDL_LIB_VULKANHDL_HPP
