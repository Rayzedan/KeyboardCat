#include "tray.h"
#include "SDL3/SDL_filesystem.h"
#include "SDL3_image/SDL_image.h"
#include <filesystem>
#include <sstream>

static void callback_quit(void* handlerPtr, SDL_TrayEntry* invoker)
{
    BaseHandler* handler = static_cast<BaseHandler*>(handlerPtr);
    (void)invoker;
    handler->Stop();
}

Tray::Tray(std::weak_ptr<BaseHandler> handler) : m_handler(handler)
{
    auto basePathPtr = SDL_GetBasePath();
    if (basePathPtr == nullptr)
    {
        std::stringstream ss;
        ss << "Failed to loading system tray icon: " << SDL_GetError() << '\n';
        throw std::runtime_error(ss.str());
    }
    const std::filesystem::path basePath = basePathPtr;
    static const auto filePath = basePath / "icon128.png";
    m_icon = IMG_Load(filePath.string().c_str());
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
    SDL_SetTrayEntryCallback(m_entry, callback_quit, m_handler.lock().get());
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
