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

void HelloTriangleApplication::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
    /*We always begin recording a command buffer by calling "vkBeginCommandBuffer" with a "vkCommandBufferBeginInfo" struct
    that specifies some details about the usage of this specific command buffer*/
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    /*The "flags" parameter spcifies how we are going to use the command buffer. The folowing values are available :
    - VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT : The command buffer will be rerecorded right after executing it once
    - VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT : This is a secondary command buffer that will be entirely within a single render pass
    - VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT : The command buffer can be resubmitted while it is also already pending execution
    None of theses flags are applicable for us right now.*/
    beginInfo.flags = 0; //Optional
    /*Specifies which state to inherit from the calling primary command buffers. Only relevant for secondary command buffers.*/
    beginInfo.pInheritanceInfo = nullptr; //Optional

    /*If command buffer already recorded, calling "vkBeginCommandBuffer will implicitly reset it. It's not possible to append commands to a buffer at a later time."*/
    if ( vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS ) {
        throw std::runtime_error("Failed to begin recording a command buffer");
    }
    /*From here we will start the render pass using the "vkCmdBeginRenderPass" function which takes a "VkRenderPassBeginInfo" struct*/
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = this->_renderPass; //bind render pass
    renderPassInfo.framebuffer = this->_swapChainFrameBuffers[imageIndex]; //bind frame buffer
    /*Defines the size of the render area. The render area is where shader loads and stores will take place. The pixels outside this region
    will have undefined values. It shuld match the size of the attachments for best performance, which is our case.*/
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = this->_swapChainExtent;
    /*Define the clear values to use for VK_ATTACHMENT_LOAD_OP_CLEAR, which we used as load operation for the color attachment.
    Set to black with 100% opacity.*/
    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f }}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;
    /*We fnially begin the render pass. All of the functions that record commands can be recognized by theyr "vkCmd" prefix.
    They return void so no error handling.
    The 3rd parameter controls how the drawinf command within the rendere pass will be provided. It can be 1 of 2 values :
    - VK_SUBPASS_CONTENTS_INLINE : The render pass commands will be embedded in the primary command buffer itself and no secondary command buffers will be executed.
    - VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS : The render pass commands will be executed from secondary command buffers.
    We won't be using secondary command buffers, so we'll go with the first option*/
    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    /*Basic drawing commands*/
    //We can now bind the graphics pipeline. 2nd parameter tells Vulkan if it is a graphics or compute pipeline.
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->_graphicsPipeline);
    /*When we defined our fixed functions, we did specify viexport and scissor state for this pipeline to be dynamic.
    So we need to set them in the command buffer before issuing our draw command.*/
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(this->_swapChainExtent.width);
    viewport.height = static_cast<float>(this->_swapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = this->_swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    /*Now we are ready to issue the draw command (for the triangle in our case).
    The actual "vkCmdDraw" function is a bit anticlimactic (causing disappointment at the end of an exciting or impressive series of events).
    It is a simple function because we specified all the information in advance. It has the following parameters, aside from the command buffer :
    - "vertexCount": Even though we don't have a vertex buffer, we technically still have 3 vertices to draw.
    - "instanceCount": Used for instance rendering, use 1 if you're not doing that.
    - "firstVertex": Used as an offset into the vertex buffer, defines the lowest value of "gl_VertexIndex" in our shaders.
    - "firstInstance": Used as an offset for instanced rendering, defines the lowest value of "gl_InstanceIndex" in our shaders.
    */
    vkCmdDraw(commandBuffer, 3, 1, 0, 0);
    /*Finishing up*/
    //Ending the render pass :
    vkCmdEndRenderPass(commandBuffer);
    //We've finished recording the command buffer :
    if ( vkEndCommandBuffer(commandBuffer) != VK_SUCCESS ) {
        throw std::runtime_error("Failed to end command buffer recording");
    }
}

