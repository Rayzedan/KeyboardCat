#ifndef LINUX_HANDLER_H
#define LINUX_HANDLER_H

#include "bongo_cat/handler/handler.h"
#include <linux/input.h>
#include <signal.h>
#include <fstream>
#include <array>

class LinuxHandler : public BaseHandler
{
public:
    static LinuxHandler& Instance();
    bool HasInput() override;
    bool HasStop() override;
    void Stop() override;
private:
    LinuxHandler();
    static void handle_signal(int);
private:
    std::ifstream m_stream;
    std::array<char, sizeof(input_event)> m_dataInput;
    struct input_event m_event;
};

#endif
