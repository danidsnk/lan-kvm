file(GLOB SRC_FILES *.cpp)
add_executable(server ${SRC_FILES})

target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/device)
target_link_libraries(server PRIVATE device)
