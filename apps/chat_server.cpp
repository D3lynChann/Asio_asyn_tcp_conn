#if (defined(_WIN32) || defined(_WIN64))
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE

#include "../3rd_party/asio/include/asio.hpp"
#include "../source/chat_message.cpp"
 
class async_server {
 
public:
	async_server(io_service &ioservice, const tcp::endpoint endpoint):
		io_service_(ioservice), acceptor_(ioservice, endpoint) {
		start_accept();                                                        
	}
		
	void start_accept() {
		std::cout << "ready for connection" << std::endl;
		
		// new a client_session
		std::shared_ptr<client_session>new_session(new client_session(io_service_));
		// hook the callback function to the system
		// after finish receiving, the system will call the function
		// here the callback function is accept_handle()
		acceptor_.async_accept(
			new_session->socket(), 
			std::bind(
				&async_server::accept_handle, this, 
				new_session, std::placeholders::_1
			)
		);	
	}
	
	// this function is called by the system
	void accept_handle(std::shared_ptr<client_session> client, const asio::error_code&error) {
		if (!error) {
			std::cout << "connect addr:";
			std::cout << client->socket().remote_endpoint().address() << std::endl;
			std::cout << "connect port:";
			std::cout << client->socket().remote_endpoint().port() << std::endl;
			
			client->start();
		}
		start_accept();
	}
	
private:
	io_service& io_service_;   
	tcp::acceptor acceptor_;                             
};

int main() {
	io_service service;                              
	ip::tcp::endpoint endpoint(tcp::v4(), 8888);     
	std::shared_ptr<async_server>service_(new async_server(service, endpoint));
	service.run();
	getchar();
	return 0;
}