#include <iostream>
#include <dds/dds.hpp>
#include "ccpp_bp.h"
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
namespace po = boost::program_options;
int socketDescriptor;
unsigned short int serverPort;
struct sockaddr_in serverAddress;
struct hostent *hostInfo;
char buf[LINE_ARRAY_SIZE], c;
string domainid,deviceid,logfile;
int spawn,flag;

REGISTER_TOPIC_TRAITS(com::netspective::medigy::BloodPressure)
bool parse_args(int argc, char* argv[])
{
  po::options_description desc("Available options for <BloodPressure Publisher> are");
  desc.add_options()
    ("help", "produce help message")
    ("Domain", po::value<std::string>(), "Device Domain ")
    ("Device-ID",po::value<std::string>(), "Device ID - for device identification")
    ("Spawn", po::value<int>(), "Number of Releses from Publisher")
    ("Log-file", po::value<std::string>(), "Log File Location")
    ;
 
  try {
    po::variables_map vm;        
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);    
   
    if (vm.count("help") || argc == 1) {
      std::cout << desc << "\n";
      return false;
    }
    
    
    if (vm.count("Domain")) 
      domainid = vm["Domain"].as<std::string>();

    if (vm.count("Device-ID")) 
      deviceid = vm["Device-ID"].as<std::string>();

    if (vm.count("Spawn")) 
      spawn = vm["Spawn"].as<int>();
    
    if (vm.count("Log-file")) 
      logfile = vm["Log-file"].as<std::string>();
    
  } 
  catch (...) {
    std::cout << desc << "\n";
    return false;
  }
  return true;
}



int main(int argc, char* argv[]) 
{

	if (!parse_args(argc, argv))
    	return 1;

	std::stringstream ss;
	std::stringstream TimeStamp;
	std::string partition = "blood";
	dds::Runtime runtime(partition);
	dds::Duration cleanup_delay = {3600, 0};
	dds::TopicQos tQos;
	tQos.set_persistent();
	tQos.set_reliable();
	tQos.set_keep_last(10);
	tQos.set_durability_service(cleanup_delay,DDS::KEEP_LAST_HISTORY_QOS,1024,8192,4196,8192);
	dds::Topic<com::netspective::medigy::BloodPressure> topic("BloodPressure", tQos);
	dds::DataWriterQos dwQos(tQos);
	dwQos.set_auto_dispose(false);
	dds::DataWriter<com::netspective::medigy::BloodPressure> dw(topic, dwQos);
	std::string tweet="test";
	com::netspective::medigy::BloodPressure tt;

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
	strcpy(buf,"BP");
	if (send(socketDescriptor, buf, strlen(buf) + 1, 0) < 0)
	{
		cerr << "cannot send data ";
		close(socketDescriptor);
		exit(1);
	}

	flag=0;
	tt.deviceDomain = DDS::string_dup(domainid.c_str());
	tt.deviceID = DDS::string_dup(deviceid.c_str());
	
	std::cout<<"\n"<<tt.deviceDomain<<"         "<<tt.deviceID<<"\n";
	while (1) 
	{
		
		while (recv(socketDescriptor, buf, 50, 0) > 0) 
		{
			std::cout <<buf<<"\n";
			char * pch;
			pch = strtok (buf,":");
			tt.timeOfMeasurement = atol(pch);
			pch = strtok (NULL, ":");
			tt.systolicPressure = (short)atoi(pch);		
			pch = strtok (NULL, ":");
			tt.diastolicPressure = (short)atoi(pch);
			pch = strtok (NULL, ":");
			tt.pulseRatePerMinute = (short)atoi (pch);
			dw.write(tt);
			if(flag == spawn)
			{
				exit(1);
			}
			flag++;
		}

	}
	return 0;
}

