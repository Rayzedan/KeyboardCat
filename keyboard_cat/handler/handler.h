#ifndef HANDLER_H
#define HANDLER_H

#include <memory>

class BaseHandler
{
public:
    virtual ~BaseHandler() = default;
    virtual void CheckInput() = 0;
    virtual bool HasStop() = 0;
    virtual void Stop() = 0;
};

std::unique_ptr<BaseHandler> make_handler();

#endif
