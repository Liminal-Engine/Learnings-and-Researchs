#include "vulkanhdl/include/_destroy/_surface/_destroy.hpp"

#include <vulkan/vulkan.h>

namespace vulkanhdl {
    namespace _destroyers {
        namespace _surface {

            void _destroy(
                const VkInstance &instance,
                const VkSurfaceKHR &surface, 
                VkAllocationCallbacks *pAllocator 
            ) {
                vkDestroySurfaceKHR(instance, surface, pAllocator);
            }

        } // _surface
    } // _destroy
} // vulkanhdl

