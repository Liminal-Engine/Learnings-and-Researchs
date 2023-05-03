#include "pipeline.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>

Pipeline::Pipeline(
    const std::string &vertexFilePath,
    const std::string &fragmentFilePath,
    Device &device,
    const PipelineConfig &config
) : _device{device}
{
    this->_createGraphicsPipeline(vertexFilePath, fragmentFilePath, config);
}

Pipeline::~Pipeline() {
    
}


std::vector<char> Pipeline::_readFile(const std::string &filePath) {
    /* initialize a new input file stream variable
        ate = when the files open, we go to the end immediatly,
        in order to simplify the process of getting the size of the file.
        binary = read the file as a binary to avoid any unwanted text
        transformations from occuring.
    */
    std::ifstream file(filePath, std::ios::ate | std::ios::binary);

    if (file.is_open() == false) {
        throw std::runtime_error("failed to open file : " + filePath);
    }
    // since we used the "ate" flag, we get the file size when using
    //tellg().
    size_t fileSize = static_cast<size_t>(file.tellg());
    //initialize our buffer with the size of the file.
    std::vector<char> buffer(fileSize);
    file.seekg(0); //go to the begining of the file.
    file.read(buffer.data(), fileSize); //read file
    file.close(); //close file
    return buffer;//return buffer
}

void Pipeline::_createGraphicsPipeline(
    const std::string &vertexFilePath,
    const std::string &fragmentFilePath,
    const PipelineConfig &config
) {
    //This functions reads the vertex and fragment code.
    auto vertCode = this->_readFile(vertexFilePath);
    auto fragCode = this->_readFile(fragmentFilePath);

    //output the size to make sure it is read correctly :
    std::cout << "Vertex Shader Code Size: " <<
    vertCode.size() << std::endl;
    std::cout << "Fragment Shader Code Size: " <<
    fragCode.size() << std::endl;
}


void Pipeline::createShaderModule(
    const std::vector<char> &code,
    VkShaderModule *shaderModule
) {
    //common pattern when using Vulkan API :
    // instead of calling a function with a bunch of parameter,
    // we configure a struc and call a Vulkan function taking this 
    // struct as paramerter
    VkShaderModuleCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    //here we are using a reinterpret_cast to cast our char vector to
    // a uint_32 * since a int32 and a char are not the same size
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());
    //this->_device.device() is a getter to the Vulkan device handle
    //that the Device class encapsulate
    if (vkCreateShaderModule(
        this->_device.device(),
        &createInfo,
        nullptr,
        shaderModule
    ) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module");
    }
}

PipelineConfig Pipeline::getDefaultPipelineConfig(
    uint32_t width,
    uint32_t height
) {
    //we just leave the struct empty for now,
    //but we will come back at it later.
    PipelineConfig config{};

    return config;
}