file(GLOB SRC_FILES *.cpp)
add_executable(client ${SRC_FILES})

find_package(Boost COMPONENTS program_options REQUIRED)
target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/device/macos
                                          ${Boost_INCLUDE_DIRS})
target_link_libraries(client PRIVATE device
                                     ${Boost_LIBRARIES})
