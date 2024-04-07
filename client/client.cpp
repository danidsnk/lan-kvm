#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

struct event {
    int type;
    int code;
    int value;
};

int main() {
    try {
        boost::asio::io_context io_context;
        tcp::resolver resolver{ io_context };
        tcp::resolver::results_type endpoints = resolver.resolve("localhost", "1448");
        tcp::socket socket{ io_context };
        boost::asio::connect(socket, endpoints);

        while (true) {
            boost::system::error_code error;
            event buf{};
            socket.read_some(boost::asio::buffer(&buf, sizeof(buf)), error);

            if (error) {
                if (error == boost::asio::error::eof) {
                    break;
                }
                throw boost::system::system_error(error);
            }

            std::cout << "type: " << buf.type
                      << " code: " << buf.code
                      << " value: " << buf.value << '\n';
        }

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
