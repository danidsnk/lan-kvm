#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

#include <CoreGraphics/CoreGraphics.h>

namespace device::emulation {

class mouse_event {
public:
    mouse_event(const mouse_event &) = delete;
    mouse_event &operator=(const mouse_event &) = delete;
    mouse_event(mouse_event &&) = delete;
    mouse_event &operator=(mouse_event &&) = delete;
    ~mouse_event();

    static void move(CGPoint position, CGEventType type = kCGEventMouseMoved);
    static void click(CGPoint position, CGEventType type /* , CGMouseButton button */);
    static void scroll(int y, int x = 0);

private:
    mouse_event() = default;
    void post();

    CGEventRef mouse_event_ = nullptr;
};

} // namespace device::emulation

#endif
