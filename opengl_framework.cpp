#include "opengl_framework.h"
#include <iostream>

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
    auto res = glfwInit();
    if (res != GLFW_TRUE)
    {
        throw std::logic_error("glfw init fail");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for Mac OS
}

OpenglFramework::~OpenglFramework()
{
    glfwTerminate();
}

bool OpenglFramework::createWindowAsCurrentContext(int w, int h)
{
    if (window)
    {
        std::cout << "window was already created" << std::endl;
        return true;
    }

    window = glfwCreateWindow(w, h, "learn opengl", NULL, NULL);
    if (! window)
    {
        std::cout << "window creation failed" << std::endl;
        return false;
    }

    glfwMakeContextCurrent(window);

    // ok only after glfwMakeContextCurrent (?)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "glad fail";
        return false;
    }

    glViewport(0, 0, w, h); // inside window, coords: (-1, 1) -> (0, 800) or (0, 600)
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return true;
}

void OpenglFramework::printNumberOfAttributes()
{
    GLint nrAtt{};
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAtt);
    std::cout << "max num of vertex attribs supported: " << nrAtt << std::endl;
}

void OpenglFramework::run(nOpenglFramework::WorkerCallbackType f)
{
    if (! window)
    {
        //        std::cout << "cannot run without window" << std::endl;
        //        return;
        throw std::logic_error("cannot run without window");
    }

    nOpenglFramework::OpenglContextData context;
    context.window = window;

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // choose color (state settings func)
        glClear(GL_COLOR_BUFFER_BIT); // clear color buffer (state using func)

        f(context);

        glfwPollEvents();
        glfwSwapBuffers(window); // front and back buffers
    }
}

void OpenglFramework::run(nOpenglFramework::WorkerCallbackSimpleType f)
{
    auto stub = [=](const nOpenglFramework::OpenglContextData&){
            f();
    };
    run(stub);
}



