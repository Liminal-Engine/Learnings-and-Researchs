#ifndef VULKAN_HDL_LIB__PRIVATE___DESTROY__WINDOW__DESTROY_HPP
#define VULKAN_HDL_LIB__PRIVATE___DESTROY__WINDOW__DESTROY_HPP

/**
 * @file _destroy.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <GLFW/glfw3.h>

namespace vulkanhdl {
    namespace _destroyers {
        namespace _window {

            /**
             * @brief Destroy a GLFW window and terminate GLFW
             * 
             * @param window [GLFWwindow *] The window to destroy
             */
            void _destroy(GLFWwindow *window);
            

        } // _window
    } // _destroy
} // vulkanhdl


#endif // VULKAN_HDL_LIB__PRIVATE___DESTROY__WINDOW__DESTROY_HPP