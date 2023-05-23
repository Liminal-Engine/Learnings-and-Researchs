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
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
    inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

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
    VkPipelineViewportStateCreateInfo viewportInfo{};
    viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportInfo.viewportCount = 1;
    viewportInfo.pViewports = &viewport;
    viewportInfo.scissorCount = 1;
    viewportInfo.pScissors = &scissor;

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
    VkPipelineLayout pipelineLayout;
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0; // Optional
    pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional //other way of passing dynamic values
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional
    
    if (vkCreatePipelineLayout(this->_logicalDevice, &pipelineLayoutInfo, nullptr, &this->_pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create pipeline layout.");
    }

    vkDestroyShaderModule(this->_logicalDevice, vertShaderModule, nullptr);
    vkDestroyShaderModule(this->_logicalDevice, fragShaderModule, nullptr);
}

void HelloTriangleApplication::_mainLoop(void) {
    while (glfwWindowShouldClose(this->_window) == false) { // poll events while window has not been ordered to close by the user
        glfwPollEvents();
    }
}

void HelloTriangleApplication::_cleanUp(void) {
    vkDestroyPipelineLayout(this->_logicalDevice, this->_pipelineLayout, nullptr);
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
