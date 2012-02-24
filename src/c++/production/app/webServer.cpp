#include "web-listener.hpp"
#include "websocketpp.hpp"
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>

using boost::asio::ip::tcp;
using namespace websocketweb;

int main(int argc, char* argv[]) 
{
	std::string host;
	int port;
	std::string data_host;
	int data_port;
	std::string full_host;
		host = argv[1];
		port = atoi(argv[2]);
		data_host = argv[3];
		data_port = atoi(argv[4]);
	if (argc < 4) 
	{
		std::cout<<"\n\n Missing Arguments ./webserver <websocket-ip> <websocket-port> <data-gen-ip> <data-gen-port>\n\n";
	}
	//std::cout<<"\n\n Data Generator : "<<data_gen_ip<<":"<<data_gen_port<<"\n\n";
	std::stringstream temp;
	temp << host << ":" << port;	
	full_host = temp.str();
	web_server_handler_ptr web_handler(new web_server_handler(data_host,data_port));
	//web_server_handler_ptr web_handler(new web_server_handler());
	try 
	{
		boost::asio::io_service io_service;
		tcp::endpoint endpoint(tcp::v6(), port);
		websocketpp::server_ptr server(new websocketpp::server(io_service,endpoint,web_handler));
		server->add_host(host);
		server->add_host(full_host);
		server->set_max_message_size(0xFFFF); // 64KiB
		server->start_accept();
		std::cout << "Starting web server on " << full_host << std::endl;
		io_service.run();
	} 
	catch (std::exception& e) 
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	std::cout<< "Stopping web server";	
	return 0;
}
