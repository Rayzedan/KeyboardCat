#ifndef TRAY_H
#define TRAY_H

#include "keyboard_cat/handler/handler.h"
#include <SDL3/SDL_tray.h>
#include <memory>

class Tray
{
public:
    Tray();
    ~Tray();
private:
    SDL_Surface* m_icon;
    std::unique_ptr<SDL_Tray*> m_system_tray;
    SDL_TrayMenu* m_menu;
    SDL_TrayEntry* m_entry;
};

#endif // !TRAY_H
