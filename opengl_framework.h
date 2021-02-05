#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "common_definitions.h"

class OpenglFramework
{
public:
    OpenglFramework();
    ~OpenglFramework();
    bool createWindowAsCurrentContext(int w = 800, int h = 600);

    void printNumberOfAttributes();

    void run(nOpenglFramework::WorkerCallbackType f);
    void run(nOpenglFramework::WorkerCallbackSimpleType f);
private:
    GLFWwindow* window = nullptr;
};

