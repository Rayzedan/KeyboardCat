#include "bongo_cat/gif/gif.h"
#include "bongo_cat/handler/handler.h"
#include "bongo_cat/renderer/renderer.h"
#include "bongo_cat/window/window.h"
#include <SDL3/SDL.h>
#include <iostream>
#include <vector>
#include <sstream>

struct Initializer
{
    Initializer()
    {
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            std::stringstream ss;
            ss << "Failed to initialize SDL: " << SDL_GetError() << '\n';
            throw std::runtime_error(ss.str());
        }
    }

    ~Initializer()
    {
        SDL_Quit();
    }
};


int main(int argc, char** argv)
{
    Initializer init;
    Window& window = Window::Instance(249, 153, -3, -180);
    const auto frames = load_gif_frames();
    if (frames.empty())
    {
        std::cerr << "Failed to load GIF: " << SDL_GetError() << std::endl;
        return -1;
    }
    Renderer renderer(window.GetRawWindow(), frames);
    auto handler = make_handler();
    bool running = true;
    while (running)
    {
        renderer.Render();
        if (handler->HasStop())
        {
            running = false;
        }
        if (handler->HasInput())
        {
            renderer.Update();
        }
    }
    return 0;
}
