#include "keyboard_cat/ui/window_helper.h"

#if defined(__APPLE__)
#import <AppKit/AppKit.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_video.h>

void ConfigureWindowSpaces(SDL_Window* window)
{
    if (!window)
    {
        return;
    }

    SDL_PropertiesID props = SDL_GetWindowProperties(window);
    void* cocoaWindowPtr =
        SDL_GetPointerProperty(props, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
    if (cocoaWindowPtr == nullptr)
    {
        return;
    }

    NSWindow* nsWindow = (__bridge NSWindow*)cocoaWindowPtr;
    NSWindowCollectionBehavior behavior = [nsWindow collectionBehavior];

    // Keep the widget visible on every Space; avoid conflicting flags.
    behavior &= ~NSWindowCollectionBehaviorMoveToActiveSpace;
    behavior |= NSWindowCollectionBehaviorCanJoinAllSpaces;
    behavior |= NSWindowCollectionBehaviorStationary;
    [nsWindow setCollectionBehavior:behavior];
    [nsWindow setLevel:NSStatusWindowLevel];
}
#endif
