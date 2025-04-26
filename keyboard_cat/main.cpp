#include "keyboard_cat/gif/gif.h"
#include "keyboard_cat/handler/handler.h"
#include "keyboard_cat/renderer/renderer.h"
#include "keyboard_cat/ui/tray.h"
#include "keyboard_cat/ui/window.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_tray.h>
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
        // TODO: Get settings from user config like json or smth
        Window& window = Window::Instance(200, 123, -3, -180);
        GifLoader loader;
        const auto frames = loader.GetFrames();
        if (frames.empty())
        {
            std::cerr << "Failed to load GIF: " << SDL_GetError() << std::endl;
            return -1;
        }
        Renderer renderer(window.GetRawWindow(), frames);
        auto handler = make_handler();
        Tray tray;
        std::atomic<bool> hasInput = false;
        std::atomic<bool> running = true;
        std::thread inputThread([&handler, &hasInput, &running]
        {
            while (running)
            {
                hasInput = handler->HasInput();
            }
        });
        renderer.Render();
        while (running)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_EVENT_QUIT)
                {
                    running = false;
                    break;
                }
                if (handler->HasStop())
                {
                    running = false;
                    break;
                }
                if (event.type == SDL_EVENT_KEY_DOWN)
                {
                    renderer.Update();
                    renderer.Render();
                    hasInput = false;
                }
            }
        }
        inputThread.join();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Get error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
