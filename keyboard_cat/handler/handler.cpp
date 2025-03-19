#include "handler.h"
// TODO: add define for UNIX/Windows
#include "keyboard_cat/handler/detail/linux/handler.h"

std::shared_ptr<BaseHandler> make_handler()
{
    return std::make_shared<LinuxHandler>(std::move(LinuxHandler::Instance()));

}
