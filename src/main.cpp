#include <iostream>

#include "../include/glad/glad.h" // always link glad before glfw
#include "GLFW/glfw3.h"

// stored vertex shader GLSL - OpenGL Shading Language
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

// stored fragment shader
const char *fragmentShaderSource = "#version 330 core\n"
                                  "out vec4 FragColor;\n"
                                  "void main()\n"
                                  "{\n"
                                  "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                  "}\0";


void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

void processVertexShader();

// global variables
const unsigned int SCREEN_WIDTH{800};
const unsigned int SCREEN_HEIGHT{600};

GLFWwindow *initWindow() {
    // glfw: init and configure
    glfwInit();
    // set glfw version to 3.3, so if that isn't the case our program will fail
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create new glfw window
    const char WINDOW_NAME[]{"myOpenGLProgram"};
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    // throw back if window creation fails
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    return window;
}

int main() {

    GLFWwindow *window = initWindow();

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        // rendering here
        glClearColor(0.2f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // we have to define 3 vertices in 3D (OpenGL handles all its graphics in 3D)
        // OpenGL's range of values: -1.0 and 1.0 on all 3 axes (x, y and z)
        // if they are outside of this spectrum, they are not taken into consideration
        // Vertex Buffer Object - stores large amount of vertice data in it
        // CPU takes a long time to get the data to GPU
        // once data is in GPU, the process becomes fast, so we want to send as much data at once as possible
        float vertices[] = {
                -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
                0.0f, 0.5f, 0.0f
        };

        processVertexShader();

        // ------------------ FRAGMENT SHADER ------------------
        // TLDR: handles colours in an RGBA form
        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);

        unsigned int VBO;
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // 1. what type of buffer we want data from
        // 2. size of data in bytes we want to pass
        // 3. actual data we want to send
        // 4. how we want to manage the data
        // STREAM, set once, used a few times at most
        // STATIC, set once, used many times
        // DYNAMIC, changed alot, used alot
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // glfw: check and call IO(key press, release, mouse move) events, swap the buffer
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void processVertexShader() {
    // ---------------------- VERTEX SHADER ----------------------
    // TLDR:
    // we create an object for the shader, so it can dynamically compile
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // 1. which shader object to compile
    // 2. how many strings are we passing as source code
    // 3. actual source code to compile
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    // check if shader compiled successfully
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}