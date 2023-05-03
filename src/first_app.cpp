#include "first_app.hpp"

FirstApp::FirstApp(const std::string &name)
: 
_window( Window(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, name) ),
_device( Device(this->_window) ),
_pipeline(Pipeline(
    "shaders/simple_shader.vert.spv",
    "shaders/simple_shader.frag.spv",
    this->_device,
    Pipeline::getDefaultPipelineConfig(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)

))
{

}

FirstApp::FirstApp(
    const int width,
    const int height,
    const std::string &name
) :
_window(Window(width, height, name)),
_device( Device(this->_window) ),
_pipeline(Pipeline(
    "shaders/simple_shader.vert.spv",
    "shaders/simple_shader.frag.spv",
    this->_device,
    Pipeline::getDefaultPipelineConfig(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)
))
{

}

void FirstApp::run(void) {
    while (this->_window.shouldClose() == false) {
        //poll window events.
        //An event can be keyboard key, mouse button or X button to quit.
        glfwPollEvents();
    }
}