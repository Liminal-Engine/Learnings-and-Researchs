#include "window/WindowWrapper.hpp"

namespace window {

    WindowWrapper::WindowWrapper(const uint32_t width, const uint32_t height, const char *title) {
        //1. Init GLFW lib
        glfwInit();
        //2. Tell GLFW not to use OpenGL context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        //3. Disable resize because we'll handle it later
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        //4. Create window
        this->_GLFWwindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
    }

    WindowWrapper::~WindowWrapper(void) {
        glfwDestroyWindow(this->_GLFWwindow);
        glfwTerminate();
    }

    bool WindowWrapper::shouldClose(void) const { return glfwWindowShouldClose(this->_GLFWwindow); }

    void WindowWrapper::pollEvents(void) const { glfwPollEvents(); }

    const std::vector<const char *> WindowWrapper::getRequiredVulkanInstanceExtensions(void) const {
        uint32_t extensionCount;
        const char ** GLFWExtensionsCStr = glfwGetRequiredInstanceExtensions(&extensionCount);
     
        return std::vector<const char *>(GLFWExtensionsCStr, GLFWExtensionsCStr + extensionCount);
    }

    //Getters
    GLFWwindow *WindowWrapper::getGLFWWindow(void) const { return this->_GLFWwindow; }

}
