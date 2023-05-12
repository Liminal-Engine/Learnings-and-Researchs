#include "HelloTriangleApplication.hpp"
#include "utils.hpp"

#include <string.h>

#include <stdexcept>
#include <iostream>
#include <vector>
#include <set>
#include <string>

const std::vector<const char *> VALIDATION_LAYERS = {
    "VK_LAYER_KHRONOS_validation" //all of the useful standard validation is bundled into this layer
};

const std::vector<const char *> DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

HelloTriangleApplication::HelloTriangleApplication(void)
:
_physicalDevice(VK_NULL_HANDLE),
_logicalDevice(VK_NULL_HANDLE)
{}


void HelloTriangleApplication::run(void) {
    this->_initWindow();
    this->_initVulkan();
    this->_mainLoop();
    this->_cleanUp();
}

void HelloTriangleApplication::_initWindow(void) {
    glfwInit(); //init GLFW lib

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // explicilty tells GLFW to NOT use OpenGL context
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //disable resize because we will need to handle it later

    this->_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr); //4th parameter : choosing the monitor, 5th : specific to OpenGL

}

void HelloTriangleApplication::_initVulkan(void) {
    if (enableValidationLayers && UTILS_checkValidationLayerSupport(VALIDATION_LAYERS) == false) {
        throw std::runtime_error("validation layers requested are not available");
    }
    this->_createInstance();
    this->_createSurface();
    this->_pickPhysicalDevice();
    this->_createLogicalDevice();
    this->_createSwapChain();
    this->_createImageViews();
    this->_createGraphicsPipeline();
}

void HelloTriangleApplication::_createInstance(void) {
    VkApplicationInfo appInfo{};

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Liminal";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    appInfo.pNext = nullptr;

    VkInstanceCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    uint32_t glfwExtensionCount = 0;
    //glfw has a method to get the required extensions
    //returns extensions and modify count since we pass a pointer :
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    if (enableValidationLayers == true) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&createInfo, nullptr, &this->_instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vulkan instance");
    }
    //Get the available extensions
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    std::cout << "List of available extensions :" << std::endl;
    for (const auto &extension : extensions) {
        std::cout << "\t" << extension.extensionName << std::endl;
    }
    std::cout << "List of required extensions :" << std::endl;
    for (int i = 0; i < glfwExtensionCount; i++) {
        std::cout << "\t" <<glfwExtensions[i] << std::endl;
    }
}

void HelloTriangleApplication::_createSurface(void) {
    if ( glfwCreateWindowSurface(this->_instance, this->_window, nullptr, &this->_surface) != VK_SUCCESS ) {
        throw std::runtime_error("Failed creating a window surface");
    }
}

void HelloTriangleApplication::_pickPhysicalDevice(void) {
    //1. Enumerate compatible devices and allocate a vector to store them
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(this->_instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support");
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(this->_instance, &deviceCount, devices.data());
    //2. Check if at least one of the compatible devices meets the requirements :
    for (const auto &device : devices) {
        if (UTILS_isDeviceSuitable(device, this->_surface, DEVICE_EXTENSIONS)) {
            this->_physicalDevice = device;
            break;
        }
    }
    //3. Throw error if no devices was found
    if (this->_physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("Failed to find a suitable GPU");
    }
}

void HelloTriangleApplication::_createLogicalDevice(void) {
    //1. Setting up queue create infos, for each queue
    QueueFamilyIndices indices = UTILS_findQueueFamilies(this->_physicalDevice, this->_surface);
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {
        indices.graphicsFamily.value(),
        indices.presentFamily.value()
    };
    float queuePriority = 1.0f; //scheduling command buffer execution
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily; //we tell the logical device which queue family to use
        queueCreateInfo.queueCount = 1; //we tell the nb of queues we want to use for a single queue family
        queueCreateInfo.pQueuePriorities = &queuePriority;
        
        queueCreateInfos.push_back(queueCreateInfo);
    }
    

    //2. Specifying features we want to enable for the device
    VkPhysicalDeviceFeatures deviceFeatures{}; //Leave blank for now as we don't anything special

    //3. Creating the logical device;
    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(DEVICE_EXTENSIONS.size());
    deviceCreateInfo.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    deviceCreateInfo.pNext = nullptr;
    //similar to instance creation :
    if (enableValidationLayers) { //if debug mode
        deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        deviceCreateInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    } else {
        deviceCreateInfo.enabledLayerCount = 0;
    }
    //4. Instanciate logical device by binding it to the physical device and store it
    if (vkCreateDevice(this->_physicalDevice, &deviceCreateInfo, nullptr, &this->_logicalDevice) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device");
    }
    //5. Bind our graphics queue handle to the logical device
    //We can use vkGetDeviceQueue to retrieve queue handles for each queue family.
    //We are using 0 because we only creating a single queue from this family (so index is 0)
    vkGetDeviceQueue(this->_logicalDevice, indices.graphicsFamily.value(), 0, &this->_graphicsQueue);
    vkGetDeviceQueue(this->_logicalDevice, indices.presentFamily.value(), 0, &this->_presentQueue);
}

