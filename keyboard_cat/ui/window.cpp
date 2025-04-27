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
    : m_isDragging(false), m_width(width), m_heigth(heigth), m_wX(wX), m_wY(wY)
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
    SDL_SetWindowPosition(*m_window, mode->w - m_width + m_wX, mode->h - m_heigth + m_wY);
}

void Window::Move(SDL_Event& event)
{
    switch (event.type)
    {
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                m_isDragging = true;

                int windowX, windowY = 0;
                SDL_GetWindowPosition(*m_window, &windowX, &windowY);

                m_wX = event.button.x - windowX;
                m_wY = event.button.y - windowY;
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
                SDL_SetWindowPosition(*m_window, event.motion.x - m_wX,
                    event.motion.y - m_wY);
            }
            break;
    }
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
