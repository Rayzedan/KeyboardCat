#pragma once

#include <SDL3/SDL_video.h>

#if defined(__APPLE__)
void ConfigureWindowSpaces(SDL_Window* window);
#else
inline void ConfigureWindowSpaces(SDL_Window*) {}
#endif
