#ifndef EVDEVTOCGKEY_H
#define EVDEVTOCGKEY_H

#include <Carbon/Carbon.h>
#include <cstdint>

namespace device::emulation {

enum CODE_KEY {
    ERROR_CODE = std::numeric_limits<CGKeyCode>::max(),
    KEY_ESC = 1,
    KEY_1 = 2,
    KEY_2 = 3,
    KEY_3 = 4,
    KEY_4 = 5,
    KEY_5 = 6,
    KEY_6 = 7,
    KEY_7 = 8,
    KEY_8 = 9,
    KEY_9 = 10,
    KEY_0 = 11,
    KEY_MINUS = 12,
    KEY_EQUAL = 13,
    KEY_BACKSPACE = 14,
    KEY_TAB = 15,
    KEY_Q = 16,
    KEY_W = 17,
    KEY_E = 18,
    KEY_R = 19,
    KEY_T = 20,
    KEY_Y = 21,
    KEY_U = 22,
    KEY_I = 23,
    KEY_O = 24,
    KEY_P = 25,
    KEY_LEFTBRACE = 26,
    KEY_RIGHTBRACE = 27,
    KEY_ENTER = 28,
    KEY_LEFTCTRL = 29,
    KEY_A = 30,
    KEY_S = 31,
    KEY_D = 32,
    KEY_F = 33,
    KEY_G = 34,
    KEY_H = 35,
    KEY_J = 36,
    KEY_K = 37,
    KEY_L = 38,
    KEY_SEMICOLON = 39,
    KEY_APOSTROPHE = 40,
    KEY_GRAVE = 41,
    KEY_LEFTSHIFT = 42,
    KEY_BACKSLASH = 43,
    KEY_Z = 44,
    KEY_X = 45,
    KEY_C = 46,
    KEY_V = 47,
    KEY_B = 48,
    KEY_N = 49,
    KEY_M = 50,
    KEY_COMMA = 51,
    KEY_DOT = 52,
    KEY_SLASH = 53,
    KEY_RIGHTSHIFT = 54,
    KEY_LEFTALT = 56,
    KEY_SPACE = 57,
    KEY_CAPSLOCK = 58,
    KEY_F1 = 59,
    KEY_F2 = 60,
    KEY_F3 = 61,
    KEY_F4 = 62,
    KEY_F5 = 63,
    KEY_F6 = 64,
    KEY_F7 = 65,
    KEY_F8 = 66,
    KEY_F9 = 67,
    KEY_F10 = 68,
    KEY_NUMLOCK = 69,
    KEY_SCROLLLOCK = 70,
    KEY_F11 = 87,
    KEY_F12 = 88,
    KEY_RIGHTCTRL = 97,
    KEY_RIGHTALT = 100,
    KEY_HOME = 102,
    KEY_UP = 103,
    KEY_PAGEUP = 104,
    KEY_LEFT = 105,
    KEY_RIGHT = 106,
    KEY_END = 107,
    KEY_DOWN = 108,
    KEY_PAGEDOWN = 109,
    KEY_INSERT = 110,
    KEY_DELETE = 111,
    KEY_LEFTMETA = 125,
    KEY_RIGHTMETA = 126,
};

inline CGKeyCode codekey_to_kvk(int key) {
    switch (key) {
    case KEY_ESC:
        return kVK_Escape;
    case KEY_1:
        return kVK_ANSI_1;
    case KEY_2:
        return kVK_ANSI_2;
    case KEY_3:
        return kVK_ANSI_3;
    case KEY_4:
        return kVK_ANSI_4;
    case KEY_5:
        return kVK_ANSI_5;
    case KEY_6:
        return kVK_ANSI_6;
    case KEY_7:
        return kVK_ANSI_7;
    case KEY_8:
        return kVK_ANSI_8;
    case KEY_9:
        return kVK_ANSI_9;
    case KEY_0:
        return kVK_ANSI_0;
    case KEY_MINUS:
        return kVK_ANSI_Minus;
    case KEY_EQUAL:
        return kVK_ANSI_Equal;
    case KEY_BACKSPACE:
        return kVK_Delete;
    case KEY_TAB:
        return kVK_Tab;
    case KEY_Q:
        return kVK_ANSI_Q;
    case KEY_W:
        return kVK_ANSI_W;
    case KEY_E:
        return kVK_ANSI_E;
    case KEY_R:
        return kVK_ANSI_R;
    case KEY_T:
        return kVK_ANSI_T;
    case KEY_Y:
        return kVK_ANSI_Y;
    case KEY_U:
        return kVK_ANSI_U;
    case KEY_I:
        return kVK_ANSI_I;
    case KEY_O:
        return kVK_ANSI_O;
    case KEY_P:
        return kVK_ANSI_P;
    case KEY_LEFTBRACE:
        return kVK_ANSI_LeftBracket;
    case KEY_RIGHTBRACE:
        return kVK_ANSI_RightBracket;
    case KEY_ENTER:
        return kVK_Return;
    case KEY_LEFTCTRL:
        return kVK_Control;
    case KEY_A:
        return kVK_ANSI_A;
    case KEY_S:
        return kVK_ANSI_S;
    case KEY_D:
        return kVK_ANSI_D;
    case KEY_F:
        return kVK_ANSI_F;
    case KEY_G:
        return kVK_ANSI_G;
    case KEY_H:
        return kVK_ANSI_H;
    case KEY_J:
        return kVK_ANSI_J;
    case KEY_K:
        return kVK_ANSI_K;
    case KEY_L:
        return kVK_ANSI_L;
    case KEY_SEMICOLON:
        return kVK_ANSI_Semicolon;
    case KEY_APOSTROPHE:
        return kVK_ANSI_Quote;
    case KEY_GRAVE:
        return kVK_ANSI_Grave;
    case KEY_LEFTSHIFT:
        return kVK_Shift;
    case KEY_BACKSLASH:
        return kVK_ANSI_Backslash;
    case KEY_Z:
        return kVK_ANSI_Z;
    case KEY_X:
        return kVK_ANSI_X;
    case KEY_C:
        return kVK_ANSI_C;
    case KEY_V:
        return kVK_ANSI_V;
    case KEY_B:
        return kVK_ANSI_B;
    case KEY_N:
        return kVK_ANSI_N;
    case KEY_M:
        return kVK_ANSI_M;
    case KEY_COMMA:
        return kVK_ANSI_Comma;
    case KEY_DOT:
        return kVK_ANSI_Period;
    case KEY_SLASH:
        return kVK_ANSI_Slash;
    case KEY_RIGHTSHIFT:
        return kVK_RightShift;
    case KEY_LEFTALT:
        return kVK_Option;
    case KEY_SPACE:
        return kVK_Space;
    case KEY_CAPSLOCK:
        return kVK_CapsLock;
    case KEY_F1:
        return kVK_F1;
    case KEY_F2:
        return kVK_F2;
    case KEY_F3:
        return kVK_F3;
    case KEY_F4:
        return kVK_F4;
    case KEY_F5:
        return kVK_F5;
    case KEY_F6:
        return kVK_F6;
    case KEY_F7:
        return kVK_F7;
    case KEY_F8:
        return kVK_F8;
    case KEY_F9:
        return kVK_F9;
    case KEY_F10:
        return kVK_F10;
    case KEY_F11:
        return kVK_F11;
    case KEY_F12:
        return kVK_F12;
    case KEY_RIGHTCTRL:
        return kVK_RightControl;
    case KEY_RIGHTALT:
        return kVK_RightOption;
    case KEY_HOME:
        return kVK_Home;
    case KEY_UP:
        return kVK_UpArrow;
    case KEY_PAGEUP:
        return kVK_PageUp;
    case KEY_LEFT:
        return kVK_LeftArrow;
    case KEY_RIGHT:
        return kVK_RightArrow;
    case KEY_END:
        return kVK_End;
    case KEY_DOWN:
        return kVK_DownArrow;
    case KEY_PAGEDOWN:
        return kVK_PageDown;
    case KEY_INSERT:
        return kVK_Help;
    case KEY_DELETE:
        return kVK_ForwardDelete;
    case KEY_LEFTMETA:
        return kVK_Command;
    case KEY_RIGHTMETA:
        return kVK_RightCommand;
    default:
        return ERROR_CODE;
    }
}

} // namespace device::emulation

#endif
