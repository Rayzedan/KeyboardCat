#include "gif.h"
#include "SDL3/SDL_filesystem.h"
#include <gif_lib.h>
#include <iostream>
#include <filesystem>

std::vector<SDL_Surface*> load_gif_frames()
{
    std::vector<SDL_Surface*> frames;

    auto basePathPtr = SDL_GetBasePath();
    if (basePathPtr == nullptr)
    {
        return frames;
    }
    const std::filesystem::path basePath = basePathPtr;
    static const auto filePath = basePath / "bongo_cat.gif";

    GifFileType* gifFile = DGifOpenFileName(filePath.string().c_str(), nullptr);
    if (!gifFile)
    {
        std::cerr << "Failed to open GIF file: " << filePath << std::endl;
        return frames;
    }

    if (DGifSlurp(gifFile) != GIF_OK)
    {
        DGifCloseFile(gifFile, nullptr);
        return frames;
    }

    for (int i = 0; i < gifFile->ImageCount; ++i)
    {
        SavedImage* frame = &gifFile->SavedImages[i];
        ColorMapObject* colorMap =
            frame->ImageDesc.ColorMap ? frame->ImageDesc.ColorMap : gifFile->SColorMap;

        SDL_Surface* surface = SDL_CreateSurface(frame->ImageDesc.Width,
            frame->ImageDesc.Height, SDL_PIXELFORMAT_ARGB8888);

        if (!surface)
        {
            std::cerr << "Failed to create surface for frame " << i << std::endl;
            continue;
        }
        for (int y = 0; y < frame->ImageDesc.Height; ++y)
        {
            for (int x = 0; x < frame->ImageDesc.Width; ++x)
            {
                int pixelIndex = y * frame->ImageDesc.Width + x;
                GifByteType colorIndex = frame->RasterBits[pixelIndex];
                GifColorType color = colorMap->Colors[colorIndex];
                Uint32 pixelColor;
                if (color.Red >= 240 && color.Green >= 180 && color.Blue >= 180)
                {
                    pixelColor = 0x00000000;
                }
                else
                {
                    pixelColor =
                        (255 << 24) | (color.Red << 16) | (color.Green << 8) | color.Blue;
                }
                Uint32* pixels = (Uint32*)surface->pixels;
                pixels[pixelIndex] = pixelColor;
            }
        }

        frames.push_back(surface);
    }

    DGifCloseFile(gifFile, nullptr);
    return frames;
}
