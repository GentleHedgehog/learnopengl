#pragma once

struct CommonSettings
{
    static inline int width{};
    static inline int height{};

    static inline float getAspectRatio()
    {
        return  height > 0 ? static_cast<float>(width) / height : 0;
    }
};
