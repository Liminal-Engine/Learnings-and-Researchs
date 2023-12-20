/**
 * @file _VkSurfaceKHRWrapper.hpp
 * @author DE VITA Matteo (matteo.devita7@gmail.com)
 * @brief Wrappers for the VkSurfaceKHR struct
 * @version 0.1
 * @date 2023-12-15
 * 
 * 
 */

#ifndef LIMINAL_LIB_VULKANWRAPPER__SURFACE__VKSURFACEKHR_WRAPPERS_H_
#define LIMINAL_LIB_VULKANWRAPPER__SURFACE__VKSURFACEKHR_WRAPPERS_H_

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace vulkan_wrapper {
    namespace _surface {
        
        /**
         * @brief Loads a VkSurfaceKHR
         * 
         * @param instance (const VkInstance &) The related vulkan instance
         * @param window (GLFWwindow *) The application window
         * @throw
         * @return The created VkSurfaceKHR
         */
        VkSurfaceKHR _load(const VkInstance &instance, GLFWwindow *window);

        /**
         * @brief Destroy a VkSurfaceKHR
         * 
         * @param surface (const VkSurfaceKHR &) The surface to destroy
         * @param instance (const VkInstance &) The related VkInstance
         * @param p_allocator (VkAllocationCallbacks *) (default = nullptr) Vulkan custom allcator
         */
        void _destroy(
            const VkSurfaceKHR &surface,
            const VkInstance &instance,
            VkAllocationCallbacks *p_allocator = nullptr
        );


    } // namespace _surface
} // namespace liminal_vulkan_wrapper 

#endif // LIMINAL_LIB_VULKANWRAPPER__SURFACE__VKSURFACEKHR_WRAPPERS_H_
