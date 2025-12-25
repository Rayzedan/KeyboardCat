#include "handler.h"
#include <SDL3/SDL_events.h>
#include <cstring>
#include <signal.h>
#include <atomic>

// Global flag for signal handler (signals require static/global context)
std::atomic<bool> g_stop = false;

DarwinHandler::DarwinHandler() : m_event(make_event(this))
{
    if (!CGEventTapIsEnabled(m_event))
    {
        throw std::runtime_error("Error, event tap is not enabled");
    }
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    m_handler_thread = std::thread([this]() { launch(m_event); });
    m_input_thread = std::thread([this]() { while (!g_stop) CheckInput(); });
}

DarwinHandler::~DarwinHandler()
{
    Stop();
    if (m_event)
    {
        CFRelease(m_event);
    }
    if (m_handler_thread.joinable())
    {
        m_handler_thread.join();
    }
    if (m_input_thread.joinable())
    {
        m_input_thread.join();
    }
}

void DarwinHandler::handle_signal(int sig)
{
    (void)sig;
    g_stop = true;
}

CGEventRef DarwinHandler::keyboard_callback([[maybe_unused]] CGEventTapProxy proxy,
    CGEventType type, CGEventRef event, void* userInfo)
{
    auto* handler = static_cast<DarwinHandler*>(userInfo);
    if (type == kCGEventKeyDown)
    {
        {
            std::lock_guard<std::mutex> lock(handler->m_mutex);
            handler->m_inputFlag = true;
        }
        handler->m_cv.notify_one();
    }

    return event;
}

CFMachPortRef DarwinHandler::make_event(DarwinHandler* self)
{
    return CGEventTapCreate(kCGHIDEventTap, kCGHeadInsertEventTap,
        kCGEventTapOptionListenOnly, CGEventMaskBit(kCGEventKeyDown), keyboard_callback,
        self);
}

void DarwinHandler::launch(CFMachPortRef eventTap)
{
    CFRunLoopSourceRef source =
        CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    if (!source)
    {
        throw std::runtime_error("Failed to create run loop source");
    }
    m_runLoop = CFRunLoopGetCurrent();
    CFRunLoopAddSource(CFRunLoopGetCurrent(), source, kCFRunLoopCommonModes);
    CFRunLoopRun();
    m_runLoop = nullptr;
}

void DarwinHandler::CheckInput()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cv.wait(lock, [this]() { return m_inputFlag || g_stop; });

    if (!g_stop.load())
    {
        m_inputFlag = false;
        SDL_Event e;
        e.type = SDL_EVENT_KEY_DOWN;
        SDL_PushEvent(&e);
    }
}

bool DarwinHandler::HasStop()
{
    return g_stop.load();
}

void DarwinHandler::Stop()
{
    g_stop = true;
    m_cv.notify_all();
    if (auto loop = m_runLoop.load())
    {
        CFRunLoopStop(loop);
    }
}
