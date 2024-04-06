project(kvm)

find_package(PkgConfig REQUIRED)
pkg_search_module(LIBEVDEV REQUIRED libevdev)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(kvm main.cpp)
target_include_directories(kvm PRIVATE ${LIBEVDEV_INCLUDE_DIRS})
target_link_libraries(kvm PRIVATE ${LIBEVDEV_LIBRARIES})
