#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>

class OpenglFramework
{
public:
    OpenglFramework();
    ~OpenglFramework();
    bool createWindowAsCurrentContext();

    using CustomFunc = std::function<void()>;
    void run(CustomFunc f);
private:
    GLFWwindow* window = nullptr;
};

