#include "evdev.h"
#include <fcntl.h>
#include <stdexcept>
#include <unistd.h>

// foostan Corne
//
// Logitech G305
// up type 2 code 1 value -1
// down type 2 code 1 value 1
// left type 2 code 0 value -1
// right type 2 code 0 value 1

file_descriptor::file_descriptor(const std::string &path, int flags) {
    if (_fd = open(path.c_str(), flags); _fd == -1) {
        throw std::runtime_error("Failed to open file descriptor");
    }
}

file_descriptor::file_descriptor(file_descriptor &&other) noexcept
    : _fd(other._fd) {
    other._fd = -1;
}

file_descriptor &file_descriptor::operator=(file_descriptor &&other) noexcept {
    if (this != &other) {
        close();
        _fd = other._fd;
        other._fd = -1;
    }
    return *this;
}

file_descriptor::~file_descriptor() { close(); }

file_descriptor::operator int() const { return _fd; }

void file_descriptor::close() const {
    if (_fd != -1) {
        ::close(_fd);
    }
}

evdev_device::evdev_device(const std::string &path, int flags)
    : _fd(path, flags) {
    if (libevdev_new_from_fd(_fd, &_dev) != 0) {
        throw std::runtime_error("Failed to create libevdev device");
    }
}

evdev_device::evdev_device(evdev_device &&other) noexcept
    : _fd(std::move(other._fd)), _dev(other._dev) {
    other._dev = nullptr;
}

evdev_device &evdev_device::operator=(evdev_device &&other) noexcept {
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

evdev_device::~evdev_device() {
    if (_dev != nullptr) {
        libevdev_free(_dev);
    }
}

libevdev *evdev_device::device() { return _dev; }

[[nodiscard]] std::string evdev_device::name() const { return libevdev_get_name(_dev); }

[[nodiscard]] std::string evdev_device::phys() const { return libevdev_get_phys(_dev); }

void evdev_device::process_events(const std::function<void(const input_event &)> &callback) const {
    auto is_error = [](int status) { return status < 0 && status != -EAGAIN; };
    auto has_next_event = [](int status) { return status >= 0; };
    while (true) {
        auto [status, event] = next_event();
        if (is_error(status)) {
            throw std::runtime_error("Failed to read event");
        }
        if (!has_next_event(status)) {
            continue;
        }
        callback(event);
    }
}

evdev_device evdev_device::create(const std::string &name) {
    evdev_device dev;
    dev._dev = libevdev_new();
    libevdev_set_name(dev._dev, name.c_str());
    // libevdev_enable_property(dev._dev, INPUT_PROP_POINTER);
    return dev;
}

evdev_device evdev_device::find_by_name(const std::string &name) {
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
    throw std::runtime_error("Failed to find device by name");
}

[[nodiscard]] event_ret evdev_device::next_event() const {
    input_event ev = {};
    const auto flags = LIBEVDEV_READ_FLAG_NORMAL | LIBEVDEV_READ_FLAG_BLOCKING;
    auto status = libevdev_next_event(_dev, flags, &ev);
    return { status, ev };
}
