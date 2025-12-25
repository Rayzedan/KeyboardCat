#include "renderer.h"

Renderer::Renderer(SDL_Window* window, const std::vector<SDL_Surface*>& frames) :
    m_windowW(0),
    m_windowH(0),
    m_currentFrame(0),
    m_frames(frames),
    m_renderer(nullptr, SDL_DestroyRenderer),
    m_window(window)
{
    if (m_window == nullptr)
    {
        throw std::invalid_argument("Passing invalid window to renderer");
    }
    SDL_Renderer* rawRenderer = SDL_CreateRenderer(m_window, nullptr);
    if (rawRenderer == nullptr)
    {
        throw std::runtime_error("Failed to create renderer");
    }
    m_renderer.reset(rawRenderer);
    SDL_GetWindowSize(m_window, &m_windowW, &m_windowH);
}

void Renderer::Render()
{
    SDL_Renderer* renderPtr = m_renderer.get();
    SDL_SetRenderDrawColor(renderPtr, 0, 0, 0, 0);
    SDL_RenderClear(renderPtr);

    auto surface = m_frames[m_currentFrame];
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderPtr, surface);
    if (texture)
    {
        SDL_FRect dstRect = {
            (m_windowW - surface->w) / 2.0f,
            (m_windowH - surface->h) / 2.0f,
            static_cast<float>(surface->w),
            static_cast<float>(surface->h)
        };
        SDL_RenderTexture(renderPtr, texture, nullptr, &dstRect);
        SDL_DestroyTexture(texture);
    }
    SDL_RenderPresent(renderPtr);
}

void Renderer::Update()
{
    if (m_currentFrame + 1 >= m_frames.size())
    {
        m_currentFrame = 0;
        return;
    }
    ++m_currentFrame;
}
