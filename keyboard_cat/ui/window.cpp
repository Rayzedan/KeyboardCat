#include "keyboard_cat/ui/window.h"
#include "keyboard_cat/ui/window_helper.h"
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <sstream>
#include <iostream>

Window& Window::Instance(const ApplicationParameters& parameters)
{
    auto [width, height, wX, wY] = parameters.windowSize;
    static Window window(width, height, wX, wY);
    return window;
}

static std::pair<int, int> GetDisplaySize(SDL_Window* window)
{
    SDL_DisplayID display = SDL_GetDisplayForWindow(window);
    const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(display);
    if (mode == nullptr)
    {
        std::stringstream ss;
        ss << "Failed to get parameters of current display: " << SDL_GetError() << '\n';
        throw std::runtime_error(ss.str());
    }
    return { mode->w, mode->h };
}


Window::Window(int width, int height, int wX, int wY)
    : m_isDragging(false),
      m_width(width),
      m_height(height),
      m_wX(wX),
      m_wY(wY),
      m_window(nullptr, SDL_DestroyWindow)
{
    SDL_Window* rawWindow =
        SDL_CreateWindow("Bongo Cat", m_width, m_height,
            SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_BORDERLESS |
            SDL_WINDOW_TRANSPARENT | SDL_WINDOW_NOT_FOCUSABLE);
    m_window.reset(rawWindow);
    if (!m_window)
    {
        std::stringstream ss;
        ss << "Failed to create window: " << SDL_GetError() << '\n';
        throw std::runtime_error(ss.str());
    }
    auto [displayWidth, displayHeight] = GetDisplaySize(m_window.get());
    SDL_SetWindowPosition(m_window.get(), displayWidth - m_width + m_wX, displayHeight - m_height + m_wY);
    ConfigureWindowSpaces(m_window.get());
}

ApplicationParameters Window::GetCurrentParameters() const
{
    auto [displayWidth, displayHeight] = GetDisplaySize(m_window.get());
    int x, y;
    SDL_GetWindowPosition(m_window.get(), &x, &y);
    int offsetX = displayWidth - x - m_width;
    int offsetY = displayHeight - y - m_height;
    return { m_width, m_height, -1 * offsetX, -1 * offsetY };
}

void Window::Move(SDL_Event& event)
{
    switch (event.type)
    {
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                m_isDragging = true;

                m_wX = event.button.x;
                m_wY = event.button.y;
            }
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                m_isDragging = false;
            }
            break;
        case SDL_EVENT_MOUSE_MOTION:
            if (m_isDragging)
            {
                int currentWindowX, currentWindowY;
                SDL_GetWindowPosition(m_window.get(), &currentWindowX, &currentWindowY);

                int deltaX = event.motion.x - m_wX;
                int deltaY = event.motion.y - m_wY;

                SDL_SetWindowPosition(m_window.get(), currentWindowX + deltaX, currentWindowY + deltaY);
            }
            break;
    }
}

SDL_Window* Window::GetRawWindow()
{
    return m_window.get();
}

Window::~Window()
{
}