void UTILS_recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
    /*We always begin recording a command buffer by calling "vkBeginCommandBuffer" with a "vkCommandBufferBeginInfo" struct
    that specifies some details about the usage of this specific command buffer*/
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    /*The "flags" parameter spcifies how we are going to use the command buffer. The folowing values are available :
    - VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT : The command buffer will be rerecorded right after executing it once
    - VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT : This is a secondary command buffer that will be entirely within a single render pass
    - VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT : The command buffer can be resubmitted while it is also already pending execution
    None of theses flags are applicable for us right now.*/
    beginInfo.flags = 0; //Optional
    /*Specifies which state to inherit from the calling primary command buffers. Only relevant for secondary command buffers.*/
    beginInfo.pInheritanceInfo = nullptr; //Optional

    /*If command buffer already recorded, calling "vkBeginCommandBuffer will implicitly reset it. It's not possible to append commands to a buffer at a leter time."*/
    if ( vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS ) {
        throw std::runtime_error("Failed to begin recording a command buffer");
    }
    
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
    this->_createRenderPass();
    this->_createGraphicsPipeline();
    this->_createFrameBuffers();
    this->_createCommandPool();
    this->_createCommandBuffer();
    this->_createSyncObjects();
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
            throw std::runtime_error("Failed to create image view.");
        }
    }
}

