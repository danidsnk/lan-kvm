#include "evdev.h"
#include <chrono>
#include <iostream>
#include <libevdev/libevdev-uinput.h>
#include <libevdev/libevdev.h>
#include <string>
#include <thread>

void demo() {
    auto dev = evdev_device::create("Virtual Mouse");
    libevdev_enable_property(dev.device(), INPUT_PROP_POINTER);

    libevdev_enable_event_type(dev.device(), EV_REL);
    libevdev_enable_event_code(dev.device(), EV_REL, REL_X, nullptr);
    libevdev_enable_event_code(dev.device(), EV_REL, REL_Y, nullptr);
    libevdev_enable_event_code(dev.device(), EV_REL, REL_WHEEL, nullptr);

    libevdev_enable_event_type(dev.device(), EV_KEY);
    libevdev_enable_event_code(dev.device(), EV_KEY, BTN_LEFT, nullptr);
    libevdev_enable_event_code(dev.device(), EV_KEY, BTN_RIGHT, nullptr);
    libevdev_enable_event_code(dev.device(), EV_KEY, BTN_MIDDLE, nullptr);

    libevdev_uinput *uinput = nullptr;
    int r = libevdev_uinput_create_from_device(dev.device(), LIBEVDEV_UINPUT_OPEN_MANAGED, &uinput);
    if (r != 0) {
        std::cerr << "Failed to create uinput device" << std::endl;
        return;
    }

    auto move = [&](int x, int y) {
        libevdev_uinput_write_event(uinput, EV_REL, REL_X, x);
        libevdev_uinput_write_event(uinput, EV_REL, REL_Y, y);
        libevdev_uinput_write_event(uinput, EV_SYN, SYN_REPORT, 0);
    };

    for (int i = 0; i < 1000; ++i) {
        move(-2, -2);
        // sleep
        std::this_thread::sleep_for(std::chrono::milliseconds(6));
    }

    libevdev_uinput_destroy(uinput);
}

void demo2() {
    auto dev = evdev_device::create("Virtual Keyboard");
    libevdev_enable_property(dev.device(), INPUT_PROP_POINTER);

    libevdev_enable_event_type(dev.device(), EV_KEY);
    // libevdev_enable_event_code(dev.device(), EV_KEY, KEY_A, nullptr);

    // Enable all keys
    for (int i = 0; i < 240; ++i) {
        libevdev_enable_event_code(dev.device(), EV_KEY, i, nullptr);
    }

    libevdev_uinput *uinput = nullptr;
    int r = libevdev_uinput_create_from_device(dev.device(), LIBEVDEV_UINPUT_OPEN_MANAGED, &uinput);
    if (r != 0) {
        std::cerr << "Failed to create uinput device" << std::endl;
        return;
    }

    auto tap_key = [&](int key) {
        libevdev_uinput_write_event(uinput, EV_KEY, key, 1);
        libevdev_uinput_write_event(uinput, EV_SYN, SYN_REPORT, 0);
        libevdev_uinput_write_event(uinput, EV_KEY, key, 0);
        libevdev_uinput_write_event(uinput, EV_SYN, SYN_REPORT, 0);
    };

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    // hold key
    libevdev_uinput_write_event(uinput, EV_KEY, KEY_A, 1);
    libevdev_uinput_write_event(uinput, EV_SYN, SYN_REPORT, 0);
    // libevdev_uinput_write_event(uinput, EV_KEY, KEY_A, 2);
    // libevdev_uinput_write_event(uinput, EV_SYN, SYN_REPORT, 1);

    for (int i = 0; i < 10; ++i) {
        tap_key(KEY_W);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    libevdev_uinput_write_event(uinput, EV_KEY, KEY_A, 0);
    libevdev_uinput_write_event(uinput, EV_SYN, SYN_REPORT, 0);

    libevdev_uinput_destroy(uinput);
}

// int main() {
//     try {
//         // auto dev = find_device_by_name("Logitech G305");
//         auto dev = evdev_device::find_by_name("foostan Corne");
//         std::cout << "Name: " << dev.name()
//                   << "\nPhys: " << dev.phys()
//                   << '\n';
//
//         // std::cout << libevdev_grab(dev.device(), LIBEVDEV_GRAB) << std::endl;
//         dev.process_events([](const input_event &event) {
//             std::cout << "Type: " << event.type
//                       << ", Code: " << event.code
//                       << ", Value: " << event.value
//                       << '\n';
//         });
//     } catch (std::exception &e) {
//         std::cerr << e.what() << std::endl;
//         return 1;
//     }
//
//     // demo2();
//
//     return 0;
// }
