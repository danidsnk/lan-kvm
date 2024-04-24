#include "keyboardevent.h"

namespace device::emulation {

keyboard_event::~keyboard_event() {
    CFRelease(key_event_);
}

void keyboard_event::press(CGKeyCode code, CGEventFlags mods, bool pressed) {
    keyboard_event event;
    event.key_event_ = CGEventCreateKeyboardEvent(nullptr, code, pressed);
    CGEventSetFlags(event.key_event_, mods);
    CGEventPost(kCGSessionEventTap, event.key_event_);
}

void keyboard_event::release(CGKeyCode code, CGEventFlags mods) {
    press(code, mods, false);
}

} // namespace device::emulation
