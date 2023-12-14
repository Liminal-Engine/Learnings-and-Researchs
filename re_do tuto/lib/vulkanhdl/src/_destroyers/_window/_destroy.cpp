#include "vulkanhdl/include/_destroyers/_window/_destroy.hpp"

namespace vulkanhdl {
    namespace _destroyers {
        namespace _window {

            void _destroy(GLFWwindow *window) {
                glfwDestroyWindow(window);
                glfwTerminate();
            }

        } // _window
    } // _destroyers
} // vulkanhdl

