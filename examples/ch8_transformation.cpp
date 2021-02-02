#include "ch8_transformation.h"
#include "ch8_shaders.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "image_framework.h"

namespace {

void printMatrix4(const glm::mat4& mat, const std::string& name = "")
{
    std::cout << "matrix: " << name << std::endl;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << mat[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

}

Ch8_Transformation::Ch8_Transformation()
{    
    putTwoTexturesOnRectangle();
}

void Ch8_Transformation::putTwoTexturesOnRectangle()
{
    auto image = std::make_shared<ImageContainer>("container.jpg");
    auto image2 = std::make_shared<ImageContainer>("awesomeface.png", true);
    if (image->getData() && image2->getData())
    {
        texApplier.prepareData(image, 0);
        texApplier2.prepareData(image2, 1);

        // create the array of vertices attributes:
        float vertices[] = {
            // positions xyz     // texture coords st
            0.5f, 0.5f, 0.0f,     1.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f,    1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f,    0.0f, 1.0f // top left
        };

        unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3 // second triangle
        };

        TrianglesDrawer::Attributes attr;
        attr.addAttribute(0, 3, 0);
        attr.addAttribute(1, 2, 3);

        tdEbo.prepareToDraw(vertices, indices, attr);

        sp.createAndLink(transformationVS, transformationFS);

        sp.use();
        // set uniform values once:
        sp.setInt("ourTexture1", 0);
        sp.setInt("ourTexture2", 1);

        glm::mat4 trans(1.f); // identity
        glm::vec3 zAxis(0.0f, 0.0f, 1.0f);
        trans = glm::rotate(trans, glm::radians(90.f), zAxis);
        glm::vec3 scaleFactors(0.5f, 0.5f, 0.5f);
        trans = glm::scale(trans, scaleFactors);
//        printMatrix4(trans);


        cb = [this, trans]()
        {
            texApplier.execute();
            texApplier2.execute();
            sp.use();            

            unsigned int transLoc = glGetUniformLocation(sp.getId().value(), "transform");
            glUniformMatrix4fv(transLoc,
                               1,  // how many matrices
                               GL_FALSE, // transpose flag (to get column-major ordering), glm has already such ordering
                               glm::value_ptr(trans)); // proper matrix representation

            tdEbo.execute();
        };
    }
}


void Ch8_Transformation::operator()()
{
    if (cb)
    {
        cb();
    }
}
