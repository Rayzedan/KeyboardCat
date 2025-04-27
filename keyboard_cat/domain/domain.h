#ifndef DOMAIN_H
#define DOMAIN_H

constexpr int g_default_width = 200;
constexpr int g_default_height = 123;
constexpr int g_default_offset_x = -3;
constexpr int g_default_offset_y = -180;

struct WindowSize
{
    const int windowWidth = g_default_width;
    const int windowHeight = g_default_height;
    int offsetX = g_default_offset_x;
    int offsetY = g_default_offset_y;
};

struct ApplicationParameters
{
    WindowSize windowSize;
};

#endif // !DOMAIN_H
