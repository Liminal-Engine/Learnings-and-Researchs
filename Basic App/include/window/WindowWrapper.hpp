#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace window {
    class WindowWrapper {
        
        public:
            WindowWrapper(const uint32_t width, const uint32_t height, const char *title);
            ~WindowWrapper(void);
            
            bool shouldClose(void) const;
            void pollEvents(void) const;
            
            std::vector<const char *>
            getRequiredVulkanInstanceExtensions(void) const;

            VkExtent2D getFrameBufferSize(void) const;


            //Getters
            GLFWwindow *getGLFWWindow(void) const;            

        private:
            GLFWwindow *_GLFWwindow;
    };

}
