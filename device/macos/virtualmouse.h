#ifndef VIRTUALMOUSE_H
#define VIRTUALMOUSE_H

#include "deviceevent.h"
#include <CoreGraphics/CoreGraphics.h>

namespace device::emulation {

class virtual_mouse {
public:
    //! cursor_speed must be greater than 0
    explicit virtual_mouse(double cursor_speed = 1)
        : cursor_speed_multiplier_(cursor_speed) {}

    [[nodiscard]] static bool is_valid(device_event ev);
    void process_event(device_event ev);

private:
    void process_axis(device_event ev);
    void process_button(device_event ev);
    [[nodiscard]] CGEventType move_or_drag() const;
    [[nodiscard]] static CGEventType code_to_button(device_event ev);
    [[nodiscard]] static CGSize display_size();
    [[nodiscard]] static CGPoint validate_position(double x, double y);

    CGPoint cursor_position_{};
    double cursor_speed_multiplier_;
    CGEventType last_button_event_ = kCGEventNull;
};

} // namespace device::emulation

#endif
