file(GLOB SRC_FILES *.cpp)
add_library(device STATIC ${SRC_FILES})

target_include_directories(device PUBLIC ${LIBEVDEV_INCLUDE_DIRS})
target_link_libraries(device PRIVATE ${LIBEVDEV_LIBRARIES})
