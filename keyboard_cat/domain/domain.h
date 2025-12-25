#ifndef DOMAIN_H
#define DOMAIN_H

#include <string_view>

// Window dimensions and positioning
constexpr int g_default_width = 200;  // Default window width in pixels
constexpr int g_default_height = 123; // Default window height in pixels
constexpr int g_default_offset_x = -3;   // Default horizontal offset from right edge of screen
constexpr int g_default_offset_y = -180; // Default vertical offset from bottom edge of screen

// Resource file paths
constexpr std::string_view g_gif_filename = "hallow_cat.gif";
constexpr std::string_view g_icon_filename = "icon128.bmp";

// Event loop timing
constexpr int g_event_loop_timeout_ms = 16; // ~60 FPS

// Config auto-save interval
constexpr int g_config_autosave_interval_ms = 30000; // Auto-save config every 30 seconds

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
