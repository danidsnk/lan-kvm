#ifndef SHAREDQUEUE_H
#define SHAREDQUEUE_H

#include <condition_variable>
#include <mutex>
#include <vector>

namespace network {

template <typename T>
class shared_queue {
public:
    using value_type = T;
    using container_type = std::vector<T>;

    void push(const T &value) {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push_back(value);
        cond_.notify_one();
    }
    void swap_buffer(std::vector<T> &queue) {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] { return !queue_.empty(); });
        queue_.swap(queue);
    }

private:
    container_type queue_;
    mutable std::mutex mutex_;
    std::condition_variable cond_;
};

} // namespace network

#endif
