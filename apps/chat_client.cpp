#if (defined(_WIN32) || defined(_WIN64))
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE

#include "../3rd_party/asio/include/asio.hpp"
#include <iostream>
#include <functional>

#if (defined(__linux__))
#include <pthread.h>
#elif (defined(_WIN32) || defined(_WIN64))
#include <thread>
#endif

using namespace asio;
using asio::ip::tcp;
 
class async_client {
	
public:
	async_client(io_service& service, tcp::endpoint endpoint):
		ioservice_(service), socket_(service) {
			socket_.async_connect(
				endpoint,
				std::bind(
					&async_client::connet_handle, 
					this, std::placeholders::_1
				)
		);
	}
 
	void connet_handle(const error_code& error) {
		if (!error) {
			std::cout << "connect!\n";
			socket_.async_read_some(
				buffer(read_buffer_),
				std::bind(
					&async_client::read_handle, 
					this, std::placeholders::_1
				)
			);
		}
	}
 
	void read_handle(const error_code& error) {
		if (!error) {
			std::cout << "item: " << read_buffer_ << std::endl;
			memset(read_buffer_, 0, sizeof(read_buffer_));
			socket_.async_read_some(
				buffer(read_buffer_),
				std::bind(
					&async_client::read_handle, 
					this, std::placeholders::_1
				)
			);
		}
	}
 
	void send(const char* msg, size_t len) {
		memcpy(write_buffer_, msg, len);
		asio::async_write(
			socket_, 
			buffer(write_buffer_, len), 
			bind(
				&async_client::write_handle, 
				this, std::placeholders::_1
			)
		);
	}
 
	// do nothing
	void write_handle(const error_code& error) {}
 
private:
	io_service&  ioservice_;
	tcp::socket socket_;
	char read_buffer_[512];
	char write_buffer_[513];
	
};
 
 
int main(int argc, char** argv) {
	io_service service;
	tcp::endpoint endpoint(ip::address::from_string("192.168.5.58"), 8888);
 
	async_client client(service, endpoint);
 
	// noticed that std::bind cannot determine which asio::io_service::*run to overload
	// so we have to use temporary variables for type conversion
	std::size_t (asio::io_service::*run)() = &asio::io_service::run;
    std::thread t(std::bind(run, &service));
 
	char line[512 + 1];
	while (std::cin.getline(line, sizeof(line))) {
		if (strcmp(line, "exit") == 0) {
			break;
		}
		client.send(line, strlen(line));
	}
 
	t.join();
	getchar();
	return 0;
}