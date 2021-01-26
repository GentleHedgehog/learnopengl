#pragma once

#include "image_framework.h"
#include <memory>

class TextureApplier
{
public:
    TextureApplier();
    ~TextureApplier();

    bool loadFromRGB(std::shared_ptr<ImageContainer> image);

    void execute();

private:
    unsigned int texture{};
};

