#include "window.hpp"

#include <stdexcept>

Window::Window(const int width, const int height, const std::string &name) :
_width{width},
_height{height},
_name{name}
{
    this->_initWindow(); //call init window
}

Window::~Window() {
    glfwDestroyWindow(this->_window); //destory window
    glfwTerminate(); //terminate GLFW
}

std::pair<int, int> Window::getSize(void) const {
    return std::make_pair(this->_width, this->_height);
}

std::string Window::getName(void) const { return this->_name; }

void Window::_initWindow(void) {
    glfwInit(); //Init GLFW
    //GLFW was first designed to use OpenGL context, we tell him not to.
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); 
    //We forbid resizing, we will wee why in tutorial 10
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    this->_window = glfwCreateWindow( //create window
        this->_width,
        this->_height,
        this->_name.c_str(),
        nullptr,
        nullptr
    );
}

bool Window::shouldClose(void) const {
    //catches any event that says the user want to quit
    return glfwWindowShouldClose(this->_window);
}


void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
    if (glfwCreateWindowSurface(instance, this->_window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface");
    }
}