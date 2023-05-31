#pragma once
#include <vulkan/vulkan.h>

#include <optional>
#include <vector>

namespace vulkan::device {
    
    class DeviceWrapper {
        public:
            DeviceWrapper(void);
            DeviceWrapper(const VkInstance &instance);
            ~DeviceWrapper();
            void cleanUp(void);

        private:

            struct QueueFamilies {
                std::optional<uint32_t> graphicsFamily;
            };

            //Init methods
            VkPhysicalDevice _pickPhysicalDevice(
                const VkInstance &instance
            );            


            //Get the queue families properties of a given physical device
            static std::vector<VkQueueFamilyProperties>
            _STATIC_getQueueFamiliesProps(
                const VkPhysicalDevice &physicalDevice
            );
           
            VkDevice _createLogicalDevice(
                const VkPhysicalDevice &physicalDevice
            );
            static VkQueue _STATIC_createQueue(
                const VkDevice &logicalDevice,
                uint32_t queueFamilyIndex
            );
            static bool _STATIC_checkDeviceQueueFamiliesSuitability(
                const VkQueueFamilyProperties &deviceQueueFamiliesProps
            );
            static bool _STATIC_checkPhysicalDeviceExtensionsSuitability(
                const VkPhysicalDevice &physicalDevice,
                const std::vector<const char *> &requiredExtensions
            );

            QueueFamilies _queueFamilies;
            VkPhysicalDevice _physicalDevice;
            VkDevice _logicalDevice;
            VkQueue _graphicsQueue;
    };

}
