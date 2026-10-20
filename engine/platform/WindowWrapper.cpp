#include "WindowWrapper.hpp"

#include <stdexcept>

WindowWrapper::WindowWrapper(
    int width,
    int height,
    const char* name,
    const float clearColor[3])
    : color{clearColor[0], clearColor[1], clearColor[2]}
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(
        width, height, name, nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    
    glfwMakeContextCurrent(window);    

    glfwSwapInterval(1);

    if (!gladLoadGL(
        reinterpret_cast<GLADloadfunc>(glfwGetProcAddress)))
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw std::runtime_error("Failed to load GLAD");
    }

}

void WindowWrapper::use() {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    if (width > 0 && height > 0) {
        glViewport(0, 0, width , height);
        glClearColor(color[0], color[1], color[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void WindowWrapper::swapBuffers() {
    glfwSwapBuffers(window);
}

void WindowWrapper::destroy() {
    glfwDestroyWindow(window);
}