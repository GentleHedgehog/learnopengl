#pragma once

#include "image_framework.h"
#include <memory>
#include <vector>
#include <glad/glad.h>

class TextureApplier
{
public:
    TextureApplier();
    ~TextureApplier();

    bool prepareData(std::shared_ptr<ImageContainer> image, unsigned int textureUnit)
    {
        setLocation(textureUnit);
        if (image->getChannels() == 3)
        {
            return loadFromRGB(image);
        }
        else if (image->getChannels() == 4)
        {
            return loadFromRGBA(image);
        }
        return false;
    }

    void setLocation(unsigned int textureUnit)
    {
        this->textureUnit = textureUnit;
    }

    bool loadFromRGB(std::shared_ptr<ImageContainer> image);

    bool loadFromRGBA(std::shared_ptr<ImageContainer> image);

    bool loadFromImage(std::shared_ptr<ImageContainer> image, GLint imageFormat);

    void execute();

private:
    unsigned int texture{};
    unsigned int textureUnit{};
};


class TextureVector
{
public:
    TextureVector() = default;

    bool addTexture(std::shared_ptr<ImageContainer> image, unsigned int textureUnit);

    void execute();

private:
    std::vector<std::shared_ptr<TextureApplier>> appliers;
};
