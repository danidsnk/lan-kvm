#ifndef LISTENER_H
#define LISTENER_H

#include "deviceevent.h"
#include <boost/asio.hpp>
#include <functional>
#include <string>

namespace network {

class listener {
public:
    listener(const std::string &host, int port)
        : endpoint_(boost::asio::ip::make_address(host), port) {}

    void start(const std::function<bool(device_event)> &handler);

private:
    void try_connect();
    void disconnect();
    std::tuple<bool, device_event> read_socket();

    boost::asio::ip::tcp::endpoint endpoint_;
    boost::asio::io_context context_;
    boost::asio::ip::tcp::socket socket_{ context_ };
};

} // namespace network

#endif
