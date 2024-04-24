#include "virtualkeyboard.h"
#include "codekeykvk.h"
#include "keyboardevent.h"

namespace device::emulation {

enum TYPE {
    KEY = 1,
};

enum KEY_STATE {
    PRESSED = 1,
    RELEASED = 0,
    HOLDED = 2,
};

void virtual_keyboard::process_event(device_event event) {
    auto code = codekey_to_kvk(event.code);

    bool valid = event.type == TYPE::KEY &&
                 code != ERROR_CODE;
    if (!valid) {
        return;
    }

    auto mods = process_modifiers(code, event.value);
    switch (event.value) {
    case KEY_STATE::PRESSED:
    case KEY_STATE::HOLDED:
        keyboard_event::press(code, mods);
        break;
    case KEY_STATE::RELEASED:
        keyboard_event::release(code, mods);
        break;
    }
}

CGEventFlags virtual_keyboard::process_modifiers(CGKeyCode code, int value) {
    CGEventFlags mod = 0;
    switch (code) {
    case kVK_Shift:
    case kVK_RightShift: {
        mod = kCGEventFlagMaskShift;
        break;
    }
    case kVK_Control:
    case kVK_RightControl: {
        mod = kCGEventFlagMaskControl;
        break;
    }
    case kVK_Option:
    case kVK_RightOption: {
        constexpr auto REAL_ALT =
            kCGEventFlagMaskAlternate | kCGEventFlagMaskNonCoalesced | NX_DEVICELALTKEYMASK;
        mod = REAL_ALT;
        break;
    }
    case kVK_Command:
    case kVK_RightCommand: {
        mod = kCGEventFlagMaskCommand;
        break;
    }
    default:
        return modifiers_;
    }

    switch (value) {
    case KEY_STATE::PRESSED:
        modifiers_ |= mod;
        break;
    case KEY_STATE::RELEASED:
        modifiers_ &= ~mod;
        break;
    default:
        break;
    }
    return modifiers_;
}

} // namespace device::emulation
