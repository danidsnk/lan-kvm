#ifndef KEYBOARDEVENT_H
#define KEYBOARDEVENT_H

#include <CoreGraphics/CoreGraphics.h>

namespace device::emulation {

class keyboard_event {
public:
    keyboard_event(const keyboard_event &) = delete;
    keyboard_event &operator=(const keyboard_event &) = delete;
    keyboard_event(keyboard_event &&) = delete;
    keyboard_event &operator=(keyboard_event &&) = delete;
    ~keyboard_event();

    static void press(CGKeyCode code, CGEventFlags mods, bool pressed = true);
    static void release(CGKeyCode code, CGEventFlags mods);

private:
    keyboard_event(CGEventRef event) : key_event_{ event } {}

    void post(CGEventFlags mods);

    CGEventRef key_event_ = nullptr;
};

} // namespace device::emulation

#endif
