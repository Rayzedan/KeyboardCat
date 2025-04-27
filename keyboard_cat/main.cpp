#include "keyboard_cat/config/config.h"
#include "keyboard_cat/gif/gif.h"
#include "keyboard_cat/handler/handler.h"
#include "keyboard_cat/init/init.h"
#include "keyboard_cat/renderer/renderer.h"
#include "keyboard_cat/ui/tray.h"
#include "keyboard_cat/ui/window.h"
#include <SDL3/SDL_events.h>
#include <iostream>
#include <vector>

// @TODO: must be configure from CMake
constexpr std::string_view g_config_path = "config.toml";

int main()
{
    try
    {
        Initializer init;
        Config config;
        Window& window = Window::Instance(config.Load(g_config_path));
        GifLoader loader;
        const auto frames = loader.GetFrames();
        Renderer renderer(window.GetRawWindow(), frames);
        auto handler = make_handler();
        Tray tray;
        renderer.Render();
        bool running = true;
        while (running)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                    case SDL_EVENT_QUIT:
                        running = false;
                        handler->Stop();
                        break;
                    case SDL_EVENT_KEY_DOWN:
                        renderer.Update();
                        renderer.Render();
                        break;
                    case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    case SDL_EVENT_MOUSE_BUTTON_UP:
                    case SDL_EVENT_MOUSE_MOTION:
                        window.Move(event);
                        break;
                }
            }
            if (handler->HasStop())
            {
                handler->Stop();
                running = false;
                break;
            }
        }
        if (!config.Save(g_config_path, window.GetCurrentParameters()))
        {
            std::cerr << "Get error while saving new config" << std::endl;
            return -1;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Get error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
