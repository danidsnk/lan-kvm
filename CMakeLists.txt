cmake_minimum_required(VERSION 3.26)
project(lan-kvm)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost REQUIRED)

add_subdirectory(device)
add_subdirectory(client)
if (LINUX)
    add_subdirectory(server)
endif()
