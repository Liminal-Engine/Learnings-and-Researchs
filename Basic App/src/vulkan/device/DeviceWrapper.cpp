#include "vulkan/device/DeviceWrapper.hpp"
#include "vulkan/settings_consts.hpp"
#include "vulkan/swap_chain/SwapChainWrapper.hpp"

#include <stdexcept>
#include <set>
#include <string>

#include "vulkan/device/DeviceWrapper.hpp"

namespace vulkan::device {

    //Default constructor
    DeviceWrapper::DeviceWrapper(void)
    :
    _physicalDevice(VK_NULL_HANDLE),
    // QueueFamilies(nullptr),
    _logicalDevice(VK_NULL_HANDLE),
    _queueFamilies(QueueFamilies{}),
    _graphicsQueue(VK_NULL_HANDLE),
    _presentQueue(VK_NULL_HANDLE)
    {}

    //Constructor
    DeviceWrapper::DeviceWrapper(
        const VkInstance &instance, const VkSurfaceKHR &surface
    )
    :
    // QueueFamilies(QueueFamilies{}),
    _physicalDevice{this->_pickPhysicalDevice(instance, surface)}, //this->QueueFamilies will be created in this method
    _logicalDevice{this->_createLogicalDevice(this->_physicalDevice)},
    _graphicsQueue{
        this->_STATIC_createQueue(
            this->_logicalDevice,
            this->_queueFamilies.graphicsFamily.value()
        )
    },
    _presentQueue{
        this->_STATIC_createQueue(
            this->_logicalDevice,
            this->_queueFamilies.presentFamily.value()
        )
    }
    {}

    //Destructor
    DeviceWrapper::~DeviceWrapper(void) {
    }

    //Getters

    vulkan::QueueFamilies
    DeviceWrapper::getDeviceQueueFamilies(void) const {
        return this->_queueFamilies;
    }

    vulkan::SwapChainSupportDetails
    DeviceWrapper::getDeviceSwapChainSupports(void) const {
        return this->_deviceSwapChainSupportDetails;
    }

    VkQueue DeviceWrapper::getGraphicsQueue(void) const {
        return this->_graphicsQueue;
    }

    VkQueue DeviceWrapper::getPresentQueue(void) const {
        return this->_presentQueue;
    }


    VkPhysicalDevice DeviceWrapper::getPhysicalDevice(void) const {
        return this->_physicalDevice;
    }

    VkDevice DeviceWrapper::getLogicalDevice(void) const {
        return this->_logicalDevice;
    }


    void DeviceWrapper::cleanUp(void) {
        vkDestroyDevice(this->_logicalDevice, nullptr);
    }


    // Private :

