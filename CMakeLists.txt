cmake_minimum_required(VERSION 3.29)
project(lan-kvm)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(LIBEVDEV REQUIRED libevdev)

add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(device)
