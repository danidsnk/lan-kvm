#include "listener.h"
#include <iostream>

using boost::asio::ip::tcp;

namespace network {

constexpr int RECONNECT_DELAY = 10;

void listener::start(const std::function<bool(device_event)> &handler) {
    while (true) {
        try_connect();
        while (true) {
            auto [success, buffer] = read_socket();
            if (!success) {
                break;
            }
            if (!handler(buffer)) {
                break;
            }
        }
        disconnect();
        std::this_thread::sleep_for(std::chrono::seconds(RECONNECT_DELAY));
    }
}

void listener::try_connect() {
    std::cout << "Connecting...\n";
    while (true) {
        boost::system::error_code error;
        socket_.connect(endpoint_, error);
        if (!error) {
            break;
        }
        socket_.close();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "Connected\n";
}

void listener::disconnect() {
    boost::system::error_code error;
    socket_.shutdown(tcp::socket::shutdown_both, error);
    if (error) {
        std::cerr << "Error: " << error.message() << '\n';
    }
    socket_.close();
    std::cout << "Disconnected\n";
}

std::tuple<bool, device_event> listener::read_socket() {
    device_event buffer{};
    boost::system::error_code error;
    socket_.read_some(boost::asio::buffer(&buffer, sizeof(buffer)), error);
    if (error == boost::asio::error::eof) {
        return { false, buffer };
    }
    if (error) {
        throw boost::system::system_error(error);
    }
    return { true, buffer };
}

} // namespace network
