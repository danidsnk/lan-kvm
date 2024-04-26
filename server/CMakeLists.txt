file(GLOB SRC_FILES *.cpp)
add_executable(server ${SRC_FILES})


find_package(Boost COMPONENTS program_options REQUIRED)
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/device/linux
                                          ${Boost_INCLUDE_DIRS})
target_link_libraries(server PRIVATE device
                                     ${Boost_LIBRARIES})
