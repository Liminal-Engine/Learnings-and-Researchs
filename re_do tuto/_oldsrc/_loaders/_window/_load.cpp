#include "liminalvulkanwrapper/include/_loaders/_window/_load.hpp"

namespace vulkanhdl {
    namespace _loaders {
        namespace _window {

            GLFWwindow *_load(const std::string &name) {
                glfwInit();

                glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Explicelty tells GFLW to NOT use OpenGL context
                glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //Disable resize because wi will need to handle it later

                return glfwCreateWindow(800, 600, name.c_str(), nullptr, nullptr);
            }

        } // vulkanhdl
    } // _loaders
} // _window