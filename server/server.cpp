// test server
#include "evdev.h"
#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

struct event {
    int type;
    int code;
    int value;
};

int main() {
    boost::asio::io_context io_context;
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 1448));
    tcp::socket socket(io_context);
    acceptor.accept(socket);
    std::cout << "Connected" << std::endl;

    try {
        auto dev = evdev_device::find_by_name("foostan Corne");
        dev.process_events([&socket](const input_event &event) {
            struct event buf = { event.type, event.code, event.value };
            boost::asio::write(socket, boost::asio::buffer(&buf, sizeof(buf)));
        });
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
