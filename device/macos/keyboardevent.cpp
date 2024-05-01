#include "keyboardevent.h"

namespace device::emulation {

keyboard_event::~keyboard_event() {
    CFRelease(key_event_);
}

void keyboard_event::press(CGKeyCode code, CGEventFlags mods, bool pressed) {
    keyboard_event event =
        CGEventCreateKeyboardEvent(nullptr, code, pressed);
    event.post(mods);
}

void keyboard_event::release(CGKeyCode code, CGEventFlags mods) {
    press(code, mods, false);
}

void keyboard_event::post(CGEventFlags mods) {
    CGEventSetFlags(key_event_, mods);
    CGEventPost(kCGSessionEventTap, key_event_);
}

} // namespace device::emulation
