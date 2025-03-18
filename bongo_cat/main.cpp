#include "bongo_cat/gif/gif.h"
#include "bongo_cat/handler/handler.h"
#include "bongo_cat/renderer/renderer.h"
#include "bongo_cat/ui/tray.h"
#include "bongo_cat/ui/window.h"
#include <SDL3/SDL.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <thread>
#include <atomic>

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

int main()
{
    try
    {
        Initializer init;
        Window& window = Window::Instance(249, 153, -3, -180);
        GifLoader loader;
        const auto frames = loader.GetFrames();
        if (frames.empty())
        {
            std::cerr << "Failed to load GIF: " << SDL_GetError() << std::endl;
            return -1;
        }
        Renderer renderer(window.GetRawWindow(), frames);
        auto handler = make_handler();
        Tray tray(handler);
        std::atomic<bool> running = true;
        std::thread sdlThread([&running]
        {
            while (running)
            {
                SDL_UpdateTrays();
                SDL_Delay(100);
            }
        });
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
        sdlThread.join();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Get error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
