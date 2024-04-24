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
    if (fd_ = open(path.c_str(), flags); fd_ == -1) {
        throw std::runtime_error("Failed to open file descriptor");
    }
}

file_descriptor::file_descriptor(file_descriptor &&other) noexcept
    : fd_(other.fd_) {
    other.fd_ = -1;
}

file_descriptor &file_descriptor::operator=(file_descriptor &&other) noexcept {
    if (this != &other) {
        close();
        fd_ = other.fd_;
        other.fd_ = -1;
    }
    return *this;
}

file_descriptor::~file_descriptor() { close(); }

file_descriptor::operator int() const { return fd_; }

void file_descriptor::close() const {
    if (fd_ != -1) {
        ::close(fd_);
    }
}

evdev_device::evdev_device(const std::string &path, int flags)
    : fd_(path, flags) {
    if (libevdev_new_from_fd(fd_, &dev_) != 0) {
        throw std::runtime_error("Failed to create libevdev device");
    }
    libevdev_grab(dev_, LIBEVDEV_GRAB);
}

evdev_device::evdev_device(evdev_device &&other) noexcept
    : fd_(std::move(other.fd_)), dev_(other.dev_) {
    other.dev_ = nullptr;
}

evdev_device &evdev_device::operator=(evdev_device &&other) noexcept {
    if (this != &other) {
        this->~evdev_device();
        fd_ = std::move(other.fd_);
        dev_ = other.dev_;
        other.dev_ = nullptr;
    }
    return *this;
}

evdev_device::~evdev_device() {
    if (dev_ != nullptr) {
        libevdev_grab(dev_, LIBEVDEV_UNGRAB);
        libevdev_free(dev_);
    }
}

void evdev_device::process_events(const std::function<bool(const input_event &)> &callback) const {
    auto is_error = [](int status) { return status < 0 && status != -EAGAIN; };
    auto has_next_event = [](int status) { return status >= 0; };

    bool running = true;
    while (running) {
        auto [status, event] = next_event();
        if (is_error(status)) {
            throw std::runtime_error("Failed to read event");
        }
        // if (!has_next_event(status)) {
        //     continue;
        // }
        running = callback(event);
    }
}

evdev_device evdev_device::create(const std::string &name) {
    evdev_device dev;
    dev.dev_ = libevdev_new();
    libevdev_set_name(dev.dev_, name.c_str());
    // libevdev_enable_property(dev._dev, INPUT_PROP_POINTER);
    return dev;
}

evdev_device evdev_device::find_by_name(const std::string &name) {
    for (int i = 0;; ++i) {
        try {
            evdev_device dev("/dev/input/event" + std::to_string(i), O_RDONLY);
            if (name == libevdev_get_name(dev.dev_)) {
                return dev;
            }
        } catch (const std::runtime_error &) {
            break;
        }
    }
    throw std::runtime_error("Failed to find device by name");
}

[[nodiscard]] std::tuple<int, input_event> evdev_device::next_event() const {
    input_event ev = {};
    const auto flags = LIBEVDEV_READ_FLAG_NORMAL;
    auto status = libevdev_next_event(dev_, flags, &ev);
    return { status, ev };
}
