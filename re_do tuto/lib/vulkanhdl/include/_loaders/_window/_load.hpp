#ifndef VULKAN_HDL_LIB__PRIVATE___LOADERS__WINDOW__LOAD_HPP
#define VULKAN_HDL_LIB__PRIVATE___LOADERS__WINDOW__LOAD_HPP

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

#include <GLFW/glfw3.h>

#include <string>

namespace vulkanhdl {
    namespace _loaders {
        namespace _window {

            /**
             * @brief Load a GLFW winow
             * 
             * @param name [const std::string] The window name
             * 
             * @return GLFWwindow* 
             */
            GLFWwindow *_load(const std::string &name);
            

        } // vulkanhdl
    } // _loaders
} // _window


#endif // VULKAN_HDL_LIB__PRIVATE___LOADERS__WINDOW__LOAD_HPP