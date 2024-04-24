#ifndef VIRTUALKEYBOARD_H
#define VIRTUALKEYBOARD_H

#include "deviceevent.h"
#include <CoreGraphics/CoreGraphics.h>

namespace device::emulation {

class virtual_keyboard {
public:
    void process_event(device_event event);

private:
    CGEventFlags process_modifiers(CGKeyCode code, int value);

    CGEventFlags modifiers_ = CGEventSourceFlagsState(kCGEventSourceStateHIDSystemState);
};

} // namespace device::emulation

#endif
