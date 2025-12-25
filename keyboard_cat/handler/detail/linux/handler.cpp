#include "handler.h"
#include "SDL3/SDL_events.h"

#include <cstring>
#include <signal.h>
#include <atomic>
#include <iostream>

std::atomic<bool> g_stop = false;

void LinuxHandler::handle_signal(int)
{
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
    m_input_thread = std::thread([this]() { while (!g_stop.load()) CheckInput(); });
}

LinuxHandler::~LinuxHandler()
{
    Stop();
    if (m_input_thread.joinable())
    {
        m_input_thread.join();
    }
}

void LinuxHandler::CheckInput()
{
    if (!g_stop.load())
    {
        m_stream.read(m_dataInput.data(), m_dataInput.size());
        std::memcpy(&m_event, m_dataInput.data(), m_dataInput.size());
        if (m_event.type == EV_KEY && m_event.value == 1)
        {
            SDL_Event e;
            e.type = SDL_EVENT_KEY_DOWN;
            SDL_PushEvent(&e);
        }
    }
}

bool LinuxHandler::HasStop()
{
    return g_stop;
}

void LinuxHandler::Stop()
{
    g_stop = true;
    if (m_stream.is_open())
    {
        m_stream.close();
    }
    if (m_input_thread.joinable())
    {
        m_input_thread.join();
    }
}
