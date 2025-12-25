#ifndef DARWIN_HANDLER_H
#define DARWIN_HANDLER_H

#include "keyboard_cat/handler/handler.h"
#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <atomic>
#include <mutex>
#include <thread>

class DarwinHandler : public BaseHandler
{
public:
    DarwinHandler();
    ~DarwinHandler();
    void CheckInput() override;
    bool HasStop() override;
    void Stop() override;
private:
    void launch(CFMachPortRef EventTap);
    static void handle_signal(int);
    static CGEventRef keyboard_callback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* userInfo);
    static CFMachPortRef make_event(DarwinHandler* self);
private:
    bool m_inputFlag = false;
    CFMachPortRef m_event;
    std::atomic<CFRunLoopRef> m_runLoop{nullptr};
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::thread m_handler_thread;
    std::thread m_input_thread;
};

#endif
