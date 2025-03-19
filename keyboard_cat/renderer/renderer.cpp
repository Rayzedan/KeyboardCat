#include "renderer.h"
#include <sstream>

Renderer::Renderer(SDL_Window* window, const std::vector<SDL_Surface*>& frames) : m_currentFrame(0), m_frames(frames)
{
    if (window == nullptr)
    {
        throw std::invalid_argument("Passing nullptr window to renderer");
    }
    m_renderer = std::make_unique<SDL_Renderer*>(SDL_CreateRenderer(window, nullptr));
    if (!m_renderer)
    {
        std::stringstream ss;
        ss << "Failed to create renderer: " << SDL_GetError() << '\n';
        throw std::runtime_error(ss.str());
    }
}

void Renderer::Render()
{
    SDL_Renderer* renderPtr = *m_renderer;
    SDL_SetRenderDrawColor(renderPtr, 0, 0, 0, 0);
    SDL_RenderClear(renderPtr);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderPtr, m_frames[m_currentFrame]);
    if (texture)
    {
        SDL_RenderTexture(renderPtr, texture, nullptr, nullptr);
        SDL_DestroyTexture(texture);
    }
    SDL_RenderPresent(renderPtr);
}

void Renderer::Update()
{
    m_currentFrame = (m_currentFrame == 0) ? 3 : 0;
}
