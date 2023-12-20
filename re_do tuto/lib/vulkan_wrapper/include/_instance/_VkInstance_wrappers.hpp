/**
 * @file _VkInstanceWrappers.hpp
 * @author DE VITA Matteo (matteo.devita7@gmail.com)
 * @brief Wrappers functions for VkInstance struct
 * @version 0.1
 * @date 2023-12-15
 * 
 * 
 */
#ifndef LIMINAL_LIB_VULKANWRAPPER__INSTANCE__VKINSTANCE_WRAPPERS_H_
#define LIMINAL_LIB_VULKANWRAPPER__INSTANCE__VKINSTANCE_WRAPPERS_H_

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

const std::vector<const char*> VALIDATION_LAYERS = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG    
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

namespace vulkan_wrapper {
    namespace _instance {

        /**
         * @brief Loads a vulkan instance
         * 
         * @param app_name (const std::string &) The application name
         * @param engine_name (const std::string &) The engine name
         * @throw std::runtime_error if any error occurs
         * @return The created VkInstance 
         */
        VkInstance _load(const std::string &app_name, const std::string &engine_name);

        /**
         * @brief Destroy a VkInstance
         * 
         * @param instance (const VkInstance &) The instance to destroy
         * @param p_allocator (VkAllocationCallbacks *) (default = nullptr) Vulkan's custom allocator
         */
        void _destroy(const VkInstance &instance, VkAllocationCallbacks *p_allocator = nullptr);
    } // vulkan_wrapper
} // _instance

#endif // LIMINAL_LIB_VULKANWRAPPER__INSTANCE__VKINSTANCE_WRAPPERS_H_
