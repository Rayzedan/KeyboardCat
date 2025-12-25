#include "keyboard_cat/config/config.h"
#include "keyboard_cat/gif/gif.h"
#include "keyboard_cat/handler/handler.h"
#include "keyboard_cat/init/init.h"
#include "keyboard_cat/logger/logger.h"
#include "keyboard_cat/renderer/renderer.h"
#include "keyboard_cat/ui/tray.h"
#include "keyboard_cat/ui/window.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_timer.h>
#include <vector>

#ifdef CONFIG_PATH
constexpr std::string_view g_config_path = CONFIG_PATH;
#else
constexpr std::string_view g_config_path = "config.toml";
#endif

int main()
{
    try
    {
        Logger::Init();
        LOG_INFO("Starting Keyboard Cat application");

        Initializer init;
        LOG_DEBUG("SDL initialized");

        Config config;
        auto params = config.Load(g_config_path);
        LOG_DEBUG("Config loaded from: {}", std::string(g_config_path));

        Window& window = Window::Instance(params);
        LOG_DEBUG("Window created");

        GifLoader loader;
        const auto frames = loader.GetFrames();
        LOG_DEBUG("Loaded {} GIF frames", frames.size());

        Renderer renderer(window.GetRawWindow(), frames);
        LOG_DEBUG("Renderer initialized");

        auto handler = make_handler();
        if (!handler)
        {
            throw std::runtime_error("Failed to create keyboard handler for this platform");
        }
        LOG_DEBUG("Keyboard handler created");

        Tray tray;
        LOG_DEBUG("System tray initialized");
        renderer.Render();
        bool running = true;
        Uint64 lastSaveTime = SDL_GetTicksNS() / 1000000; // Convert to milliseconds
        bool configNeedsSave = false;

        while (running)
        {
            SDL_Event event;
            // Wait for events with timeout to avoid busy-wait and reduce CPU usage
            while (SDL_WaitEventTimeout(&event, g_event_loop_timeout_ms))
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
                    case SDL_EVENT_MOUSE_MOTION:
                        window.Move(event);
                        configNeedsSave = true;
                        break;
                    case SDL_EVENT_MOUSE_BUTTON_UP:
                        window.Move(event);
                        configNeedsSave = true;
                        break;
                }
            }

            // Auto-save config periodically to prevent data loss on crash
            Uint64 currentTime = SDL_GetTicksNS() / 1000000; // Convert to milliseconds
            if (configNeedsSave && (currentTime - lastSaveTime >= g_config_autosave_interval_ms))
            {
                if (config.Save(g_config_path, window.GetCurrentParameters()))
                {
                    LOG_DEBUG("Config auto-saved");
                    configNeedsSave = false;
                    lastSaveTime = currentTime;
                }
                else
                {
                    LOG_WARN("Failed to auto-save config");
                }
            }

            if (handler->HasStop())
            {
                handler->Stop();
                running = false;
                break;
            }
        }

        LOG_INFO("Application shutting down");
        if (!config.Save(g_config_path, window.GetCurrentParameters()))
        {
            LOG_ERROR("Failed to save config on exit");
            return -1;
        }
        LOG_INFO("Config saved successfully");
    }
    catch (const std::exception& e)
    {
        LOG_CRITICAL("Fatal error: {}", e.what());
        return -1;
    }
    LOG_INFO("Application terminated normally");
    return 0;
}
