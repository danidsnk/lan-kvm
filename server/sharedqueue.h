#include <mutex>
#include <condition_variable>
#include <vector>

template <typename T>
class shared_queue {
public:
    using value_type = T;
    using queue_type = std::vector<T>;

    shared_queue() = default;
    shared_queue(const shared_queue &) = delete;
    shared_queue &operator=(const shared_queue &) = delete;
    shared_queue(shared_queue &&) = delete;
    shared_queue &operator=(shared_queue &&) = delete;
    ~shared_queue() = default;
    void push(const T &value) {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push_back(value);
        cond_.notify_one();
    }
    void wait_swap(std::vector<T> &queue) noexcept {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] { return !queue_.empty(); });
        queue = std::move(queue_);
    }

private:
    std::vector<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cond_;
};
