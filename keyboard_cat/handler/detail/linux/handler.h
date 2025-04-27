#ifndef LINUX_HANDLER_H
#define LINUX_HANDLER_H

#include "keyboard_cat/handler/handler.h"
#include <linux/input.h>
#include <fstream>
#include <thread>
#include <array>

class LinuxHandler : public BaseHandler
{
public:
    LinuxHandler();
    ~LinuxHandler();
    void CheckInput() override;
    bool HasStop() override;
    void Stop() override;
private:
    static void handle_signal(int);
private:
    std::ifstream m_stream;
    std::array<char, sizeof(input_event)> m_dataInput;
    struct input_event m_event;
    std::thread m_input_thread;
};

#endif
