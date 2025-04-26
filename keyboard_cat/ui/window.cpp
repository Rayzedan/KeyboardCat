#include "window.h"
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL.h>
#include <sstream>

Window& Window::Instance(int width, int heigth, int wX, int wY)
{
    static Window window(width, heigth, wX, wY);
    return window;
}


Window::Window(int width, int heigth, int wX, int wY)
    : m_width(width), m_heigth(heigth), m_wX(wX), m_wY(wY)
{
    m_window =
        std::make_unique<SDL_Window*>(SDL_CreateWindow("Bongo Cat", m_width, m_heigth,
            SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_BORDERLESS |
            SDL_WINDOW_TRANSPARENT | SDL_WINDOW_RESIZABLE));
    if (*m_window == nullptr)
    {
        std::stringstream ss;
        ss << "Failed to create window: " << SDL_GetError() << '\n';
        throw std::runtime_error(ss.str());
    }
    SDL_DisplayID display = SDL_GetDisplayForWindow(*m_window);
    const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(display);
    if (mode == nullptr)
    {
        std::stringstream ss;
        ss << "Failed to get parameters of current display: " << SDL_GetError() << '\n';
        throw std::runtime_error(ss.str());
    }
     // SDL_SetWindowPosition(*m_window, mode->w / 2 - m_width / 2, mode->h / 2 - m_heigth / 2);
    SDL_SetWindowPosition(*m_window, mode->w - m_width + m_wX, mode->h - m_heigth + m_wY);
}

SDL_Window* Window::GetRawWindow()
{
    return *m_window;
}

Window::~Window()
{
    if (m_window)
    {
        SDL_DestroyWindow(*m_window);
    }
}
