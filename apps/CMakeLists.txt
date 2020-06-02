add_executable(Client chat_client.cpp)
add_executable(Server chat_server.cpp)

target_link_libraries(Client libAsio)
target_link_libraries(Server libAsio)

target_compile_options(Client PRIVATE -std=c++11)
target_compile_options(Server PRIVATE -std=c++11)