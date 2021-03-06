#include "image_framework.h"

#define  STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <iostream>

ImageFramework::ImageFramework()
{

}

ImageContainer::ImageContainer(const std::string &fileName, bool isFlipOnLoad)
{
    stbi_set_flip_vertically_on_load(isFlipOnLoad);

    name = fileName;

    auto d = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
    if (d)
    {
        data = d;
        std::cout << "load image " << fileName << ", 0x" << std::hex << (uint64_t)d << std::endl;
        std::cout << "w " << width << " h " << height << " ch " << nrChannels << std::endl;
    }
    else
    {
        std::cout << "failed to load image " << fileName << std::endl;
    }
}

ImageContainer::~ImageContainer()
{
    if (data.has_value())
    {
        stbi_image_free(data.value());
        std::cout << "unload image " << std::hex << (uint64_t)data.value() << std::endl;
    }
}

const unsigned char *ImageContainer::getData() const
{
    if (data.has_value())
    {
        return data.value();
    }
    else
    {
        return nullptr;
    }
}

int ImageContainer::getChannels() const
{
    return nrChannels;
}

std::string ImageContainer::getName() const
{
    return name;
}

int ImageContainer::getWidth() const
{
    return width;
}

int ImageContainer::getHeight() const
{
    return height;
}
