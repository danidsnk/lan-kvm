#ifndef DEVICECAPTURE_H
#define DEVICECAPTURE_H

#include "evdev.h"
#include <thread>

namespace device {

class device_capture {
public:
    explicit device_capture(const std::string &device_name)
        : device_(evdev_device::find_by_name(device_name)) {}

    device_capture(const device_capture &) = delete;
    device_capture &operator=(const device_capture &) = delete;
    device_capture(device_capture &&) = delete;
    device_capture &operator=(device_capture &&) = delete;

    ~device_capture() {
        thread_.request_stop();
        thread_.join();
    }

    template <typename Q>
    void start(Q &queue) {
        thread_ = std::jthread([this, &queue](std::stop_token stoken) {
            device_.process_events(
                [this, &queue, stoken](const input_event &event) {
                    bool running = !stoken.stop_requested();
                    bool event_valid = event.type == EV_KEY || event.type == EV_REL;

                    if (running && event_valid) {
                        queue.push({ event.type, event.code, event.value });
                    }
                    return running;
                });
        });
    }

private:
    evdev_device device_;
    std::jthread thread_;
};

} // namespace device

#endif
