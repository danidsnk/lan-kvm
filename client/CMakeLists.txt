file(GLOB SRC_FILES *.cpp)
add_executable(client ${SRC_FILES})

target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/device)
target_link_libraries(client PRIVATE device)
