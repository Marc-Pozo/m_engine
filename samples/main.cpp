
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

static GLuint compileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char log[1024]{};
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        std::cerr << "Shader error: " << log << '\n';
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

int main()
{
    if (!glfwInit())
        return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
        1280, 720, "M-Engine", nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGL(
            reinterpret_cast<GLADloadfunc>(glfwGetProcAddress)))
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    const char* vertexSource = R"(
        #version 450 core

        layout(location = 0) in vec3 aPosition;
        layout(location = 1) in vec3 aColor;

        out vec3 color;

        uniform mat4 uMVP;

        void main()
        {
            color = aColor;
            gl_Position = uMVP * vec4(aPosition, 1.0);
        }
    )";

    const char* fragmentSource = R"(
        #version 450 core

        in vec3 color;
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(color, 1.0);
        }
    )";

    GLuint vertexShader =
        compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader =
        compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    if (!vertexShader || !fragmentShader)
    {
        if (vertexShader) glDeleteShader(vertexShader);
        if (fragmentShader) glDeleteShader(fragmentShader);
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint linked = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    if (!linked)
    {
        char log[1024]{};
        glGetProgramInfoLog(program, sizeof(log), nullptr, log);
        std::cerr << "Program link error: " << log << '\n';
        glDeleteProgram(program);
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // Position (x, y, z), color (r, g, b).
    const float vertices[] = {
        -0.5f,-0.5f,-0.5f, 1,0,0,
         0.5f,-0.5f,-0.5f, 0,1,0,
         0.5f, 0.5f,-0.5f, 0,0,1,
        -0.5f, 0.5f,-0.5f, 1,1,0,
        -0.5f,-0.5f, 0.5f, 1,0,1,
         0.5f,-0.5f, 0.5f, 0,1,1,
         0.5f, 0.5f, 0.5f, 1,1,1,
        -0.5f, 0.5f, 0.5f, 1,0.5f,0
    };

    const unsigned int indices[] = {
        0,2,1, 0,3,2,
        4,5,6, 4,6,7,
        0,1,5, 0,5,4,
        2,3,7, 2,7,6,
        0,4,7, 0,7,3,
        1,2,6, 1,6,5
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float),
                          reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        if (width <= 0 || height <= 0)
            continue;

        glViewport(0, 0, width, height);
        glClearColor(0.08f, 0.10f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float time = static_cast<float>(glfwGetTime());

        glm::mat4 model = glm::rotate(
            glm::mat4(1.0f), time,
            glm::vec3(0.5f, 1.0f, 0.0f));

        glm::mat4 view = glm::lookAt(
            glm::vec3(2.0f, 2.0f, 3.0f),
            glm::vec3(0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            static_cast<float>(width) / height,
            0.1f, 100.0f);

        glm::mat4 mvp = projection * view * model;

        glUseProgram(program);
        glUniformMatrix4fv(
            glGetUniformLocation(program, "uMVP"),
            1, GL_FALSE, glm::value_ptr(mvp));

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 36,
                       GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
    }

    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(program);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
