#pragma once

#include <optional>


class ImageContainer
{
public:
    explicit ImageContainer(const std::string& fileName, bool isFlipOnLoad = false);
    ~ImageContainer();

    const unsigned char* getData() const;

    int getWidth() const;
    int getHeight() const;
    int getChannels() const;

    std::string getName() const;

private:
    std::optional<unsigned char*> data{};
    int width{}, height{}, nrChannels{};
    std::string name;
};

class ImageFramework
{
public:
    ImageFramework();
};

