#include "virtualkeyboard.h"
#include "codekeyconverter.h"
#include "keyboardevent.h"
#include <iostream>

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
    if (event.type != TYPE::KEY) {
        return;
    }

    CGKeyCode code{ 0 };
    try {
        code = codekey_to_cgcodekey(event.code);
    } catch (const std::invalid_argument &e) {
        std::cerr << e.what() << std::endl;
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
    case kVK_LeftArrow:
    case kVK_RightArrow:
    case kVK_UpArrow:
    case kVK_DownArrow: {
        mod = kCGEventFlagMaskSecondaryFn;
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
