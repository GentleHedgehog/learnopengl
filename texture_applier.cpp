#include "texture_applier.h"

#include <iostream>

#include <GLFW/glfw3.h>

TextureApplier::TextureApplier()
{
    glGenTextures(1, &texture); // 1 - count of textures, &texture - begin of the id array
    std::cout << "gen tex: texture = " << texture << std::endl;
}

TextureApplier::~TextureApplier()
{
    glDeleteTextures(1, &texture);
    std::cout << "delete tex: texture = " << texture << std::endl;
}

bool TextureApplier::loadFromRGB(std::shared_ptr<ImageContainer> image)
{
    return loadFromImage(image, GL_RGB);
}

bool TextureApplier::loadFromRGBA(std::shared_ptr<ImageContainer> image)
{
    return loadFromImage(image, GL_RGBA);
}

bool TextureApplier::loadFromImage(std::shared_ptr<ImageContainer> image, GLint imageFormat)
{
    if (! image)
    {
        return false;
    }

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



    // generate texture:
    glTexImage2D(GL_TEXTURE_2D, // texture target
                 0, // mipmap level (base level == 0)
                 imageFormat, // texture format to store
                 image->getWidth(),
                 image->getHeight(),
                 0, // some C-legacy stuff (border?)
                 imageFormat, // format of the source image
                 GL_UNSIGNED_BYTE, // data type of the source image
                 image->getData());

    // we can generate an each mipmap level by hand (call glTexImage2D for each level), or just call to:
    glGenerateMipmap(GL_TEXTURE_2D); // texture target

    return true;
}

void TextureApplier::execute()
{
    // some drivers require to assign a texture unit to each sampler uniform
    /* glUniform1i can assign a location (known as a texture unit) value to sampler
     * default texture unit == 0 (but is not for all drivers)
     * we can set it manually:
     *  glActiveTexture(GL_TEXTURE0); // 0..15, GL_TEXTURE8 ~ GL_TEXTURE0+8
     *  glBindTexture(GL_TEXTURE_2D, texture);
    */

    std::cout << "execute tex: unit = " << textureUnit << ", texture = " << texture << std::endl;

    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, texture);
}

bool TextureVector::addTexture(std::shared_ptr<ImageContainer> image, unsigned int textureUnit)
{
    // it is a smart pointer ( due to reallocations of the vector )
    auto applier = std::make_shared<TextureApplier>();
    bool ok = applier->prepareData(image, textureUnit);

    if (ok)
        appliers.push_back(applier);

    std::cout << ">>> add new tex to vector: " << image->getName() << std::endl;
    std::cout << image->getName() << std::endl;
    std::cout << "unit: " << textureUnit << std::endl;
    std::cout << "result: " << std::boolalpha << ok << std::endl;
    std::cout << "all tex cout: " << appliers.size() << std::endl << std::endl;

    return ok;
}

void TextureVector::execute()
{
    std::cout << "====" << std::endl;
    for(auto& app : appliers)
    {
        app->execute();
    }
}
