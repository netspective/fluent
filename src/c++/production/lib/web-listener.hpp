#ifndef WEB_HPP
#define WEB_HPP

#include <boost/shared_ptr.hpp>
#include "/var/www/zaphoyd-websocketpp-e5b87e6/src/websocketpp.hpp"
#include "/var/www/zaphoyd-websocketpp-e5b87e6/src/websocket_connection_handler.hpp"

#include <map>
#include <string>
#include <vector>


namespace websocketweb {

class web_server_handler : public websocketpp::connection_handler {
public:
	pthread_attr_t attr;
        pthread_t id;
	//std::string name_topics[100];
	std::string name_topics;
	//int topic_count;
	websocketpp::session_ptr temp_client;
	std::string temp_msg;
	pthread_t pt_ary[100];
	std::string currentdevice[100];
	int pthread_count;
	web_server_handler() {}
	virtual ~web_server_handler() {}
	
	void validate(websocketpp::session_ptr client); 
	
	


	// add new connection to the lobby
	void on_open(websocketpp::session_ptr client);
		
	// someone disconnected from the lobby, remove them
	void on_close(websocketpp::session_ptr client);
	
	void on_message(websocketpp::session_ptr client,const std::string &msg);
	
	// lobby will ignore binary messages
	void on_message(websocketpp::session_ptr client,
		const std::vector<unsigned char> &data) {}
private:
	std::string serialize_state();
	std::string encode_message(std::string sender,std::string msg,bool escape = true);
	std::string get_con_id(websocketpp::session_ptr s);
	
	void send_to_all(std::string data);
	static void *startTopicsThread(void *arg);
	static void *startThread(void *arg);
	// list of outstanding connections
	std::map<websocketpp::session_ptr,std::string> m_connections;
};

typedef boost::shared_ptr<web_server_handler> web_server_handler_ptr;

}
#endif // WEB_HPP
