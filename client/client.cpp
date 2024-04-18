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
        while (true) {
            boost::asio::io_context context;
            tcp::resolver resolver{ context };
            tcp::socket socket{ context };

            std::cout << "Connecting...\n";
            while (true) {
                tcp::endpoint endpoint{ boost::asio::ip::make_address("127.0.0.1"), 1448 };
                boost::system::error_code error;
                socket.connect(endpoint, error);
                if (!error) {
                    break;
                }
                socket.close();
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            std::cout << "Connected\n";

            while (true) {
                event buf = {};
                boost::system::error_code error;
                socket.read_some(boost::asio::buffer(&buf, sizeof(buf)), error);
                if (error == boost::asio::error::eof) {
                    break;
                }
                if (error) {
                    throw boost::system::system_error(error);
                }
                if (buf.code == 96) {
                    socket.close();
                    break;
                }
                std::cout << "type: " << buf.type
                          << " code: " << buf.code
                          << " value: " << buf.value << '\n';
            }
            std::cout << "Disconnected\n";
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
