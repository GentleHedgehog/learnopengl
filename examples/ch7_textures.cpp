#include "ch7_textures.h"
#include "ch7_shaders.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include "image_framework.h"

Ch7_Textures::Ch7_Textures()
{    
    putTextureOnTriangle();
}

void Ch7_Textures::putTextureOnTriangle()
{
    auto image = std::make_shared<ImageContainer>("container.jpg");
    if (image->getData())
    {
        texApplier.loadFromRGB(image);

        // create the array of vertices attributes:
        float vertices[] = {
            // positions xyz      // colors rgb    // texture coords st
            0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f // top left
        };

        unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3 // second triangle
        };

        TrianglesDrawer::Attributes attr;
        attr.addAttribute(0, 3, 0);
        attr.addAttribute(1, 3, 3);
        attr.addAttribute(2, 2, 6);

        tdEbo.prepareToDraw(vertices, indices, attr);

        sp.createAndLink(textureVS, textureFS);

        // some drivers require to assign a texture unit to each sampler uniform
        /* glUniform1i can assign a location (known as a texture unit) value to sampler
         * default texture unit == 0 (but is not for all drivers)
         * we can set it manually:
         *  glActiveTexture(GL_TEXTURE0); // 0..15, GL_TEXTURE8 ~ GL_TEXTURE0+8
         *  glBindTexture(GL_TEXTURE_2D, texture);
        */

        cb = [this]()
        {
            texApplier.execute();
            sp.use();
            tdEbo.execute();
        };
    }
}

void Ch7_Textures::operator()()
{
    if (cb)
    {
        cb();
    }
}
