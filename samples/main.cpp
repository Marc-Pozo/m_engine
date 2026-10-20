/*
    TODO: 
        - Spec and create input class
        - Spec and create class to load 3d models
        - Spec and create class to display 3d models
*/


#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "engine/renderer/Shader.hpp"
#include "engine/platform/WindowWrapper.hpp"

int main()
{
    if (!glfwInit()) return 1;
    float color[] = {0.3f,0.4f,0.2f};
    WindowWrapper windowWrapper(1270, 720, "Sample1", color);

    Shader ourShader(
        "assets/shaders/vertex.vs",
        "assets/shaders/fragment.fs"
    );

    GLint linked = GL_FALSE;
    glGetProgramiv(ourShader.ID, GL_LINK_STATUS, &linked);

    if (!linked)
    {
        char log[1024]{};
        glGetProgramInfoLog(ourShader.ID, sizeof(log), nullptr, log);
        std::cerr << "Program link error: " << log << '\n';
        glDeleteProgram(ourShader.ID);
        windowWrapper.destroy();
        glfwTerminate();
        return 1;
    }

    // Position (x, y, z)
    float vertices[] = {
        // positions         // colors
        0.2f, -0.2f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.2f, -0.2f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
        0.0f,  0.2f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2,   // first triangle
    };

    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
                 vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
                 indices, GL_STATIC_DRAW);

    // Set the first vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Set the second vertex attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    float verticalOffset = 0;
    float horizontalOffset = 0;

    while (!glfwWindowShouldClose(windowWrapper.window))
    {

        glfwPollEvents();

        if (glfwGetKey(windowWrapper.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(windowWrapper.window, GLFW_TRUE);
        
        if (glfwGetKey(windowWrapper.window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            horizontalOffset += .01;
        }
        
        if (glfwGetKey(windowWrapper.window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            horizontalOffset -= .01;
        }
         
        if (glfwGetKey(windowWrapper.window, GLFW_KEY_UP) == GLFW_PRESS) {
            verticalOffset += .01;
        }
        
        if (glfwGetKey(windowWrapper.window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            verticalOffset -= .01;
        }

        ourShader.setFloat("horizontalOffset", horizontalOffset);
        ourShader.setFloat("verticalOffset", verticalOffset);

        windowWrapper.use();

        ourShader.use();

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        windowWrapper.swapBuffers();
    }

    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(ourShader.ID);

    windowWrapper.destroy();
    glfwTerminate();

    return 0;
}
