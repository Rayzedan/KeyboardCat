#include "handler.h"
#include <cstring>
#include <iostream>

sig_atomic_t g_stop = 0;

LinuxHandler& LinuxHandler::Instance()
{
    static LinuxHandler handler;
    return handler;
}

void LinuxHandler::handle_signal(int)
{
    g_stop = 1;
}

LinuxHandler::LinuxHandler() : m_stream("/dev/input/event3")
{
    if (!m_stream)
    {
        throw std::runtime_error("Can't open /dev/input/event3");
    }
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
}

bool LinuxHandler::HasInput()
{
    m_stream.read(m_dataInput.data(), m_dataInput.size());
    std::memcpy(&m_event, m_dataInput.data(), m_dataInput.size());
    return m_event.type == EV_KEY && m_event.value == 1;
}

bool LinuxHandler::HasStop()
{
    return g_stop;
}

void LinuxHandler::Stop()
{
    g_stop = 1;
}