    //Pick an existing device to use from the computer
    VkPhysicalDevice DeviceWrapper::_pickPhysicalDevice(
        const VkInstance &instance, const VkSurfaceKHR &surface
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
            if (this->_STATIC_checkPhysicalDeviceExtensionsSuitability( //If device extensions are supported
                device,
                REQUIRED_DEVICE_EXTENSIONS
            ) == true) {
                std::vector<VkQueueFamilyProperties> queueFamiliesProps =
                DeviceWrapper::_STATIC_getQueueFamiliesProps(device); //2. List compatible queues family props

                // for (uint32_t i = 0; i < queueFamiliesProps.size(); i++) { //3.  For each queue family props
                    vulkan::QueueFamilies queueFamilies =
                    this->_STATIC_getQueueFamiliesFromProps(
                        queueFamiliesProps, device, surface
                    ); //Get queue families for this device
                    vulkan::SwapChainSupportDetails swapChainSupports =
                    vulkan::swap_chain::SwapChainWrapper::
                    STATIC_querySupport(device, surface); //Get swap chain supports for this device
                    
                    if (
                        queueFamilies.isComplete() &&
                        swapChainSupports.formats.empty() == false &&
                        swapChainSupports.presentModes.empty() == false
                    ) {
                        this->_queueFamilies = queueFamilies; //Store the compatible queue families for later use
                        this->_deviceSwapChainSupportDetails = swapChainSupports; //Store the compatible swap chain supports
                        return device;
                    }
                // }
            }
        }
        throw std::runtime_error("Failed to find a compatible GPU \
on the computer");
    }

    //Get the device queue families related properties
    std::vector<VkQueueFamilyProperties>
    DeviceWrapper::_STATIC_getQueueFamiliesProps
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

    //Create the _logicalDevice member
    VkDevice DeviceWrapper::_createLogicalDevice(
        const VkPhysicalDevice &physicalDevice
    ) {        
        //TODO : define this logical device creation in details in notion
        //1. Specify the queue for the device
        float queuePriority = 1.0f;
        VkDeviceQueueCreateInfo queueCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,            
            .queueFamilyIndex = this->_queueFamilies.graphicsFamily.value(),
            .queueCount = 1,
            .pQueuePriorities = &queuePriority //influence the schdeluling of command buffer execution
        };
        //2. Specify the device features, empty for now
        VkPhysicalDeviceFeatures features = {};
        //3. Logical device creation
        VkDevice logicalDevice{};
        VkDeviceCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = nullptr,
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &queueCreateInfo,
            .enabledLayerCount = 0,
            .enabledExtensionCount = static_cast<uint32_t>(REQUIRED_DEVICE_EXTENSIONS.size()),
            .ppEnabledExtensionNames = REQUIRED_DEVICE_EXTENSIONS.data(),
            .pEnabledFeatures = &features,

        };
        if (ENABLE_VALIDATION_LAYERS) {
            createInfo.enabledLayerCount = LAYER_COUNT;
            createInfo.ppEnabledLayerNames = LAYER_NAMES;
        }
        if (vkCreateDevice(
            physicalDevice,
            &createInfo,
            nullptr,
            &logicalDevice
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create logical device");
        }
        return logicalDevice;
    }

    //Create a queue, for example the _graphicsQueue member
    VkQueue DeviceWrapper::_STATIC_createQueue(
        const VkDevice &logicalDevice,
        uint32_t queueFamilyIndex
    ) {
        VkQueue queue;

        vkGetDeviceQueue(logicalDevice, queueFamilyIndex, 0, &queue);
        return queue;
    }

    //get queue families from all queue famlilies props
    vulkan::QueueFamilies DeviceWrapper::_STATIC_getQueueFamiliesFromProps(
        const std::vector<VkQueueFamilyProperties> &queueFamiliesProps,
        const VkPhysicalDevice &physicalDevice,
        const VkSurfaceKHR &surface
    ) {
        vulkan::QueueFamilies queueFamilies{};

        VkBool32 presentSupport = false;

        for (
            uint32_t i = 0;
            i < queueFamiliesProps.size() || queueFamilies.isComplete() == false;
            i++
        ) { //3.  For each queue family props
            if (presentSupport == false) {
                vkGetPhysicalDeviceSurfaceSupportKHR(
                    physicalDevice, i, surface, &presentSupport
                );
                if (presentSupport == true) {
                    queueFamilies.presentFamily = i;                
                }
            }
            if (queueFamilies.graphicsFamily.has_value() == false) {
                if (queueFamiliesProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) { //graphics commands compatibility                    
                    queueFamilies.graphicsFamily = i;
                }
            }
        }
        return queueFamilies;
    }

    //Check if a physical device meets the required extensions
    bool DeviceWrapper::_STATIC_checkPhysicalDeviceExtensionsSuitability(
        const VkPhysicalDevice &physicalDevice,
        const std::vector<const char *> &requiredExtensions
    ) {
        //1. Get tghe device available extensions
        uint32_t nAvailableExtensions = 0;
        vkEnumerateDeviceExtensionProperties(
            physicalDevice,
            nullptr,
            &nAvailableExtensions,
            nullptr
        );
        std::vector<VkExtensionProperties>
        availableExtensions(nAvailableExtensions);
        vkEnumerateDeviceExtensionProperties(
            physicalDevice,
            nullptr,
            &nAvailableExtensions,
            availableExtensions.data()
        );
        //2. Create a set from the required extensions to erase duplicates
        std::set<std::string> setRequiredExtensions(
            requiredExtensions.begin(),
            requiredExtensions.end()
        );
        //3. We erase values from the set everytime we encouter them. That way, if the set is empty, all required extensions are met
        for (const VkExtensionProperties &extProps : availableExtensions) {
            setRequiredExtensions.erase(extProps.extensionName);
        }
        return setRequiredExtensions.empty();
    }
}
