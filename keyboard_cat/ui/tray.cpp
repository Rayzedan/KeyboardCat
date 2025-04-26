#include "tray.h"
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL.h>
#include <filesystem>
#include <sstream>

static void callback_quit([[maybe_unused]] void* handlerPtr, [[maybe_unused]] SDL_TrayEntry* invoker)
{
    std::exit(0);
}

Tray::Tray()
{
    auto basePathPtr = SDL_GetBasePath();
    if (basePathPtr == nullptr)
    {
        std::stringstream ss;
        ss << "Failed to loading system tray icon: " << SDL_GetError() << '\n';
        throw std::runtime_error(ss.str());
    }
    const std::filesystem::path basePath = basePathPtr;
    static const auto filePath = basePath / "icon128.bmp";
    m_icon = SDL_LoadBMP(filePath.string().c_str());
    if (m_icon == nullptr)
    {
        std::stringstream ss;
        ss << "Failed to loading system tray icon: " << SDL_GetError() << '\n';
        throw std::runtime_error(ss.str());
    }
    m_system_tray = std::make_unique<SDL_Tray*>(SDL_CreateTray(m_icon, "keyboard_cat"));
    if (!m_system_tray)
    {
        std::stringstream ss;
        ss << "Failed to create system tray: " << SDL_GetError() << '\n';
        throw std::runtime_error(ss.str());
    }
    m_menu = SDL_CreateTrayMenu(*m_system_tray);
    if (m_menu == nullptr)
    {
        std::stringstream ss;
        ss << "Failed to create system menu: " << SDL_GetError() << '\n';
        throw std::runtime_error(ss.str());
    }
    m_entry = SDL_InsertTrayEntryAt(m_menu, -1, "Stop", SDL_TRAYENTRY_BUTTON);
    SDL_SetTrayEntryCallback(m_entry, callback_quit, NULL);
}

Tray::~Tray()
{
    if (m_icon)
    {
        SDL_DestroySurface(m_icon);
    }
    if (m_system_tray)
    {
        SDL_DestroyTray(*m_system_tray);
    }
}
