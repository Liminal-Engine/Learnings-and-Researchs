#include "vulkan/device/DeviceWrapper.hpp"

#include <stdexcept>

namespace vulkan::device {

    DeviceWrapper::DeviceWrapper(void)
    :
    _physicalDevice(VK_NULL_HANDLE),
    // _queueFamilies(nullptr),
    _logicalDevice(VK_NULL_HANDLE)
    {}

    DeviceWrapper::DeviceWrapper(const VkInstance &instance)
    :
    _physicalDevice{this->_getPhysicalDevice(instance)}
    {
       
    }

    // Private :
    VkPhysicalDevice DeviceWrapper::_getPhysicalDevice(
        const VkInstance &instance
    ) {
        //1. Enumerate the available devices
        uint32_t nDevice = 0;
        vkEnumeratePhysicalDevices(instance, &nDevice, nullptr);
        if (nDevice == 0) {
            throw std::runtime_error("Failed to find GPUs on the device");
        }
        std::vector<VkPhysicalDevice> foundDevices(nDevice);
        vkEnumeratePhysicalDevices(instance, &nDevice, foundDevices.data());
        //2. Check if at least one physical device is compatible
        for (const VkPhysicalDevice &device : foundDevices) { //1. For each device
            static std::vector<VkQueueFamilyProperties> queueFamiliesProps =
            DeviceWrapper::_STATICgetQueueFamiliesProps(device); //2. List compatible queues family props

            for (uint32_t i = 0; i < queueFamiliesProps.size(); i++) { //3.  For each queue family props
                DeviceWrapper::QueueFamilies queueFamilies{}; 
                
                if (queueFamiliesProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) { //4. If compatible with graphics commands
                    queueFamilies.graphicsFamily = i;
                    this->_queueFamilies = queueFamilies; //Store the compatible queue families for later use
                    return device;
                }
            }
        }
        throw std::runtime_error("Failed to find a compatible GPU\
        on this device");
    }

    //STATIC METHOD
    std::vector<VkQueueFamilyProperties>
    DeviceWrapper::_STATICgetQueueFamiliesProps
    (
        const VkPhysicalDevice &physicalDevice
    ) {
        uint32_t nQueueFamily = 0;
    
        vkGetPhysicalDeviceQueueFamilyProperties(
            physicalDevice,
            &nQueueFamily,
            nullptr
        );
        std::vector<VkQueueFamilyProperties> queueFamiliesProps(nQueueFamily);
        vkGetPhysicalDeviceQueueFamilyProperties(
            physicalDevice,
            &nQueueFamily,
            queueFamiliesProps.data()
        );
        return queueFamiliesProps;
    }

}