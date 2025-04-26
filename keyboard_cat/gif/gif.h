#ifndef GIF_H
#define GIF_H

#include <vector>
#include <SDL3/SDL_surface.h>
#include <set>


class GifLoader
{
public:
    GifLoader();
    const std::vector<SDL_Surface*>& GetFrames() const;
    ~GifLoader();
private:
    std::vector<SDL_Surface*> m_frames;
};

#endif // !GIF_H
