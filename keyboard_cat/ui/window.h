#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_tray.h>
#include <memory>

class Window
{
public:
    static Window& Instance(int width, int heigth, int wX, int wY);
    void Move(SDL_Event& event);
    SDL_Window* GetRawWindow();
private:
    Window(int width, int heigth, int wX, int wY);
    ~Window();
private:
    bool m_isDragging;
    int m_width;
    int m_heigth;
    int m_wX;
    int m_wY;
    int m_mouseX;
    int m_mouseY;
    std::unique_ptr<SDL_Window*> m_window;
};

#endif // !WINDOW_H
