/**
 * @file _VkInstanceWrappers.cpp
 * @author DE VITA Matteo (matteo.devita7@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-15
 * 
 * 
*/

#include "_instance/_VkInstanceWrappers.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace vulkan_wrapper {
    namespace _instance {

        /**
         * @brief Loads the GLFW required extensions
         * 
         * @param n_extensions (uint32_t *) A pointer to a uint32_t set to the number of extensions count
         * @return The required extensions
        */
        static const char **__getGLFWRequiredExtensions(uint32_t *n_extensions) {
            const char **res = glfwGetRequiredInstanceExtensions(n_extensions);

            if (n_extensions == 0) {
                throw std::runtime_error("No GLFW extension found");
            }
            return res;
        }

        /**
         * @brief Loadqs a VkInstanceCreateInfo
         * 
         * @param app_info (VkApplicationInfo *) - A pointer to a VkApplicationInfo to
         * integrate in the final VkInstanceCreateInfo
         * @return The created VkInstanceCreateInfo 
        */
        static VkInstanceCreateInfo __loadCreateInfo(VkApplicationInfo *app_info) {
            VkInstanceCreateInfo create_info{};
            create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            create_info.pApplicationInfo = app_info;
            // 2.1 Get extensions required by GLFW.
            uint32_t n_extensions = 0;
            const char **glfwExtensions = __getGLFWRequiredExtensions(&n_extensions);
            create_info.enabledExtensionCount = n_extensions;
            create_info.ppEnabledExtensionNames = glfwExtensions;
            /**
             * TODO : ENABLE VALIDATION LAYERS IN DEBUG MODE
            */
            create_info.enabledLayerCount = 0;
            return create_info;
        }

        /**
         * @brief Loads a VkApplicationInfo
         * 
         * @param app_name (const std::string &) The application name
         * @param engine_name (const std::string &) The engine name
         * @return The created VkApplicationInfo
        */
        static VkApplicationInfo __loadAppInfo(const std::string &app_name, const std::string &engine_name) {
            VkApplicationInfo res{};

            res.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            res.pApplicationName = app_name.c_str(),
            res.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            res.pEngineName = engine_name.c_str(),
            res.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            res.apiVersion = VK_API_VERSION_1_0;
            res.pNext = nullptr;
            return res;
        }

        VkInstance _load(const std::string &app_name, const std::string &engine_name) {
            if (app_name == "" || engine_name == "")
                throw std::runtime_error("Parameter errors in vulkanhdl::_loaders::_load_instance");

            // 1. Create appInfo : Optional but provides useful info to the driver in order to optimize.
            VkApplicationInfo app_info = __loadAppInfo(app_name, engine_name);

            // 2. Optional. Tells Vulan driver which extensions and validation layers we want to use.
            VkInstanceCreateInfo create_info = __loadCreateInfo(&app_info);

            if (enableValidationLayers) {
                create_info.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
                create_info.ppEnabledLayerNames = VALIDATION_LAYERS.data();
            } else {
                create_info.enabledLayerCount = 0;
            }

            // 3. TODO : add validation layers in DEBUG mode ONLY
            VkInstance res;
            if (vkCreateInstance(&create_info, nullptr, &res) != VK_SUCCESS) {
                throw std::runtime_error("Error while creating the Vulkan instance");
            }
            return res;
        }
        
        void _destroy(const VkInstance &instance, VkAllocationCallbacks *p_allocator) {
            vkDestroyInstance(instance, p_allocator);
        }

    } // vulkan_wrapper
} // _instance
