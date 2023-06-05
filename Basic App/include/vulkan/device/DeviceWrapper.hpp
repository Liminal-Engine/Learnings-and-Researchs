#pragma once
#include <vulkan/vulkan.h>

#include <optional>
#include <vector>

// #include "vulkan/swap_chain/SwapChainWrapper.hpp"

#include "vulkan/global_structs.hpp"

namespace vulkan::device {

    class DeviceWrapper {
        public:
            DeviceWrapper(void);
            DeviceWrapper(
                const VkInstance &instance, const VkSurfaceKHR &surface
            );
            ~DeviceWrapper();

            

            //Getters
            VkPhysicalDevice getPhysicalDevice(void) const;
            VkDevice getLogicalDevice(void) const;
            VkQueue getGraphicsQueue(void) const;
            VkQueue getPresentQueue(void) const;
            
            vulkan::SwapChainSupportDetails
            getDeviceSwapChainSupports(void) const;
            
            vulkan::QueueFamilies getDeviceQueueFamilies(void) const;

            void cleanUp(void);

        private:

            //Init methods
            VkPhysicalDevice _pickPhysicalDevice(
                const VkInstance &instance, const VkSurfaceKHR &surface
            );            
            static vulkan::QueueFamilies _STATIC_getQueueFamiliesFromProps(
                const std::vector<VkQueueFamilyProperties> &queueFamiliesProps,
                const VkPhysicalDevice &physicalDevice,
                const VkSurfaceKHR &surface
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
            static bool _STATIC_checkPhysicalDeviceExtensionsSuitability(
                const VkPhysicalDevice &physicalDevice,
                const std::vector<const char *> &requiredExtensions
            );

            
            QueueFamilies _queueFamilies;

            vulkan::SwapChainSupportDetails _deviceSwapChainSupportDetails;
        
            VkPhysicalDevice _physicalDevice;
            VkDevice _logicalDevice;
            VkQueue _graphicsQueue; 
            VkQueue _presentQueue;
    };

}
