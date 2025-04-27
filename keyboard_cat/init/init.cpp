#include "keyboard_cat/init/init.h"
#include <SDL3/SDL_init.h>
#include <sstream>

Initializer::Initializer()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::stringstream ss;
        ss << "Failed to initialize SDL: " << SDL_GetError() << '\n';
        throw std::runtime_error(ss.str());
    }
}

Initializer::~Initializer()
{
    SDL_Quit();
}