void HelloTriangleApplication::_createSwapChain(void) {
    //1. Get supported swap chain features
    SwapChainSupports SWSupports = UTILS_querySwapChainSupport(this->_physicalDevice, this->_surface);

    //2. Pick the optimum features
    VkSurfaceFormatKHR bestSurfaceFormat = UTILS_chooseSwapChainSurfaceFormat(SWSupports.surfaceFormats);
    VkPresentModeKHR bestPresentationMode = UTILS_chooseSwapChainPresentMode(SWSupports.presentModes);
    VkExtent2D bestExtent = UTILS_chooseSwapChainExtent(this->_window, SWSupports.surfaceCapabilities);

    //3. Choose the nb of images we would like to have in the swap chain.
    // Choose the min + 1 to not have to wait for the driver to complete internal 
    // operations before we can acquire nother image to render to.
    uint32_t imageCount = SWSupports.surfaceCapabilities.minImageCount + 1;
    //4. Make sur we do not exceeed the max nb of image allowed in the swap chain.
    // Where 0 is a special value meaning that there is no maximum nb of image
    if (SWSupports.surfaceCapabilities.maxImageCount > 0 && imageCount > SWSupports.surfaceCapabilities.maxImageCount) {
        imageCount = SWSupports.surfaceCapabilities.maxImageCount;
    }
    //5. Filling up the swap chain creation struct :
    VkSwapchainCreateInfoKHR SWCreateInfo{};

    SWCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR; //type of create info
    SWCreateInfo.surface = this->_surface; //surface tied to this swap chain
    SWCreateInfo.minImageCount = imageCount;
    SWCreateInfo.imageFormat = bestSurfaceFormat.format;
    SWCreateInfo.imageColorSpace = bestSurfaceFormat.colorSpace;
    SWCreateInfo.imageExtent = bestExtent;
    SWCreateInfo.imageArrayLayers = 1; //Nb if layer each image consits of. Always 1 unless we are developing a stereoscopic 3D app.
    //Specifies what kind of operations we will use the images in the swap chain for. We are going to render directly to them so we use
    //VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT. If we wanted to render images separatly to perform operations on them before we could use
    //VK_IMAGE_USAGE_TRANSFER_DST_BIT  and use a memory operation to transfer the renderd image to a swap chain image.
    SWCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    //We can specify pre transform operations such as 90 degress rotation if supported. If we don't want, use currentTransform.
    SWCreateInfo.preTransform = SWSupports.surfaceCapabilities.currentTransform;
    //"compositeAlpha" specifies if the alpha channel shuld be use for blending with other windows in the windows system.
    //In most cases, you'll want to ignore the alpha channel, hence, use VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR
    SWCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    SWCreateInfo.presentMode = bestPresentationMode;
    //if "clipped" set to VK_TRUE, it means we don't care about the color of pixels that are obscured , by other windows for example.
    //You'll get the best performances by enbling clipping
    SWCreateInfo.clipped = VK_TRUE;
    //With Vulkan, it is possible that a swap chain becomes invalid or unoptimized, because the window has been resized for axample.
    //In that case, swap chain needs to be recreated ffrom sratch adn a reference to the old one must be passed. This is a complex topic
    //and not our case for now so lmet's just set it to VK_NULL_HANDLE
    SWCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    /*
        6. Bind the the corresponding queue
        If the graphics queue differs from the presentation queue, our images will be used across different queus. We will be drawing in
        the swap chain from the graphics queue and then submitting them on the presentation queue. There are 2 ways to handle images that are
        accessed from multiple queues :
        - VK_SHARING_MODE_EXCLUSIVE : an image is owned by one queue family at a time and ownership must be explicitly transferred before
        using it in another queue family. OFFERS THE BEST PERFORMANCE
        - VK_SHARING_MODE_CONCURRENT : an image can be used across multiple queue families without explicit ownership transfers.
        Concurrent mode requires to specify in advance which queue families ownership will be shared using "queueFamilyIndexCount" and 
        "pQueueFamilyIndices" parameters. If the graphics queue family and presentation queue family are the same, whichc is the case on
        most hardware, we should stick to exclusive mode, because concurrent mode requires you to specify at least 2 distinct queue
        families
    */
    QueueFamilyIndices indices = UTILS_findQueueFamilies(this->_physicalDevice, this->_surface);
    uint32_t QueueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        SWCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        SWCreateInfo.queueFamilyIndexCount = 2;
        SWCreateInfo.pQueueFamilyIndices = QueueFamilyIndices;
    } else {
        SWCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        SWCreateInfo.queueFamilyIndexCount = 0; //optional
        SWCreateInfo.pQueueFamilyIndices = QueueFamilyIndices; //optional
    }
    if ( vkCreateSwapchainKHR(this->_logicalDevice, &SWCreateInfo, nullptr, &this->_swapChain) != VK_SUCCESS ) {
        throw std::runtime_error("Failed to create swap chain");
    }

    //Retrieving the swap chain images
    vkGetSwapchainImagesKHR(this->_logicalDevice, this->_swapChain, &imageCount, nullptr);
    this->_swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(this->_logicalDevice, this->_swapChain, &imageCount, this->_swapChainImages.data());


    this->_swapChainImageFormat = bestSurfaceFormat.format;
    this->_swapChainExtent = bestExtent;
}

