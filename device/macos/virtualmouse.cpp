#include "virtualmouse.h"
#include "mouseevent.h"
#include <stdexcept>

namespace device::emulation {

enum TYPE {
    BUTTON = 1,
    AXIS = 2,
};

enum BUTTON {
    NONE = 0,
    LEFT = 272,
    RIGHT = 273,
    MIDDLE = 274,
};

enum AXIS {
    X = 0,
    Y = 1,
    SCROLL_Y = 11,
};

enum BUTTON_STATE {
    PRESSED = 1,
    RELEASED = 0,
    HOLDED = 2,
};

CGSize virtual_mouse::display_size() {
    auto id = CGMainDisplayID();
    return CGDisplayBounds(id).size;
}

CGPoint virtual_mouse::validate_position(double x, double y) {
    static auto [width, height] = display_size();
    if (x < 0) {
        x = 0;
    } else if (x > width) {
        x = width;
    }
    if (y < 0) {
        y = 0;
    } else if (y > height) {
        y = height;
    }
    return { x, y };
}

bool virtual_mouse::is_valid(device_event ev) {
    bool holded = ev.type == TYPE::BUTTON && ev.value == BUTTON_STATE::HOLDED;
    bool mouse_button = ev.code == BUTTON::LEFT ||
                        ev.code == BUTTON::RIGHT ||
                        ev.code == BUTTON::MIDDLE;
    return (!holded && mouse_button) || ev.type == TYPE::AXIS;
}

void virtual_mouse::process_event(device_event ev) {
    switch (ev.type) {
    case TYPE::AXIS:
        process_axis(ev);
        break;
    case TYPE::BUTTON:
        process_button(ev);
        break;
    default:
        break;
    }
}

inline int scroll_speed(int value) {
    // expected value: 120 or -120
    // CGEventCreateScrollWheelEvent requires max 10 or -10
    // https://developer.apple.com/documentation/coregraphics/1541327-cgeventcreatescrollwheelevent
    constexpr int div = 20;
    return value / div;
}

void virtual_mouse::process_axis(device_event ev) {
    auto [x, y] = cursor_position_;
    switch (ev.code) {
    case AXIS::X:
        x += ev.value * cursor_speed_multiplier_;
        break;
    case AXIS::Y:
        y += ev.value * cursor_speed_multiplier_;
        break;
    case AXIS::SCROLL_Y:
        mouse_event::scroll(scroll_speed(ev.value));
        return;
    default:
        return;
    }
    cursor_position_ = validate_position(x, y);
    mouse_event::move(cursor_position_, move_or_drag());
}

void virtual_mouse::process_button(device_event ev) {
    auto btn = code_to_button(ev);
    mouse_event::click(cursor_position_, btn);
    last_button_event_ = btn;
}

CGEventType virtual_mouse::move_or_drag() const {
    switch (last_button_event_) {
    case kCGEventLeftMouseDown:
        return kCGEventLeftMouseDragged;
    case kCGEventRightMouseDown:
        return kCGEventRightMouseDragged;
    case kCGEventOtherMouseDown:
        return kCGEventOtherMouseDragged;
    default:
        return kCGEventMouseMoved;
    }
}

CGEventType virtual_mouse::code_to_button(device_event ev) {
    switch (ev.code) {
    case BUTTON::LEFT:
        return ev.value == BUTTON_STATE::PRESSED ? kCGEventLeftMouseDown
                                                 : kCGEventLeftMouseUp;
    case BUTTON::RIGHT:
        return ev.value == BUTTON_STATE::PRESSED ? kCGEventRightMouseDown
                                                 : kCGEventRightMouseUp;
    case BUTTON::MIDDLE:
        return ev.value == BUTTON_STATE::PRESSED ? kCGEventOtherMouseDown
                                                 : kCGEventOtherMouseUp;
    default:
        throw std::invalid_argument("Invalid mouse button");
    }
}

}; // namespace device::emulation
