#include "handler.h"
#if __linux__
#include "keyboard_cat/handler/detail/linux/handler.h"
#elif __APPLE__
#include "keyboard_cat/handler/detail/macos/handler.h"
#endif

std::unique_ptr<BaseHandler> make_handler()
{
#if __linux__
    return std::make_unique<LinuxHandler>();
#elif __APPLE__
    return std::make_unique<DarwinHandler>();
#else
    static_assert(false, "Unsupported platform: only Linux and macOS are supported");
    return nullptr;
#endif
}
