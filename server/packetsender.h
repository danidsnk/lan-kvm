#ifndef PACKETSENDER_H
#define PACKETSENDER_H

#include "deviceevent.h"
#include "sharedqueue.h"
#include <boost/asio.hpp>
#include <iostream>

namespace network {

constexpr int TICK_RATE = 144;
constexpr int SECOND = 1000;
constexpr int SERVER_TIMER = SECOND / TICK_RATE;

class packet_sender {
    using tcp = boost::asio::ip::tcp;

public:
    explicit packet_sender(int port) : port_(port) {}

    auto &queue() { return queue_; }

    void start() {
        tcp::acceptor acceptor(context_, tcp::endpoint(tcp::v4(), port_));
        acceptor.accept(socket_);

        next_tick();

        context_.run();
    }

private:
    void send_packet() {
        // 2 events per milisecond
        shared_queue<device_event>::container_type buf;
        buf.reserve(SERVER_TIMER * 2);
        queue_.swap_buffer(buf);

        boost::asio::async_write(
            socket_, boost::asio::buffer(buf),
            [this](boost::system::error_code ec, std::size_t) {
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

    int port_;
    shared_queue<device_event> queue_;
    boost::asio::io_context context_;
    tcp::socket socket_{ context_ };
    std::chrono::milliseconds interval_{ SERVER_TIMER };
    boost::asio::steady_timer timer_{ context_ };
};

} // namespace network

#endif
