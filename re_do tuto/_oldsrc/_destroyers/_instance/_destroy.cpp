#include "liminalvulkanwrapper/include/_destroyers/_instance/_destroy.hpp"


#include <vulkan/vulkan.h>

namespace vulkanhdl {
    namespace _destroyers {
        namespace _instance {
            void _destroy(const VkInstance &instance, VkAllocationCallbacks *pAllocator) {
                vkDestroyInstance(instance, pAllocator);
            }

        } // _instance
    } // _destroy
} // vulkanhdl


