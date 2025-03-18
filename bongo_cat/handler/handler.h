#ifndef HANDLER_H
#define HANDLER_H

#include <memory>

class BaseHandler
{
public:
    BaseHandler() = default;
    virtual bool HasInput() = 0;
    virtual bool HasStop() = 0;
    virtual void Stop() = 0;
};

std::shared_ptr<BaseHandler> make_handler();

#endif
