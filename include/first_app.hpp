#pragma once

#include "window.hpp"

class FirstApp {

    public:

        static constexpr int DEFAULT_WINDOW_WIDTH = 800;
        static constexpr int DEFAULT_WINDOW_HEIGHT = 600;

        //2 constructors, the first will use default values
        FirstApp(const std::string &name);
        FirstApp(
            const int width,
            const int height,
            const std::string &name
        );

        void run(void);

    private :
        Window _window; //pointer to a window

};