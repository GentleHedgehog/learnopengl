#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

namespace  {

void framebuffer_size_callback(GLFWwindow* w, int width, int height)
{
    glViewport(0, 0, width, height);
}

}//nm

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "learn opengl", NULL, NULL);
    if (! window)
    {
        std::cout << "failed" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "glad fail";
        return -1;
    }

    glViewport(0, 0, 800, 600); // inside window, coords: (-1, 1) -> (0, 800) or (0, 600)
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window); // front and back buffers
        glfwPollEvents();
    }

//    getchar();
//    cout << "Hello World!" << endl;

    glfwTerminate();
    return 0;
}
