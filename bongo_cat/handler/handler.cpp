#include "handler.h"
#include "bongo_cat/handler/detail/linux/handler.h"

std::unique_ptr<BaseHandler> make_handler()
{
    return std::make_unique<LinuxHandler>(std::move(LinuxHandler::Instance()));
}
