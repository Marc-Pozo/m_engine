
#ifndef WINDOW_H
#define WINDOW_H

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

class WindowWrapper {
    public:        
        GLFWwindow * window;
        const float color[3];
        WindowWrapper(
            int width,
            int height,
            const char* name,
            const float clearColor[3]
        );

        void use();

        void swapBuffers();

        void destroy();

};

#endif