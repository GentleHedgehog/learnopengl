#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>

class OpenglFramework
{
public:
    OpenglFramework();
    ~OpenglFramework();
    bool createWindowAsCurrentContext(int w = 800, int h = 600);

    void printNumberOfAttributes();

    using CustomFunc = std::function<void()>;
    void run(CustomFunc f);
private:
    GLFWwindow* window = nullptr;
};

