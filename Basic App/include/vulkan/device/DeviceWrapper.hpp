#pragma once
#include <vulkan/vulkan.h>

#include <optional>
#include <vector>

namespace vulkan::device {
    
    class DeviceWrapper {
        public:
            DeviceWrapper(void);
            DeviceWrapper(const VkInstance &instance);

        private:

            struct QueueFamilies {
                std::optional<uint32_t> graphicsFamily;
            };

            //Init methods
            VkPhysicalDevice _getPhysicalDevice(const VkInstance &instance);
            //Get the queue families properties of a given physical device
            static std::vector<VkQueueFamilyProperties>
            _STATICgetQueueFamiliesProps(
                const VkPhysicalDevice &physicalDevice
            );
           
            void _createLogicalDevice(void);

            VkPhysicalDevice _physicalDevice;
            QueueFamilies _queueFamilies;
            VkDevice _logicalDevice;
    };

}
