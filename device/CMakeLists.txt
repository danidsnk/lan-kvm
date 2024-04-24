if (APPLE)
    set(PLATFORM_SRC macos)
else()
    find_package(PkgConfig REQUIRED)
    pkg_search_module(LIBEVDEV REQUIRED libevdev)
    set(PLATFORM_SRC linux)
endif()

file(GLOB SRC_FILES ${PLATFORM_SRC}/*.cpp)
add_library(device STATIC ${SRC_FILES})

if (APPLE)
    target_link_libraries(device
        "-framework ApplicationServices"
        "-framework Carbon")
else()
    target_include_directories(device PUBLIC ${LIBEVDEV_INCLUDE_DIRS})
    target_link_libraries(device PRIVATE ${LIBEVDEV_LIBRARIES})
endif()
