#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    constexpr int width = 800;
    constexpr int height = 800;
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGl Test", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    /// Make Graphics
    // Shader
    const char* vertexShaderSource =
        "#version 460 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.xyz, 1.0);\n"
        "}\0";
    GLuint vertexShader = 0;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    const char* fragmentShaderSource =
        "#version 460 core\n"
        "uniform vec4 uColor;\n"
        "out vec4 outColor;\n"
        "void main()\n"
        "{\n"
        "   outColor = uColor;\n"
        "}\0";
    GLuint fragmentShader = 0;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint uColorLoc = glGetUniformLocation(shaderProgram, "uColor");

    // data
    GLfloat vertices1[] = {
        -1.0f, 1.0f, 0.0f,
        -1.0f, 0.8f, 0.0f,
        -0.8f, 0.8f, 0.0f,
        -0.8f, 1.0f, 0.0f,
    };
    GLfloat vertices2[] = {
        -0.9f, 0.9f, 0.0f,
        -0.9f, 0.7f, 0.0f,
        -0.7f, 0.7f, 0.0f,
        -0.7f, 0.9f, 0.0f,
    };
    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    // VBOs
    GLuint VBO1 = 0;
    glGenBuffers(1, &VBO1);
    GLuint VBO2 = 0;
    glGenBuffers(1, &VBO2);
    // IBOs
    GLuint IBO;
    glGenBuffers(1, &IBO);
    // VAOs
    GLuint VAO1;
    glGenVertexArrays(1, &VAO1);
    GLuint VAO2;
    glGenVertexArrays(1, &VAO2);


    // VAO1 Binding
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_DYNAMIC_DRAW);
    glBindVertexArray(VAO1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // VAO2 Binding
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glBindVertexArray(VAO2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniform4f(uColorLoc, 0.0f, 0.0f, 1.0f, 1.0f);
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glUseProgram(shaderProgram);
        glUniform4f(uColorLoc, 1.0f, 0.0f, 0.0f, 1.0f);
        glBindVertexArray(VAO2);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}