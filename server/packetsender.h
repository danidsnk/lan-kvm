#ifndef PACKETSENDER_H
#define PACKETSENDER_H

#include "deviceevent.h"
#include "sharedqueue.h"
#include <boost/asio.hpp>
#include <iostream>
#include <memory>

namespace network {

constexpr int TICK_RATE = 144;
constexpr int SECOND = 1000;
constexpr int SERVER_TIMER = SECOND / TICK_RATE;
// 2 events per milisecond
constexpr int BUFFER_SIZE = SERVER_TIMER * 2;

class packet_sender {
    using tcp = boost::asio::ip::tcp;

public:
    explicit packet_sender(int port) : port_(port) {}

    auto queue() { return queue_; }

    void start() {
        tcp::acceptor acceptor(context_, tcp::endpoint(tcp::v4(), port_));
        acceptor.accept(socket_);

        next_tick();

        context_.run();
    }

private:
    void send_packet() {
        auto buf = queue_->pop_all();
        socket_.async_send(
            boost::asio::buffer(buf),
            [this](boost::system::error_code ec, auto) {
                if (!ec) {
                    next_tick();
                } else {
                    std::cerr << "Error on send: " << ec.message() << "\n";
                }
            });
    }

    void next_tick() {
        timer_.expires_after(interval_);
        timer_.async_wait([this](auto) { send_packet(); });
    }

    using queue_t = shared_queue<device_event>;

    int port_;
    std::shared_ptr<queue_t> queue_{ new queue_t{ BUFFER_SIZE } };
    boost::asio::io_context context_;
    tcp::socket socket_{ context_ };
    std::chrono::milliseconds interval_{ SERVER_TIMER };
    boost::asio::steady_timer timer_{ context_ };
};

} // namespace network

#endif
