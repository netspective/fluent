#include <iostream>
#include <dds/dds.hpp>
#include "ccpp_tempmonitor.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <boost/program_options.hpp>
#define MAX_LINE 100
#define LINE_ARRAY_SIZE (MAX_LINE+1)
using namespace std;
int socketDescriptor;
unsigned short int serverPort;
struct sockaddr_in serverAddress;
struct hostent *hostInfo;
char buf[LINE_ARRAY_SIZE], c;
	REGISTER_TOPIC_TRAITS(com::netspective::medigy::TempMonitor)
int main(int argc, char* argv[]) 
{
	std::stringstream ss;
	std::stringstream TimeStamp;
	std::string partition = "temp";
	dds::Runtime runtime(partition);
	dds::Duration cleanup_delay = {3600, 0};
	dds::TopicQos tQos;
	tQos.set_persistent();
	tQos.set_reliable();
	tQos.set_keep_last(10);
	tQos.set_durability_service(cleanup_delay,DDS::KEEP_LAST_HISTORY_QOS,1024,8192,4196,8192);
	dds::Topic<com::netspective::medigy::TempMonitor> topic("TempMonitor", tQos);
	dds::DataWriterQos dwQos(tQos);
	dwQos.set_auto_dispose(false);
	dds::DataWriter<com::netspective::medigy::TempMonitor> dw(topic, dwQos);
	std::string tweet="test";
	com::netspective::medigy::TempMonitor tt;
	/*Connection with test data generator*/
	hostInfo = gethostbyname("127.0.0.1");
	if (hostInfo == NULL) 
	{
		cout << "problem interpreting host: " << buf << "\n";
		exit(1);
	}
	serverPort=5000;
	cin.get(c); // dispose of the newline
	socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (socketDescriptor < 0) 
	{
		cerr << "cannot create socket\n";
		exit(1);
	}
	serverAddress.sin_family = hostInfo->h_addrtype;
	memcpy((char *) &serverAddress.sin_addr.s_addr,
			hostInfo->h_addr_list[0], hostInfo->h_length);
	serverAddress.sin_port = htons(serverPort);
	if (connect(socketDescriptor,(struct sockaddr *) &serverAddress,sizeof(serverAddress)) < 0) 
	{
		cerr << "cannot connect\n";
		exit(1);
	}
	strcpy(buf,"TEMPMONITOR");
	if (send(socketDescriptor, buf, strlen(buf) + 1, 0) < 0)
	{
		cerr << "cannot send data ";
		close(socketDescriptor);
		exit(1);
	}
	while (1) 
	{
		while (recv(socketDescriptor, buf, 50, 0) > 0) 
		{
			std::cout <<buf<<"\n";
			char * pch;
			pch = strtok (buf,":");
			tt.timeOfMeasurement = atol(pch);
			pch = strtok (NULL, ":");
			tt.temp = (short)atoi(pch);
			dw.write(tt);
		}
	}
	return 0;
}
