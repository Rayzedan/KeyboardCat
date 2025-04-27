#include "gif.h"
#include <SDL3/SDL_filesystem.h>
#include <gif_lib.h>
#include <filesystem>

GifLoader::GifLoader()
{
    auto basePathPtr = SDL_GetBasePath();
    if (basePathPtr == nullptr)
    {
        throw std::runtime_error("Can't get sdl base path");
    }
    const std::filesystem::path basePath = basePathPtr;
    static const auto filePath = basePath / "hallow_cat.gif";

    GifFileType* gifFile = DGifOpenFileName(filePath.string().c_str(), nullptr);
    if (!gifFile)
    {
        throw std::runtime_error("Can't find gif file");
    }

    if (DGifSlurp(gifFile) != GIF_OK)
    {
        DGifCloseFile(gifFile, nullptr);
        throw std::runtime_error("Can't extract data from gif file");
    }

    int transparentIndex = -1;
    for (int i = 0; i < gifFile->ImageCount; ++i)
    {
        SavedImage* frame = &gifFile->SavedImages[i];

        for (int j = 0; j < frame->ExtensionBlockCount; ++j)
        {
            ExtensionBlock& ext = frame->ExtensionBlocks[j];
            if (ext.Function == GRAPHICS_EXT_FUNC_CODE && ext.ByteCount >= 4)
            {
                bool transparencyFlag = ext.Bytes[0] & 0b00000001;
                if (transparencyFlag)
                {
                    transparentIndex = static_cast<unsigned char>(ext.Bytes[3]);
                }
            }
        }

        ColorMapObject* colorMap =
            frame->ImageDesc.ColorMap ? frame->ImageDesc.ColorMap : gifFile->SColorMap;

        SDL_Surface* surface = SDL_CreateSurface(frame->ImageDesc.Width,
            frame->ImageDesc.Height, SDL_PIXELFORMAT_ARGB8888);
        if (!surface)
        {
            throw std::runtime_error("Failed to create surface for frame");
        }

        Uint32* pixels = static_cast<Uint32*>(surface->pixels);
        for (int y = 0; y < frame->ImageDesc.Height; ++y)
        {
            for (int x = 0; x < frame->ImageDesc.Width; ++x)
            {
                int pixelIndex = y * frame->ImageDesc.Width + x;
                if (pixelIndex >= frame->ImageDesc.Width * frame->ImageDesc.Height)
                    continue;

                GifByteType colorIndex = frame->RasterBits[pixelIndex];
                if (colorIndex >= colorMap->ColorCount)
                    continue;

                GifColorType color = colorMap->Colors[colorIndex];

                Uint8 alpha = (colorIndex == transparentIndex) ? 0x00 : 0xFF;

                Uint32 pixelColor =
                    (alpha << 24) | (color.Red << 16) | (color.Green << 8) | (color.Blue);
                pixels[pixelIndex] = pixelColor;
            }
        }

        m_frames.push_back(surface);
    }
    DGifCloseFile(gifFile, nullptr);
}

GifLoader::~GifLoader()
{
    for (SDL_Surface* ptr : m_frames)
    {
        if (ptr != nullptr)
        {
            SDL_DestroySurface(ptr);
        }
    }
}

const std::vector<SDL_Surface*>& GifLoader::GetFrames() const
{
    if (m_frames.empty())
    {
        throw std::runtime_error("Failed to load GIF: " + std::string(SDL_GetError()));
    }
    return m_frames;
}
