#include "opengl_framework.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

namespace  {

void framebuffer_size_callback(GLFWwindow* w, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* w)
{
    if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(w, true);
    }
}

}//nm

OpenglFramework::OpenglFramework()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for Mac OS
}

OpenglFramework::~OpenglFramework()
{
    glfwTerminate();
}

bool OpenglFramework::initialize()
{
    GLFWwindow* window = glfwCreateWindow(800, 600, "learn opengl", NULL, NULL);
    if (! window)
    {
        std::cout << "failed" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    // ok only after glfwMakeContextCurrent (?)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "glad fail";
        return false;
    }


    glViewport(0, 0, 800, 600); // inside window, coords: (-1, 1) -> (0, 800) or (0, 600)
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // choose color (state settings func)
        glClear(GL_COLOR_BUFFER_BIT); // clear color buffer (state using func)


        glfwPollEvents();
        glfwSwapBuffers(window); // front and back buffers
    }

    return true;
}



