#include "handler.h"
#include "SDL3/SDL_events.h"
#include <cstring>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>

sig_atomic_t g_stop = 0;

LinuxHandler& LinuxHandler::Instance()
{
    static LinuxHandler handler;
    return handler;
}

void LinuxHandler::handle_signal(int)
{
    SDL_Event e;
    e.type = SDL_EVENT_QUIT;
    SDL_PushEvent(&e);
    g_stop = 1;
}

LinuxHandler::LinuxHandler() : m_fd(open("/dev/input/event3", O_RDONLY | O_NONBLOCK))
{
    if (m_fd < 0)
    {
        throw std::runtime_error("Can't open /dev/input/event3");
    }
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
}

bool LinuxHandler::HasInput()
{
    if (!g_stop)
    {
        const int res = read(m_fd, m_dataInput.data(), m_dataInput.size());
        if (res < 0)
        {
            return false;
        }
        std::memcpy(&m_event, m_dataInput.data(), m_dataInput.size());
        return (m_event.type == EV_KEY && m_event.value == 1);
    }
    return false;
}

bool LinuxHandler::HasStop()
{
    return g_stop;
}

void LinuxHandler::Stop()
{
    SDL_Event e;
    e.type = SDL_EVENT_QUIT;
    SDL_PushEvent(&e);
    g_stop = 1;
}
