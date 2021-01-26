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
    unsigned int texture{};
    glGenTextures(1, &texture); // 1 - count of textures, &texture - begin of the id array

    glBindTexture(GL_TEXTURE_2D, texture);

    // to set texture params (for s and t, corresp. to x and y):
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    /* if we choose GL_CLAMP_TO_BORDER, we have to set user color:
        float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    */

    // magnifying and minifying operations:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR); // this filtering is only for minifying
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    auto image = std::make_unique<ImageContainer>("container.jpg");
    if (image->getData())
    {
        // generate texture:
        glTexImage2D(GL_TEXTURE_2D, // texture target
                     0, // mipmap level (base level == 0)
                     GL_RGB, // texture format to store
                     image->getWidth(),
                     image->getHeight(),
                     0, // some C-legacy stuff (border?)
                     GL_RGB, // format of the source image
                     GL_UNSIGNED_BYTE, // data type of the source image
                     image->getData());

        // we can generate an each mipmap level by hand (call glTexImage2D for each level), or just call to:
        glGenerateMipmap(GL_TEXTURE_2D); // texture target


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
         *  glActiveTexture(GL_TEXTURE0);
         *  glBindTexture(GL_TEXTURE_2D, texture);
        */

        cb = [this, texture]()
        {
            glBindTexture(GL_TEXTURE_2D, texture);
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
