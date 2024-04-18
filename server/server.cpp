#include "devicecapture.h"
#include "packetsender.h"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main() {
    try {
        while (true) {
            boost::asio::io_context context;
            tcp::acceptor acceptor(context, tcp::endpoint(tcp::v4(), 1448));
            tcp::socket socket(context);
            acceptor.accept(socket);
            std::cout << "Connected\n";

            using dev_event = device_capture<shared_queue>::device_event;
            {
                shared_queue<dev_event> queue;

                packet_sender<dev_event> sender(
                    socket, queue, std::chrono::milliseconds(1000 / 144));
                device_capture<shared_queue> mouse_capture("Logitech G305", queue);
                device_capture<shared_queue> keyboard_capture("foostan Corne", queue);

                mouse_capture.start();
                keyboard_capture.start();
                sender.start();

                context.run();
            }
            std::cout << "Disconnected\n";
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
