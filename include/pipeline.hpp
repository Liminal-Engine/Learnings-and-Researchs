#pragma once

#include "device.hpp"

#include <string>
#include <vector>

struct PipelineConfig {};

class Pipeline {
    public:
        //The constructor takes both vertex and fragment shaders files
        Pipeline(
            const std::string &vertexFilePath,
            const std::string &fragmentFilePath,
            Device &device,
            const PipelineConfig &config
        );
        ~Pipeline();

        //delete our copy constructors
        //we want to avoid accidentally duplicating the pointers to
        // our Vulkan object
        Pipeline(const Pipeline &) = delete;
        void operator=(const Pipeline &) = delete;

        static PipelineConfig getDefaultPipelineConfig(
            uint32_t width,
            uint32_t height
        );

    private:
        static std::vector<char> _readFile(const std::string &filePath);
        void _createGraphicsPipeline(
            const std::string &vertexFilePath,
            const std::string &fragmentFilePath,
            const PipelineConfig &config
        );
        void createShaderModule(
            const std::vector<char> &code,
            VkShaderModule *shaderModule
        );

        //reference to the device
        //could be memeory unsafe but a pipeline always needs a device
        // so it is the only time we will do this
        Device &_device;
        //The handle to our Vulkan pipeline object
        VkPipeline _graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
};