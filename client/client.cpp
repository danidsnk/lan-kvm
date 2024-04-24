#include "listener.h"
#include "virtualkeyboard.h"
#include "virtualmouse.h"
#include <boost/program_options.hpp>
#include <iostream>

namespace {

constexpr int STOP_CODE = 96;
constexpr double MOUSE_SPEED = .8;

} // namespace

std::tuple<std::string, int> parse_args(int argc, char **argv) {
    namespace po = boost::program_options;
    po::options_description desc("Options");
    desc.add_options()
        ("host,h", po::value<std::string>(), "Host")
        ("port,p", po::value<int>(), "Port");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (!vm.count("host") || !vm.count("port")) {
        throw std::runtime_error("Host and port are required");
    }
    return { vm["host"].as<std::string>(), vm["port"].as<int>() };
}

bool event_handler(device_event ev) {
    static device::emulation::virtual_mouse mouse{ MOUSE_SPEED };
    static device::emulation::virtual_keyboard keyboard;

    if (ev.code == STOP_CODE) {
        return false;
    }
    std::cout << "type: " << ev.type
              << " code: " << ev.code
              << " value: " << ev.value << '\n';
    if (mouse.is_valid(ev)) {
        mouse.process_event(ev);
    } else {
        keyboard.process_event(ev);
    }
    return true;
}

int main(int argc, char **argv) {
    try {
        auto [host, port] = parse_args(argc, argv);
        network::listener listener{ host, port };
        listener.start(event_handler);

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
