#ifndef VULKAN_HDL_LIB__PRIVATE___LOADERS__COMMANDBUFFER__LOAD_HPP
#define VULKAN_HDL_LIB__PRIVATE___LOADERS__COMMANDBUFFER__LOAD_HPP

/**
 * @file _load.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-13
 * 
 * @copyright Copyright (c) 2023
 * 
*/

#include <vulkan/vulkan.h>


namespace vulkanhdl {
    namespace _loaders {
        namespace _commandbuffer {

            /**
             * @brief Load a command buffer
             * 
             * @param logicalDevice [const VkDevice &] The related logical device
             * @param commandPool [const VkCommandPool &] The related command pool
             * @return VkCommandBuffer 
             */
            VkCommandBuffer _load(
                const VkDevice &logicalDevice,
                const VkCommandPool &commandPool
            );
            

        } // _commandbuffer
    } // _loaders
} // vulkanhdl


#endif // VULKAN_HDL_LIB__PRIVATE___LOADERS__COMMANDBUFFER__LOAD_HPP