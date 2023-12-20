#ifndef VULKAN_HDL_LIB__PRIVATE___LOADERS__INSTANCE__LOAD_HPP
#define VULKAN_HDL_LIB__PRIVATE___LOADERS__INSTANCE__LOAD_HPP

/**
 * @file _load.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <vulkan/vulkan.hpp>

#include <string>

namespace vulkanhdl {
    namespace _loaders {
        namespace _instance {
            VkInstance _load(const std::string &app_name, const std::string &engine_name);
        } // namespace _instance
    } // namespace _loaders
} // namespace vulkanhdl


#endif // VULKAN_HDL_LIB__PRIVATE___LOADERS__INSTANCE__LOAD_HPP