void HelloTriangleApplication::_createGraphicsPipeline(void) {
    std::vector<char> vertShaderCode = UTILS_readFile("./shaders/shader.vert.spv");
    std::vector<char> fragShaderCode = UTILS_readFile("./shaders/shader.frag.spv");

    VkShaderModule vertShaderModule = UTILS_createShaderModule(vertShaderCode, this->_logicalDevice);
    VkShaderModule fragShaderModule = UTILS_createShaderModule(fragShaderCode, this->_logicalDevice);


    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT; //telling which pipeline stage it is
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    /*
        There is one more (optional) member, pSpecializationInfo, which we won't be using here,
        but is worth discussing. It allows you to specify values for shader constants.
        You can use a single shader module where its behavior can be configured at
        pipeline creation by specifying different values for the constants used in it.
        This is more efficient than configuring the shader using variables at render time,
        because the compiler can do optimizations like eliminating if statements that
        depend on these values. If you don't have any constants like that, then you
        can set the member to nullptr, which our struct initialization does automatically.
    */

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {
        vertShaderStageInfo,
        fragShaderStageInfo,
    };

    /*1. Vertex input : describes the format of the vertex data that will be passed
    to the vertex shader :
        1. Bindings : Spacing between data and wheter the data is per-vertex or per-instance
        2. Attribute descriptions : type of the attributes passed to the vertex shader,
        which binding to load them from and which offset.
    As of now, we are hard coding the vertex data directly in the vertex shader, so we'll configure
    the struct to specifyt that there is no vertex data for now.*/
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr; //Optional
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr; //Optional
    /*"pVertexBindingDescriptions" and "pVertexAttributeDescriptions"
    are pointers to structs that described what is said in the previous comment*/

    /*2. Input assembly : describes what kind of geometry will be drawn from the vertices
    and if primitive restart should be enabled. So the "topology" member can have values like :
    - VK_PRIMITIVE_TOPOLOGY_POINT_LIST : points from vertices
    - VK_PRIMITIVE_TOPOLOGY_LINE_LIST : line from every 2 vertices without reuse.
    - VK_PRIMITIVE_TOPOLOGY_LINE_STRIP : ernd vertex of everyline is used as start vertex for next line
    - VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST : triangle from every 3 vertices without reuse
    - VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP : the last two vertices of each triangle is used as the first two for the next one
    Normally, the vertices are loaded from the vertex buffer by index in sequential order, but with an element
    buffer you can specify the indices to use yourself. This allows you to perform optimizations like
    reusing vertices. If you set the primitiveRestartEnable member to VK_TRUE, then it's possible to break up 
    lines and triangles in the _STRIP topology modes by using a special index of 0xFFFF or 0xFFFFFFFF.
    */
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo{};
    inputAssemblyStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyStateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyStateInfo.primitiveRestartEnable = VK_FALSE;

   /*3. Viewports : Describes the region of the framebuffer that the output will be rendered to.*/
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) this->_swapChainExtent.width;
    viewport.height = (float) this->_swapChainExtent.height;
    viewport.minDepth = 0.0f; //specify range of depth values for the framebuffer
    viewport.maxDepth = 1.0f;

    /*4. Scissor : Describes in which regions pixels will actually be stored.
    Any pixel outside the scissor rectangles will be discarded by the rasterizer. :
    https://vulkan-tutorial.com/images/viewports_scissors.png*/
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = this->_swapChainExtent;

    /*5. Viewport state : contains : viewport and scissor*/
    VkPipelineViewportStateCreateInfo viewportStateInfo{};
    viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateInfo.viewportCount = 1;
    viewportStateInfo.pViewports = &viewport;
    viewportStateInfo.scissorCount = 1;
    viewportStateInfo.pScissors = &scissor;

    /*6. Rasterizer : takes geomtry shaped by the vertices from vertex shader and turns it into
    fragments to be colored by the fragment shader.
    Additionnaly, it performs depth testing, face culling and the scissor test. It can be configured to
    output fragments that fill entire polygons or just edges (wireframe rendering)*/

    VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
    rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationInfo.depthClampEnable = VK_FALSE; //fragments beyond the near and far planes are campled instead of discarded
    rasterizationInfo.rasterizerDiscardEnable = VK_FALSE; //IF SET TO VK_TRUE  : geometry never passes trhough the rasterizer stage.
    /*polygonMode can take 3 values 
    - VK_POLYGON_MODE_FILL fill the area of the polygon with fragments
    - VK_POLYGON_MODE_LINE : polygon edges are drawn as lines
    - VK_POLYGON_MODE_POINT : polygon vertices are drawn as points
    Any other mode requires enabling GPU feature
    */
    rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL; 
    rasterizationInfo.lineWidth = 1.0f; //thickness of lines in nb of fragments. Thicker than 1.0f requires GHPU feature.
    rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT; //type of face culling : disabled, front faces, back faces or both
    rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE; //vertex order for faces to be considered front-facing : clockwise or counterclockwise*
    rasterizationInfo.depthBiasEnable = VK_FALSE;
    rasterizationInfo.depthBiasConstantFactor = 0.0f; // Optional
    rasterizationInfo.depthBiasClamp = 0.0f; // Optional
    rasterizationInfo.depthBiasSlopeFactor = 0.0f; // Optional

    /*7. Multisampling : one of the way to perform anti-alisaing*, it works by combining the fragment shader results of
    multiple polygons that rasterize to the same pixel. Because it doesn't need to run the fragment shader multiple times
    if only one polygon maps to a pixel, it is significantly less expensive tham renering to a higher resolution and then
    downscaling. Enabling it required enabling GPU feature.
    For now, we will keep it disable.*/
    VkPipelineMultisampleStateCreateInfo multisampleInfo{};
    multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleInfo.sampleShadingEnable = VK_FALSE;
    multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampleInfo.minSampleShading = 1.0f; //Optional
    multisampleInfo.pSampleMask = nullptr; // Optional
    multisampleInfo.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampleInfo.alphaToOneEnable = VK_FALSE; // Optional

    /*8. Color blending : After a fragment shader has returned a color, it needs to be combined with the color that is already
    in the framebuffer. This transformation is known as color blending and there are 2 ways to do it :
        1. Mix the old and new value to produce a final color
        2. Combine the old and new value using a bitwise operation
    "VkPipelineColorBlendAttachmentState" : configuration per attached framebuffer
    "VkPipelineColorBlendStateCreateInfo" : GLOBAL color blending settings
    We only have on eframebuffer so we only need one VkPipelineColorBlendAttachmentState
    If "blendEnable" is VK_FALSE, the new color from the fragment shader is passed through unmodified. Otherwise
    mixiing operation appens
    */
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE; // Optional
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA; // Optional
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA; // Optional
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

    VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
    colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendInfo.logicOpEnable = VK_FALSE;
    colorBlendInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlendInfo.attachmentCount = 1;
    colorBlendInfo.pAttachments = &colorBlendAttachment;
    colorBlendInfo.blendConstants[0] = 0.0f; // Optional
    colorBlendInfo.blendConstants[1] = 0.0f; // Optional
    colorBlendInfo.blendConstants[2] = 0.0f; // Optional
    colorBlendInfo.blendConstants[3] = 0.0f; // Optional
    /*If you preffer to use the second method of blending (bitwise combination), you must set "logicOpEnable" to
    VK_TRUE and set "logicOp". Doing so will automatically disable the first method.*/

    /*9. ¨Pipeline layout"
    You can use "uniform" values in shader with "VkPipelineLayout", similar to global variables and can be changed at drawing time to change
    shader behavior without having to recreate them*/
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0; // Optional
    pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional //other way of passing dynamic values
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

    /*While most of the pipeline state needs to be baked into the pipeline state,
    a limited amount of the state can actually be changed without recreating the
    pipeline at draw time. Examples are the size of the viewport, line width and blend constants.
    If you want to use dynamic state and keep these properties out, then you'll have to fill in a
    VkPipelineDynamicStateCreateInfo structure like this:*/
    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();
    
    if (vkCreatePipelineLayout(this->_logicalDevice, &pipelineLayoutInfo, nullptr, &this->_pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create pipeline layout.");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    //Here is where we actually bind all the created stages to the pipeline :
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssemblyStateInfo;
    pipelineInfo.pViewportState = &viewportStateInfo;
    pipelineInfo.pRasterizationState = &rasterizationInfo;
    pipelineInfo.pMultisampleState = &multisampleInfo;
    pipelineInfo.pDepthStencilState = nullptr; // Optional
    pipelineInfo.pColorBlendState = &colorBlendInfo;
    pipelineInfo.pDynamicState = &dynamicState;
    //then, we reference the pipeline layout which is a Vulkan handle rather than a struct pointer :
    pipelineInfo.layout = this->_pipelineLayout;
    ////Reference to the render oass abd the index of the subpass where this graphics pipeline will be used.
    pipelineInfo.renderPass = this->_renderPass;
    pipelineInfo.subpass = 0;
    /*Vulkan allow you to create a new graphics pipeline by deriving from an existing pipeline
    The idea of pipeline derivatives is that it is less expensive to set up pipelines when they have much
    functionality in common with an existing pipeline and switching between pipelines from the same
    parent can also be done quicker.
    These values are only used if the VK_PIPELINE_CREATE_DERIVATIVE_BIT flag is also specified in
    the flags field of VkGraphicsPipelineCreateInfo.*/
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optional

    //Create the graphics pipeline :
    /*The function is actually designed to take multiple VkGraphicsPipelineCreateInfo objects and create multiple
    VkPipeline objects in a single call. The second parameter, for which we've passed the VK_NULL_HANDLE argument,
    references an optional VkPipelineCache object. A pipeline cache can be used to store and reuse data relevant
    to pipeline creation across multiple calls to vkCreateGraphicsPipelines and even across program executions
    if the cache is stored to a file. This makes it possible to significantly speed up pipeline creation at
    a later time.*/
    // if ( vkCreateGraphicsPipelines(this->_logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &this->_graphicsPipeline) != VK_SUCCESS) {
    //     throw std::runtime_error("Failed to create graphics pipeline");
    // }

    if (vkCreateGraphicsPipelines(this->_logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &this->_graphicsPipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }
    vkDestroyShaderModule(this->_logicalDevice, vertShaderModule, nullptr);
    vkDestroyShaderModule(this->_logicalDevice, fragShaderModule, nullptr);
}

void HelloTriangleApplication::_createRenderPass(void) {
    // We will have only 1 color buffer attachment represented by one of the images from the swap chain
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = this->_swapChainImageFormat; // must match the format of the swap chain
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;// we are not multisampling yet, so stick to 1 sample.
    /*"loadOp" and "storeOp" determine what to do with the data in the attachment before and after rendering.
    "loadOp" can be :
        - VK_ATTACHMENT_LOAD_OP_LOAD: Preserve the existing contents of the attachment
        - VK_ATTACHMENT_LOAD_OP_CLEAR: Clear the values to a constant at the start
        - VK_ATTACHMENT_LOAD_OP_DONT_CARE: Existing contents are undefined, we don't care about them
    "storeOp" can be :
        - VK_ATTACHMENT_STORE_OP_STORE: Rendered contents will be stored in memory and can be read later.
        - VK_ATTACHMENT_STORE_OP_DONT_CARE: Contents of the framebuffer will be undefined after the rendering operation.
    */
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; //our app won't do anything with the stencil buffer
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    //Images need to be transinioed to specific layouts that are suitable for the operation that theya re going to be involved in next.
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    /*subpasses are subsequent rendering operations that depends on the the contents of frambuffers in previous passes
    a single render pass can consist of multiple subpasses but we will stick to 1 for now.
    Subpasses references are defined using the "VkAttachmentReference" struct*/
    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    //Subpasses itself are described using the VkSubpassDescription struct
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef; //indices are direclty references from the fragment shader with the "layou (location = 0) out vec4 outColor" directive

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL; // implicit subpass before or after the render pass depending on whether it is specified in srcSubpass or dstSubpass
    dependency.dstSubpass = 0; //efers to our subpass, which is the first and only one. The dstSubpass must always be higher than srcSubpass to prevent cycles in the dependency graph (unless one of the subpasses is VK_SUBPASS_EXTERNAL).
    /*Specify the operations to wait on and the stages in which these operations occur
    We need to wait for the swap chain to finish reading from the image before we can access it.
    This can be accomplished by waiting on the color attachment output stage itself.*/
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    /*The operations that should wait on this are in the color attachment stage and involve
    the writing of the color attachment. These settings will prevent the transition from happening until
    it's actually necessary (and allowed): when we want to start writing colors to it.*/
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    /*Subpasses in a render pass automatically take care of image layout transitions. These transitions are controlled
    by subpass depencencies, which specify memory and execution deêndencies between subpasses.
    There are 2 built-in depencies that take care of the transition at the start and at the end of the render pass, but the former
    does not occur at the right time : it assumes that the transition occurs at the start of the pipeline, but we haven't acquired 
    the image yet at that point. There are 2 ways to deal with this problem :
        1. change the waitStages for the imageAvailableSemaphore to VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT to ensure that the render passes don't begin until the image is available
        2. make the render pass wait for the "VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT"
    We'll go with the 2nd option :*/

    //Create the render pass itself
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if ( vkCreateRenderPass(this->_logicalDevice, &renderPassInfo, nullptr, &this->_renderPass) != VK_SUCCESS ) {
        throw std::runtime_error("Failed to create render pass");
    }
}

void HelloTriangleApplication::_createFrameBuffers(void) {
    //1. Resizing all the container to hold all of the framebuffer
    this->_swapChainFrameBuffers.resize(this->_swapChainImageViews.size());
    //2. Iterate through the image views and create a framebuffer for each of them
    for (size_t i = 0; i < this->_swapChainImageViews.size(); i++) {
        VkImageView attachments[] = {
            this->_swapChainImageViews[i]
        };
        VkFramebufferCreateInfo frameBufferInfo{};
        frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frameBufferInfo.renderPass = this->_renderPass;
        //Specify the "VkImageView" objects that should be bound to the respective attachment descriptions in the render pass "pAttachment" array
        frameBufferInfo.attachmentCount = 1;
        frameBufferInfo.pAttachments = attachments;
        frameBufferInfo.width = this->_swapChainExtent.width;
        frameBufferInfo.height = this->_swapChainExtent.height;
        frameBufferInfo.layers = 1; //Number of layers in image arrays

        if ( vkCreateFramebuffer(this->_logicalDevice, &frameBufferInfo, nullptr, &this->_swapChainFrameBuffers[i]) != VK_SUCCESS ) {
            throw std::runtime_error("Failed to create framebuffer");
        }
    }
}

void HelloTriangleApplication::_createCommandPool(void) {
    QueueFamilyIndices queueFaimyIndices = UTILS_findQueueFamilies(this->_physicalDevice, this->_surface);

    VkCommandPoolCreateInfo commandPoolInfo{};
    commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    /*There are 2 possible flags for command pool :
    - VK_COMMAND_POOL_CREATE_TRANSIENT_BIT : Hint that command buffers are rerecorded with new commands very often (may change memory allocation behavior)
    - VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT : Allow command buffers to be rerecorded individually, without this flag they all have to be reset together
    We will be recording a command buffer every frame so we want to be able to reset and rerecord over it.
    Thus, we need to set the VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT flag bit for our command pool.
    Command buffers are executed by submitting them on one of the device queues, like the graphics and presentation queues we retrieved.
    Each command pool can only allocate command buffers that are submitted on a single type of queue. We're going to record commands for drawing,
    which is why we've chosen the graphics queue family.*/
    commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolInfo.queueFamilyIndex = queueFaimyIndices.graphicsFamily.value();

    if ( vkCreateCommandPool(this->_logicalDevice, &commandPoolInfo, nullptr, &this->_commandPool) != VK_SUCCESS ) {
        throw std::runtime_error("Failed to create command pool");
    }
}

void HelloTriangleApplication::_createCommandBuffer(void) {
    VkCommandBufferAllocateInfo commandBufferAllocInfo{};
    commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocInfo.commandPool = this->_commandPool;
    /*"level specifies if the allocated command buffers are primary or secondary command buffers :
    - VK_COMMAND_BUFFER_LEVEL_PRIMARY : Can be sumbitted to a queue for execution, but cannot be called from other command buffers
    - VK_COMMAND_BUFFER_LEVEL_SECONDARY : Cannot be submitted direclty, but can be called from primary command buffers"*/
    commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocInfo.commandBufferCount = 1; //We are only allocating 1 command buffer

    if ( vkAllocateCommandBuffers(this->_logicalDevice, &commandBufferAllocInfo, &this->_commandBuffer) != VK_SUCCESS ) {
        throw std::runtime_error("Failed to allocate command buffers");
    }
}

void HelloTriangleApplication::_createSyncObjects(void) {
    /*Here we will be creating our 2 sempahores and our fence.
    Creating a semaphore require to fill in the "VkSemaphoreCreateInfo" struct. The current version of the API
    doesn't actually have any required fields besides "sType"*/
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    //Creating a fence require to fil in a "VkFenceCreateInfo" struct, only "sType" is mendatory
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    /*We must create the fence in the signaled state because we wait for the fence to be signaled at the first frame
    but if it is not the case, we will wait indefinitely at the first frame*/
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    if (
        vkCreateSemaphore(this->_logicalDevice, &semaphoreInfo, nullptr, &this->_imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(this->_logicalDevice, &semaphoreInfo, nullptr, &this->_renderFinishedSemaphore) != VK_SUCCESS ||
        vkCreateFence(this->_logicalDevice, &fenceInfo, nullptr, &this->_inFlightFence) != VK_SUCCESS
    )  {
        throw std::runtime_error("Failed to create sempahores or fences");
    }
}

void HelloTriangleApplication::drawFrame(void) {
    /*1. At the start of the frame, we want to wait untiil the previous frame has finshed, so that the
    command buffer and semaphores are available to use. To do that we call "vkWaitForFences"
    This function takes an array of fences and waits on the host for either any or all of the fences to be
    signlaled before returning. The "VK_TRUE" we pass here indicates that we wawnt to wait for all fences,
    but in the case of a single one, it doesn't matter. This function also has a tiemeout parameter that we
    set to the maxium value of a 64 bit unsigned integer, "UINT64_MAX", which effectively disablrs timeout.*/
    vkWaitForFences(this->_logicalDevice, 1, &this->_inFlightFence, VK_TRUE, UINT64_MAX);
    //2. After waiting, we must manually reset the fence to the unsignaled state with the "vkResetFences" call :
    vkResetFences(this->_logicalDevice, 1, &this->_inFlightFence);
    //3. Next, we must acquire an image from the swap chain. Swapchain is an extension feature, so we must use a "vk*HDR" function
    uint32_t imageIndex;
    vkAcquireNextImageKHR(
        this->_logicalDevice,
        this->_swapChain,
        UINT64_MAX, //Timeout, in this case no timeout
        this->_imageAvailableSemaphore, //Sync objects that are to be signlaed when the presentation engine is finished using the image
        VK_NULL_HANDLE, //Sync objects that are to be signlaed when the presentation engine is finished using the image
        &imageIndex //Variable to store the index of the swap chain image that has become available. It refers to the vkImage in our this->_swapChainImages array. We will use this index to pick the vkFrameBuffer
    );
    //4. Next, we must record the command buffer
        //First, reset it to make sure it is able to be recorded
    vkResetCommandBuffer(this->_commandBuffer, 0);
        //Next, record a command buffer with the imageIndex retrieved from the call to "vkAcquireNextImageKHR()".
    this->recordCommandBuffer(this->_commandBuffer, imageIndex);
    //5. Submit the command buffer. This is done using a "vkSubmitInfo" struct
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkSemaphore vkWaitSemaphores[] = {this->_imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    /*We want to wait with writing colors to the image until it's available, so we're specifying the stage of the
    graphics pipeline that writes to the color attachment. That means that theoretically the implementation can
    already start executing our vertex shader and such while the image is not yet available. Each entry in the
    waitStages array corresponds to the semaphore with the same index in pWaitSemaphores.*/
    submitInfo.pWaitSemaphores = vkWaitSemaphores; //Which sempahores to wait on before execution begins
    submitInfo.pWaitDstStageMask = waitStages; // Which stage(s) of the pipeline to wait before execution begins
    //Which command buffers to actually submit for execution :
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &this->_commandBuffer;
    //Which semaphores to signal once the command buffer(s) have finished execution.
    VkSemaphore signalSemaphores[] = {this->_renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if ( vkQueueSubmit(this->_graphicsQueue, 1, &submitInfo, this->_inFlightFence) != VK_SUCCESS ) {//The fence here is optional
        throw std::runtime_error("Failed to submit draw command buffer");
    }
    //6. Subpass dependencies, we've updated our renderPass to inclue dependencies
    /*7. The last step of drawing a frame is submitting the result back to the swap chain ot have it eventually show up on*
    the screen. We configure the presentation using a "VkPresentInfoKHR" struct*/
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    //Which semaphore to wait on before presentation can happen.
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    //Swap chains to present images to and the index of the image for each swap chain. Will almost always be a single one.
    VkSwapchainKHR swapChains[] = {this->_swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;//The one we retrieved from the call to "vkAcquireNextImageKHR"
    presentInfo.pResults = nullptr; //Optional : allows you to specify an array of VkResult values to check for every individual swap chain if presentation was successful
    //Submits the request to present an image to the swap chain. We'll implement error handling later.
    vkQueuePresentKHR(this->_presentQueue, &presentInfo);
}
void HelloTriangleApplication::_mainLoop(void) {
    while (glfwWindowShouldClose(this->_window) == false) { // poll events while window has not been ordered to close by the user
        glfwPollEvents();
        this->drawFrame();
    }
    /*All operations in this->drawFrame() are asynchronous. This means that when we exit the loop in _mainLoop,
    drawing and presentation operations may still be going on. Cleaning up resources while that is happening will
    probably cause a crash.
    TO fix that problem, we should wait for the logical device to finish operations before exiting _mainLoop and
    destroying the window.
    You can also wait for operations in a specific command queue to be finished with vkQueueWaitIdle.
    These functions can be used as a very rudimentary way to perform synchronization.*/
    vkDeviceWaitIdle(this->_logicalDevice);
}

void HelloTriangleApplication::_cleanUp(void) {
    vkDestroySemaphore(this->_logicalDevice, this->_imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(this->_logicalDevice, this->_renderFinishedSemaphore, nullptr);
    vkDestroyFence(this->_logicalDevice, this->_inFlightFence, nullptr);
    vkDestroyCommandPool(this->_logicalDevice, this->_commandPool, nullptr);
    for (auto frameBuffer : this->_swapChainFrameBuffers) {
        vkDestroyFramebuffer(this->_logicalDevice, frameBuffer, nullptr);
    }
    vkDestroyPipeline(this->_logicalDevice, this->_graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(this->_logicalDevice, this->_pipelineLayout, nullptr);
    vkDestroyRenderPass(this->_logicalDevice, this->_renderPass, nullptr);
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
