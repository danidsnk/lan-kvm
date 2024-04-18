#include "deviceevent.h"
#include "evdev.h"
#include <thread>

template <template <typename> typename Q>
class device_capture {
public:
    using device_event = device_event_;

    explicit device_capture(const std::string &device_name,
                            Q<device_event> &queue)
        : device_(evdev_device::find_by_name(device_name)),
          queue_(queue) {}

    device_capture(const device_capture &) = delete;
    device_capture &operator=(const device_capture &) = delete;
    device_capture(device_capture &&) = delete;
    device_capture &operator=(device_capture &&) = delete;

    ~device_capture() {
        running_ = false;
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    void start() {
        thread_ = std::thread([this] {
            device_.process_events([this](const input_event &event) {
                if (running_ && (event.type == EV_KEY || event.type == EV_REL)) {
                    device_event ev = { event.type, event.code, event.value };
                    queue_.push(ev);
                }
                return running_;
            });
        });
    }

private:
    evdev_device device_;
    Q<device_event> &queue_;
    std::thread thread_;
    bool running_ = true;
};
