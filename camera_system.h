#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class CameraSystem
{
public:
    CameraSystem();

    void process(GLFWwindow *w);
    float getCurrentFOV();
    glm::mat4 getCurrentViewMatrix();
};

