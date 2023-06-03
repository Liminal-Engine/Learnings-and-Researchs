#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <iostream>
#include <string>
#include <sstream>

#include "vulkan/pipeline/PipelineWrapper.hpp"

namespace vulkan::pipeline {
    PipelineWrapper::PipelineWrapper(void)
    {
    }

    PipelineWrapper::PipelineWrapper(
        const VkDevice &logicalDevice,
        const std::string &shaderDirectory
    ) {
        this->addShadersFromDirectory(logicalDevice, shaderDirectory);
    }

    void PipelineWrapper::cleanUp(const VkDevice &logicalDevice) {
        for (const auto &pair : this->_vertexShaderModules) {
            vkDestroyShaderModule(logicalDevice, pair.second, nullptr);
        }
        for (const auto &pair : this->_fragmentShaderModules) {
            vkDestroyShaderModule(logicalDevice, pair.second, nullptr);
        }
    }

    void PipelineWrapper::addShadersFromDirectory(
        const VkDevice &logicalDevice,
        const std::string &shaderDirectory
    ) {
        for (
            const auto &entry :
            std::filesystem::recursive_directory_iterator(shaderDirectory)
        ) {
            std::string tmp;
            std::stringstream ss;
            std::vector<std::string> separatedFileName;
            std::string vertexType;

            if (entry.path().string().ends_with(".spv")) {
                ss << entry.path().filename().string();
                while(getline(ss, tmp, '.')) {
                    separatedFileName.push_back(tmp);
                }
                vertexType = separatedFileName.at(separatedFileName.size() - 2);
                std::cout << vertexType << std::endl;
                if (vertexType == "vert") {
                    this->addVertexShader(
                        logicalDevice, 
                        entry.path().string(), //the path is given as a name
                        entry.path().string()
                    );
                } else if (vertexType == "frag") {
                    this->addFragmentShader(
                        logicalDevice, 
                        entry.path().string(), //the path is given as a name
                        entry.path().string()
                    );
                } else {
                    throw std::runtime_error("An error occured with the \
following vertex file : " + entry.path().string());
                }
                separatedFileName.clear();
            }
        }
    }


    void PipelineWrapper::addVertexShader(
        const VkDevice &logicalDevice,
        const std::string &name,
        const std::string &filePath
    ) {
        this->_addShaderModule(
            {name, this->_getShaderModule(logicalDevice, filePath)},
            this->_vertexShaderModules
        );
    }

    void PipelineWrapper::addFragmentShader(
        const VkDevice &logicalDevice,
        const std::string &name,
        const std::string &filePath
    ) {
        this->_addShaderModule(
            {name, this->_getShaderModule(logicalDevice, filePath)},
            this->_fragmentShaderModules
        );
    }

    void PipelineWrapper::deleteVertexShader(
        const VkDevice &logicalDevice, const std::string &name
    ) {
        this->_deleteShaderModule(
            logicalDevice, name, this->_vertexShaderModules
        );
    }

    void PipelineWrapper::deleteFragmentShader(
        const VkDevice &logicalDevice, const std::string &name
    ) {
        this->_deleteShaderModule(
            logicalDevice, name, this->_fragmentShaderModules
        );
    }

    //private methods
    VkShaderModule PipelineWrapper::_getShaderModule(
        const VkDevice &logicalDevice,
        const std::string &filePath
    ) {
        //1. Read shader file as binary
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);

        if (file.is_open() == false) {
            throw std::runtime_error("Failed to load file : " + filePath);
        }
        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        //2. Create shader module from shader code
        VkShaderModuleCreateInfo createInfo {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize = buffer.size(),
            .pCode = reinterpret_cast<const uint32_t *>(buffer.data()) // The bytecode pointer is a uint32_t pointer rather than a char pointer. Therefore we will need to cast the pointer with reinterpret_cast as shown below. When you perform a cast like this, you also need to ensure that the data satisfies the alignment requirements of uint32_t. Lucky for us, the data is stored in an std::vector where the default allocator already ensures that the data satisfies the worst case alignment requirements.                        
        };
        VkShaderModule shaderModule{};
        if (vkCreateShaderModule(
            logicalDevice, &createInfo, nullptr, &shaderModule
        ) != VK_SUCCESS) {
            throw std::runtime_error(
                "Failed to create shader module from" + filePath
            );
        }
        return shaderModule;
    }


    void PipelineWrapper::_addShaderModule(
        const std::pair<std::string, VkShaderModule> &pair,
        std::unordered_map<std::string, VkShaderModule> &unorderedMap
    ) {
        unorderedMap.insert(pair);
    }

    void PipelineWrapper::_deleteShaderModule(
        const VkDevice &logicalDevice,
        const std::string &name,
        std::unordered_map<std::string, VkShaderModule> &unorderedMap
    ) {
        vkDestroyShaderModule(
            logicalDevice, unorderedMap[name], nullptr
        );
        unorderedMap.erase(name);
    }
}