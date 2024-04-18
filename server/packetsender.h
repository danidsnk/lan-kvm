#include "sharedqueue.h"
#include <boost/asio.hpp>
#include <iostream>

template <typename T>
class packet_sender {
public:
    using tcp = boost::asio::ip::tcp;

    packet_sender(tcp::socket &socket, shared_queue<T> &queue, std::chrono::milliseconds interval)
        : socket_(std::move(socket)), queue_(queue), interval_(interval),
          timer_(socket_.get_executor()) {}

    packet_sender(const packet_sender &) = delete;
    packet_sender &operator=(const packet_sender &) = delete;
    packet_sender(packet_sender &&) = delete;
    packet_sender &operator=(packet_sender &&) = delete;
    ~packet_sender() = default;

    void start() { set_timer(); }

private:
    void send_packet() {
        typename shared_queue<T>::queue_type buf(interval_.count() * 2);
        queue_.wait_swap(buf);

        std::cout << buf.size() << "  Packets sent\n";
        boost::asio::async_write(
            socket_, boost::asio::buffer(buf),
            [this](boost::system::error_code ec, std::size_t) {
                if (!ec) {
                    set_timer();
                } else {
                    std::cerr << "Error on send: " << ec.message() << std::endl;
                }
            });
    }

    void set_timer() {
        timer_.expires_after(interval_);
        timer_.async_wait([this](const boost::system::error_code &) { send_packet(); });
    }

    tcp::socket socket_;
    shared_queue<T> &queue_;
    std::chrono::milliseconds interval_;
    boost::asio::steady_timer timer_;
};
