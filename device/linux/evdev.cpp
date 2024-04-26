#include "evdev.h"
#include <fcntl.h>
#include <filesystem>
#include <format>
#include <ranges>

namespace device {

evdev_device::evdev_device(const std::string &path, int flags)
    : fd_(path, flags) {
    if (libevdev_new_from_fd(fd_, &dev_) != 0) {
        throw evdev_error("Failed to initialize libevdev device");
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

void evdev_device::process_events(const std::function<bool(const input_event &)> &callback) {
    input_event event{};
    do {
        event = next_event();
    } while (callback(event));
}

evdev_device evdev_device::create(const std::string &name) {
    evdev_device dev;
    dev.dev_ = libevdev_new();
    libevdev_set_name(dev.dev_, name.c_str());
    // libevdev_enable_property(dev._dev, INPUT_PROP_POINTER);
    return dev;
}

evdev_device evdev_device::find_by_name(const std::string &name) {
    using dir = std::filesystem::directory_iterator;
    using std::ranges::views::filter;

    auto is_event = [](auto &&entry) {
        return entry.path().string().contains("event");
    };

    for (auto &&entry : dir("/dev/input") | filter(is_event)) {
        evdev_device dev(entry.path(), O_RDONLY);
        if (name == libevdev_get_name(dev.dev_)) {
            return dev;
        }
    }

    throw evdev_error(std::format("Failed to find device: {}", name));
}

input_event evdev_device::next_event() {
    input_event ev = {};
    constexpr auto flags = LIBEVDEV_READ_FLAG_NORMAL;
    auto status = libevdev_next_event(dev_, flags, &ev);

    if (status < 0 && status != -EAGAIN) {
        throw evdev_error("Failed to read event");
    }
    return ev;
}

} // namespace device
