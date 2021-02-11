#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class CameraSystem
{
public:
    CameraSystem(glm::vec3 cameraPos = {0.f, 0.f, 3.f});

    void process(GLFWwindow *w);
    glm::vec3 getCurrentPosition();

    float getCurrentFOV();
    glm::mat4 getCurrentViewMatrix();
};

