#include "vulkanhdl/include/_destroy/_window/_destroy.hpp"

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

