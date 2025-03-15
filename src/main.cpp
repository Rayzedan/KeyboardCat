#include <SDL3/SDL.h>
#include <SDL3/SDL_properties.h>
#include <SDL3_image/SDL_image.h>
#include <filesystem>
#include <fstream>
#include <gif_lib.h>
#include <iostream>
#include <linux/input.h>
#include <set>
#include <string.h>
#include <vector>

SDL_AppResult SDL_Fail()
{
    SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error %s", SDL_GetError());
    return SDL_APP_FAILURE;
}

std::vector<SDL_Surface*> LoadGifFrames(const char* filePath)
{
    std::vector<SDL_Surface*> frames;

    GifFileType* gifFile = DGifOpenFileName(filePath, nullptr);
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

    std::set<GifColorType> unique_colors;
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


int main(int argc, char** argv)
{
    struct input_event keyboardEvent;
    std::ifstream file("/dev/input/event3");
    char dataInput[sizeof(keyboardEvent)];
    if (!file.is_open())
    {
        return -1;
    }

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    int windowWidth = 249;
    int windowHeight = 153;

    SDL_Window* window = SDL_CreateWindow("Bongo Cat", windowWidth, windowHeight,
        SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_BORDERLESS | SDL_WINDOW_TRANSPARENT);
    if (!window)
    {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return SDL_Fail();
    }
    SDL_SetWindowPosition(window, 1920 - windowWidth - 3, 1200 - windowHeight + 47);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer)
    {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return SDL_Fail();
    }
    auto basePathPtr = SDL_GetBasePath();
    if (basePathPtr == nullptr)
    {
        return SDL_Fail();
    }
    const std::filesystem::path basePath = basePathPtr;
    const auto fontPath = basePath / "bongo_cat.gif";
    auto frames = LoadGifFrames(fontPath.string().c_str());
    if (frames.empty())
    {
        std::cerr << "Failed to load GIF: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return SDL_Fail();
    }

    bool running = true;
    size_t currentFrame = 0;
    while (running)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        SDL_Texture* texture =
            SDL_CreateTextureFromSurface(renderer, frames[currentFrame]);
        if (texture)
        {
            SDL_RenderTexture(renderer, texture, nullptr, nullptr);
            SDL_DestroyTexture(texture);
        }
        SDL_RenderPresent(renderer);
        file.read(dataInput, sizeof(keyboardEvent));
        memcpy(&keyboardEvent, dataInput, sizeof(keyboardEvent));
        if (keyboardEvent.type == EV_KEY && keyboardEvent.value == 1)
        {
            if (keyboardEvent.code == KEY_ESC)
            {
                running = false;
            }
            currentFrame = (currentFrame == 0) ? 3 : 0;
        }
        // while (SDL_PollEvent(&event))
        // {
        //     if (event.type == SDL_EVENT_QUIT)
        //     {
        //         running = false;
        //     }
        // }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
