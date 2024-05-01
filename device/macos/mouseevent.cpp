#include "mouseevent.h"

namespace device::emulation {

mouse_event::~mouse_event() {
    CFRelease(mouse_event_);
}

void mouse_event::move(CGPoint position, CGEventType type) {
    mouse_event event =
        CGEventCreateMouseEvent(nullptr, type, position,
                                kCGMouseButtonLeft /* ignored? */);
    event.post();
}

void mouse_event::scroll(int y, int x) {
    mouse_event event =
        CGEventCreateScrollWheelEvent(nullptr, kCGScrollEventUnitLine, 1, y, x);
    event.post();
}

void mouse_event::click(CGPoint position, CGEventType type) {
    mouse_event event =
        CGEventCreateMouseEvent(nullptr, type, position, kCGMouseButtonCenter);
    event.post();
}

void mouse_event::post() {
    CGEventPost(kCGSessionEventTap, mouse_event_);
}

} // namespace device::emulation
