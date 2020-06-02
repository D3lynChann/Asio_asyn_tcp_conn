#if (defined(_WIN32) || defined(_WIN64))
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE

#include "../3rd_party/asio/include/asio.hpp"
#include <iostream>
#include <functional>

using namespace asio;
using asio::ip::tcp;

// since socket cannot be copied, we use enable_shared_from_this here for a continuous connection
class client_session : public std::enable_shared_from_this<client_session> { 

public:
    // consrtuct a sokcet with io_service
	// just initial the socket
	client_session(io_service &ioservice)              
		: socket_(ioservice) { }
		
	// for gaining private socket_
	// noticed that return a conference of socket_
	tcp::socket& socket() {
		return socket_;
	}
	
	// handle a connection from a client
	void start() {
		async_write(
			socket_, asio::buffer("welcome!\n"),
			std::bind(
				&client_session::write_handle, 
				shared_from_this(), std::placeholders::_1
			)
		);
		
		// clear the buffer
		memset(read_buf_, 0, sizeof(read_buf_)); 
		
		// read asynchronously from buffer
		// noticed that we bind read_handle here
		socket_.async_read_some(
			asio::buffer(read_buf_, 512),        
			std::bind(
				&client_session::read_handle, shared_from_this(), 
				std::placeholders::_1, std::placeholders::_2
			)
		);
	}
	
	// function for reading from client
	void read_handle(const asio::error_code& error, size_t bytes_transferred) {
		// firstly check the error_code
		if (!error) {
			// do some thing here
			std::cout << "item:" << read_buf_ << std::endl;
			std::cout << "size:" << bytes_transferred << std::endl;
			std::cout << std::endl;
			read_buf_[0] = '!';
			// callback function
			async_write(
				socket_, 
				asio::buffer(read_buf_, bytes_transferred),
				std::bind(
					&client_session::write_handle, 
					shared_from_this(), std::placeholders::_1
				)
			);                                                                         
		}
	}
	
	// function for writing to client
	void write_handle(const asio::error_code& error) {
		if (!error) {
			// clear the buffer first
			memset(read_buf_, 0, sizeof(read_buf_));
			
			// it looks like start()
			socket_.async_read_some(
				asio::buffer(read_buf_, 512),
				std::bind(
					&client_session::read_handle, shared_from_this(),
					std::placeholders::_1, std::placeholders::_2
				)
			);
		}
	}
	
private:
	// a main buffer for reading
	char read_buf_[512];
	// a main socket for connection
	tcp::socket socket_; 
	
};