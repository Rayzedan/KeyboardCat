#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL_render.h>
#include <memory>
#include <vector>

class Renderer
{
public:
    Renderer(SDL_Window* window, const std::vector<SDL_Surface*>& frames);
    void Render();
    void Update();
private:
    int m_windowW;
    int m_windowH;
    size_t m_currentFrame;
    const std::vector<SDL_Surface*>& m_frames;
    std::unique_ptr<SDL_Renderer*> m_renderer;
    SDL_Window* m_window;
};


#endif // !RENDERER_H
