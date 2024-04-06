#include <fcntl.h>
#include <iostream>
#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>
#include <string>
#include <unistd.h>
#include <chrono>
#include <thread>

// foostan Corne
//
// Logitech G305
// up type 2 code 1 value -1
// down type 2 code 1 value 1
// left type 2 code 0 value -1
// right type 2 code 0 value 1

class file_descriptor {
public:
    file_descriptor() = default;
    file_descriptor(const std::string &path, int flags) {
        if (_fd = open(path.c_str(), flags); _fd == -1) {
            throw std::runtime_error("Failed to open file descriptor");
        }
    }
    file_descriptor(const file_descriptor &) = delete;
    file_descriptor &operator=(const file_descriptor &) = delete;
    file_descriptor(file_descriptor &&other) noexcept
        : _fd(other._fd) {
        other._fd = -1;
    }
    file_descriptor &operator=(file_descriptor &&other) noexcept {
        if (this != &other) {
            close();
            _fd = other._fd;
            other._fd = -1;
        }
        return *this;
    }
    ~file_descriptor() { close(); }
    operator int() const { return _fd; }

private:
    void close() const {
        if (_fd != -1) {
            ::close(_fd);
        }
    }
    int _fd = -1;
};

class evdev_device {
public:
    evdev_device() = default;
    explicit evdev_device(const std::string &path, int flags)
        : _fd(path, flags) {
        if (libevdev_new_from_fd(_fd, &_dev) != 0) {
            throw std::runtime_error("Failed to create libevdev device");
        }
    }
    evdev_device(const evdev_device &) = delete;
    evdev_device &operator=(const evdev_device &) = delete;
    evdev_device(evdev_device &&other) noexcept
        : _fd(std::move(other._fd)), _dev(other._dev) {
        other._dev = nullptr;
    }
    evdev_device &operator=(evdev_device &&other) noexcept {
        if (this != &other) {
            if (_dev != nullptr) {
                libevdev_free(_dev);
            }
            _fd = std::move(other._fd);
            _dev = other._dev;
            other._dev = nullptr;
        }
        return *this;
    }
    ~evdev_device() {
        if (_dev != nullptr) {
            libevdev_free(_dev);
        }
    }
    libevdev *device() { return _dev; }
    std::string name() { return libevdev_get_name(_dev); }
    std::string phys() { return libevdev_get_phys(_dev); }

    struct event_ret {
        int status;
        input_event event;
    };
    event_ret next_event() {
        input_event ev = {};
        const auto flags = LIBEVDEV_READ_FLAG_NORMAL | LIBEVDEV_READ_FLAG_BLOCKING;
        auto status = libevdev_next_event(_dev, flags, &ev);
        return { status, ev };
    }
    static evdev_device create(const std::string &name) {
        evdev_device dev;
        dev._dev = libevdev_new();
        libevdev_set_name(dev._dev, name.c_str());
        // libevdev_enable_property(dev._dev, INPUT_PROP_POINTER);
        return dev;
    }

private:
    libevdev *_dev = nullptr;
    file_descriptor _fd;
};

void process_events(evdev_device &device) {
    auto is_error = [](int status) { return status < 0 && status != -EAGAIN; };
    auto has_next_event = [](int status) { return status >= 0; };

    while (true) {
        auto [status, event] = device.next_event();
        if (is_error(status)) {
            std::cerr << "Failed to get next event" << std::endl;
            break;
        }
        if (!has_next_event(status)) {
            continue;
        }
        std::cout << "type: " << event.type
                  << " code: " << event.code
                  << " value: " << event.value
                  << '\n';
    }
}

evdev_device find_device_by_name(const std::string &name) {
    for (int i = 0;; ++i) {
        try {
            evdev_device dev("/dev/input/event" + std::to_string(i), O_RDWR | O_CLOEXEC);
            if (dev.name() == name) {
                return dev;
            }
        } catch (const std::runtime_error &) {
            break;
        }
    }
    return {};
}

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
        //sleep
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

int main() {
    // auto dev = find_device_by_name("Logitech G305");
    auto dev = find_device_by_name("foostan Corne");
    if (dev.device() == nullptr) {
        std::cerr << "Failed to find device" << std::endl;
        return 1;
    }
    std::cout << "Name: " << dev.name()
              << "\nPhys: " << dev.phys()
              << '\n';

    // std::cout << libevdev_grab(dev.device(), LIBEVDEV_GRAB) << std::endl;
    process_events(dev);


    // demo2();


    return 0;
}
