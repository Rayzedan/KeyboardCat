#include "handler.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_haptic.h"

#include <cstring>
#include <signal.h>
#include <iostream>

sig_atomic_t g_stop = 0;

void LinuxHandler::handle_signal(int)
{
    SDL_Event e;
    e.type = SDL_EVENT_QUIT;
    SDL_PushEvent(&e);
    g_stop = 1;
}

LinuxHandler::LinuxHandler()
{
    SDL_KeyboardID* id = SDL_GetKeyboards(NULL);
    if (id == nullptr)
    {
        throw std::runtime_error("Can't get keyboard ID");
    }
    m_stream.open("/dev/input/event" + std::to_string(*id));
    if (!m_stream)
    {
        throw std::runtime_error("Can't open /dev/input/event" + std::to_string(*id));
    }
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
}

bool LinuxHandler::HasInput()
{
    if (!g_stop)
    {
        m_stream.read(m_dataInput.data(), m_dataInput.size());
        std::memcpy(&m_event, m_dataInput.data(), m_dataInput.size());
        return m_event.type == EV_KEY && m_event.value == 1;
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
