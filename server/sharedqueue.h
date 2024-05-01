#ifndef SHAREDQUEUE_H
#define SHAREDQUEUE_H

#include <boost/circular_buffer.hpp>
#include <condition_variable>
#include <mutex>
#include <vector>

namespace network {

template <typename T>
class shared_queue {
public:
    using value_type = T;
    using container_type = boost::circular_buffer<T>;

    explicit shared_queue(container_type::capacity_type capacity)
        : buffer_(capacity) {}

    void push(const T &value) {
        std::unique_lock<std::mutex> lock(mutex_);
        buffer_.push_back(value);
        cond_.notify_one();
    }
    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] { return !buffer_.empty(); });

        auto value = buffer_.front();
        buffer_.pop_front();
        return value;
    }
    std::vector<T> pop_all() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] { return !buffer_.empty(); });

        std::vector<T> values{ buffer_.begin(), buffer_.end() };
        buffer_.clear();
        return values;
    }

private:
    container_type buffer_;
    mutable std::mutex mutex_;
    std::condition_variable cond_;
};

} // namespace network

#endif
