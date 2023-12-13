#include "lib/vulkanhdl/include/_loaders/_instance/_load.hpp"

#//TMP ! remove this
#include "vulkanhdl/include/VulkanHdl.hpp"

#include <GLFW/glfw3.h>
#include <stdexcept>

namespace vulkanhdl {
    namespace _loaders {
        namespace _instance {

            /**
             * @brief Load GLFW required extensions
             * 
             * @param nExtensions [uint32_t] A pointer to a uint32_t set to the number of extensions count
             * @return const char ** - The extensions
             */
            static const char **__get_glfw_required_extensions(uint32_t *nExtensions) {
                const char **res = glfwGetRequiredInstanceExtensions(nExtensions);

                if (nExtensions == 0) {
                    throw std::runtime_error("No GLFW extension found");
                }
                return res;
            }

            /**
             * @brief Load a VkInstanceCreateInfo
             * 
             * @param appInfo (VkApplicationInfo *) - A pointer to a VkApplicationInfo to
             * integrate in the final VkInstanceCreateInfo
             * 
             * @return VkInstanceCreateInfo 
             */
            static VkInstanceCreateInfo __load_create_info(VkApplicationInfo *appInfo) {
                VkInstanceCreateInfo createInfo{};
                createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
                createInfo.pApplicationInfo = appInfo;
                // 2.1 Get extensions required by GLFW.
                uint32_t nExtensions = 0;
                const char ** glfwExtensions = __get_glfw_required_extensions(&nExtensions);
                createInfo.enabledExtensionCount = nExtensions;
                createInfo.ppEnabledExtensionNames = glfwExtensions;
                /**
                 * TODO : ENABLE VALIDATION LAYERS IN DEBUG MODE
                 */
                createInfo.enabledLayerCount = 0;
                return createInfo;
            }

            /**
             * @brief Load a VkApplicationInfo
             * 
             * @return VkApplicationInfo 
             */
            static VkApplicationInfo __load_app_info(const std::string &app_name, const std::string &engine_name) {
                VkApplicationInfo appInfo{};

                appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
                appInfo.pApplicationName = app_name.c_str(),
                appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
                appInfo.pEngineName = engine_name.c_str(),
                appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
                appInfo.apiVersion = VK_API_VERSION_1_0;
                appInfo.pNext = nullptr;
                return appInfo;
            }

            VkInstance _load(const std::string &app_name, const std::string &engine_name) {
                if (app_name == "" || engine_name == "")
                    throw std::runtime_error("Parameter errors in vulkanhdl::_loaders::_load_instance");

                // 1. Create appInfo : Optional but provides useful info to the driver in order to optimize.
                VkApplicationInfo app_info = __load_app_info(app_name, engine_name);

                // 2. Optional. Tells Vulan driver which extensions and validation layers we want to use.
               VkInstanceCreateInfo createInfo = __load_create_info(&app_info);

                if (enableValidationLayers) {
                    createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
                    createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
                } else {
                    createInfo.enabledLayerCount = 0;
                }

                // 3. TODO : add validation layers in DEBUG mode ONLY
                VkInstance res;
                if (vkCreateInstance(&createInfo, nullptr, &res) != VK_SUCCESS) {
                    throw std::runtime_error("Error while creating the Vulkan instance");
                }
                return res;
            }

        } // namespace _instance
    } // namespace _loaders
} // namespace vulkanhdl