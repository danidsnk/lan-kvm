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
        running_ = false;
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    template <typename Q>
    void start(Q &queue) {
        thread_ = std::thread([this, &queue] {
            device_.process_events([this, &queue](const input_event &event) {
                if (running_ && (event.type == EV_KEY || event.type == EV_REL)) {
                    queue.push({ event.type, event.code, event.value });
                }
                return running_;
            });
        });
    }

private:
    evdev_device device_;
    std::thread thread_;
    bool running_ = true;
};

} // namespace device

#endif
