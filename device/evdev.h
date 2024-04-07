#include <functional>
#include <libevdev/libevdev-uinput.h>
#include <libevdev/libevdev.h>
#include <string>

class file_descriptor {
public:
    file_descriptor() = default;
    file_descriptor(const std::string &path, int flags);
    file_descriptor(const file_descriptor &) = delete;
    file_descriptor &operator=(const file_descriptor &) = delete;
    file_descriptor(file_descriptor &&other) noexcept;
    file_descriptor &operator=(file_descriptor &&other) noexcept;
    ~file_descriptor();
    operator int() const;

private:
    void close() const;
    int _fd = -1;
};

struct event_ret {
    int status;
    input_event event;
};

class evdev_device {
public:
    evdev_device() = default;
    explicit evdev_device(const std::string &path, int flags);
    evdev_device(const evdev_device &) = delete;
    evdev_device &operator=(const evdev_device &) = delete;
    evdev_device(evdev_device &&other) noexcept;
    evdev_device &operator=(evdev_device &&other) noexcept;
    ~evdev_device();
    libevdev *device();
    [[nodiscard]] std::string name() const;
    [[nodiscard]] std::string phys() const;

    void process_events(const std::function<void(const input_event &)> &callback) const;

    static evdev_device create(const std::string &name);
    static evdev_device find_by_name(const std::string &name);

private:
    [[nodiscard]] event_ret next_event() const;

    libevdev *_dev = nullptr;
    file_descriptor _fd;
};
