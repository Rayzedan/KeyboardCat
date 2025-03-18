#include "handler.h"
#include "bongo_cat/handler/detail/linux/handler.h"

std::shared_ptr<BaseHandler> make_handler()
{
    return std::make_shared<LinuxHandler>(std::move(LinuxHandler::Instance()));

}
