#ifndef EVDEV_H
#define EVDEV_H

#include "filedescriptor.h"
#include <functional>
#include <libevdev/libevdev.h>

namespace device {

class evdev_device {
public:
    struct evdev_error : std::runtime_error {
        explicit evdev_error(const std::string &message)
            : std::runtime_error(message) {}
    };

    evdev_device(const evdev_device &) = delete;
    evdev_device &operator=(const evdev_device &) = delete;
    evdev_device(evdev_device &&other) noexcept;
    evdev_device &operator=(evdev_device &&other) noexcept;
    ~evdev_device();

    void process_events(const std::function<bool(const input_event &)> &callback);

    static evdev_device create(const std::string &name);
    static evdev_device find_by_name(const std::string &name);

private:
    evdev_device() = default;
    evdev_device(const std::string &path, int flags);

    input_event next_event();

    libevdev *dev_ = nullptr;
    utils::file_descriptor fd_;
};

} // namespace device

#endif
