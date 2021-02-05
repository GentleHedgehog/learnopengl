#pragma once

#include <functional>
#include <GLFW/glfw3.h>


namespace nOpenglFramework {

struct OpenglContextData
{
    GLFWwindow* window = nullptr;
};

using WorkerCallbackSimpleType = std::function<void()>;
using WorkerCallbackType = std::function<void(const OpenglContextData& window)>;

}

