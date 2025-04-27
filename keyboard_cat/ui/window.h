#ifndef WINDOW_H
#define WINDOW_H

#include "keyboard_cat/domain/domain.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_tray.h>
#include <memory>

class Window
{
public:
    static Window& Instance(const ApplicationParameters& parameters);
    void Move(SDL_Event& event);
    SDL_Window* GetRawWindow();
    ApplicationParameters GetCurrentParameters() const;
private:
    Window(int width, int height, int wX, int wY);
    ~Window();
private:
    bool m_isDragging;
    int m_width;
    int m_height;
    int m_wX;
    int m_wY;
    std::unique_ptr<SDL_Window*> m_window;
};

#endif // !WINDOW_H
