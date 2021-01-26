#pragma once

#include <optional>


class ImageContainer
{
public:
    explicit ImageContainer(const std::string& fileName);
    ~ImageContainer();

    const unsigned char* getData() const;

    int getWidth() const;
    int getHeight() const;

private:
    std::optional<unsigned char*> data{};
    int width{}, height{}, nrChannels{};
};

class ImageFramework
{
public:
    ImageFramework();
};

