#include "devicecapture.h"
#include "packetsender.h"
#include <boost/asio.hpp>
#include <boost/program_options.hpp>

std::tuple<std::string, std::string, int> parse_args(int argc, char **argv) {
    namespace po = boost::program_options;
    po::options_description desc("Options");
    desc.add_options()
        ("mouse,m", po::value<std::string>(), "Mouse name")
        ("keyboard,k", po::value<std::string>(), "Keyboard name")
        ("port,p", po::value<int>(), "Port");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (!vm.contains("mouse") ||
        !vm.contains("keyboard") ||
        !vm.contains("port")) {

        throw std::runtime_error("Mouse, keyboard, and port are required");
    }
    return { vm["mouse"].as<std::string>(),
             vm["keyboard"].as<std::string>(),
             vm["port"].as<int>() };
}

int main(int argc, char **argv) {
    try {
        auto [mouse, keyboard, port] = parse_args(argc, argv);

        network::packet_sender sender{ port };
        device::device_capture mouse_capture{ mouse };
        device::device_capture keyboard_capture{ keyboard };

        mouse_capture.start(sender.queue());
        keyboard_capture.start(sender.queue());
        sender.start();

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