void HelloTriangleApplication::_createImageViews(void) {
    //1. Resize to the nb of images in the swap chain
    this->_swapChainImageViews.resize(this->_swapChainImages.size());

    //2. FOR EACH swap chain images, create an image view using a VkImageViewCreateInfo struct
    for (size_t i = 0; i < this->_swapChainImages.size(); i++) {
        VkImageViewCreateInfo createInfo{};

        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = this->_swapChainImages[i];
        //"viewType" and "format" specifies how the image data should be interpreted.
        //"viewType" allows to treat images as 1D, 2D or 3D textures as well as cube maps
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = this->_swapChainImageFormat;
        //"components" allows to swizzle the color channles around. Example : we can map all of
        //the channles to the red channel for a monochrome texture. We can also map constant values of
        //0 and 1 to a channel. We will stick to the default mapping :
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        //"subresourceRange" describes the purpose of the image and which part of it should be accessed.
        //our image will be used as color targets without any mipmapping lvls or multiple layers
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        
        if (vkCreateImageView(this->_logicalDevice, &createInfo, nullptr, &this->_swapChainImageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create image view");
        }

    }
}

void HelloTriangleApplication::_createGraphicsPipeline(void) {

}


void HelloTriangleApplication::_mainLoop(void) {
    while (glfwWindowShouldClose(this->_window) == false) { // poll events while window has not been ordered to close by the user
        glfwPollEvents();
    }
}

void HelloTriangleApplication::_cleanUp(void) {
    for (auto imageView : this->_swapChainImageViews) {
        vkDestroyImageView(this->_logicalDevice, imageView, nullptr);
    }
    vkDestroySwapchainKHR(this->_logicalDevice, this->_swapChain, nullptr);
    vkDestroyDevice(this->_logicalDevice, nullptr);
    vkDestroySurfaceKHR(this->_instance, this->_surface, nullptr);
    vkDestroyInstance(this->_instance, nullptr); //destroy vulkan instance    
    glfwDestroyWindow(this->_window); //destroy window
    glfwTerminate(); //terminate GLFW
}