
#pragma once

//!Mandatory in our case, this means that we wan GLFW to also include Vulkan headers with it
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h> //Include GLFW
#include <string> //Include standard string header
#include <utility>

class Window {

    public:
        Window(const int width, const int height, const std::string &name);
        ~Window();

        std::pair<int, int> getSize(void) const;
        std::string getName(void) const;
        bool shouldClose(void) const;
        void createWindowSurface(
            VkInstance instance,
            VkSurfaceKHR *surface
        );

        /*
            In this tutorial, we will see that resource acquisition
            IS initialization.
            Since we're using a pointer to a window, we don't want 2 windows
            pointing to the same resource, so we delete the copy constructor
            and operator
        */
        Window(const Window&) = delete; //Copy constructor deletion
        Window &operator=(const Window &) = delete; //copy oprator deletion

    private:
        GLFWwindow *_window; //pointer to GLFW window
        const int _width; //window width
        const int _height; //window height
        const std::string _name; //window name
        
        void _initWindow(void); //to initialize window   
};