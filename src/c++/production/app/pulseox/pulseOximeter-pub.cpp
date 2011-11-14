#include <iostream>
#include "SimpleDDS.h"
#include <dds/dds.hpp>
#include "ccpp_pulseox.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/SimpleLayout.hh>
#include <ctime>
#include <boost/program_options.hpp>
#define MAX_LINE 100
#define LINE_ARRAY_SIZE (MAX_LINE+1)
using namespace DDS;
using namespace std;
namespace po = boost::program_options;
using namespace com::netspective::medigy;
int socketDescriptor;
unsigned short int serverPort;
struct sockaddr_in serverAddress;
struct hostent *hostInfo;
char buf[LINE_ARRAY_SIZE], c;
int sizebuf;
string pubtopic,domainid,deviceid,logfile;
string current_time()
{
	time_t rawtime;
	time ( &rawtime );	
	string str = ctime(&rawtime);
	return str.substr(0,str.size()-1);

}
bool parse_args(int argc, char* argv[])
{
  po::options_description desc("Available options for <Pulse Oximeter Publisher> are");
  desc.add_options()
    ("help", "produce help message")
    ("domain", po::value<std::string>(), "Device Domain ")
    ("device-id",po::value<std::string>(), "Device ID - for device identification")
    ("log-file", po::value<std::string>(), "Log File Location")
    ;
 
  try {
    po::variables_map vm;        
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);    
   
    if (vm.count("help") || argc == 1) {
      std::cout << desc << "\n";
      return false;
    }
    
    
    if (vm.count("domain")) 
      domainid = vm["domain"].as<std::string>();

    if (vm.count("device-id")) 
	{
      deviceid = vm["device-id"].as<std::string>();
	//string key ("{");
  	//size_t start,end;
  	//string key1 ("}");
        //start=deviceid.rfind(key);
        //end=deviceid.rfind(key1);
        //pubtopic = deviceid.substr(0,start)+deviceid.substr(start+1,end-start-1);
	//cout<<"\n"<<pubtopic;	
	}

    if (vm.count("log-file")) 
      logfile = vm["log-file"].as<std::string>();
    
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
	log4cpp::Appender *appender = new log4cpp::FileAppender("FileAppender",logfile);
	log4cpp::Layout *layout = new log4cpp::SimpleLayout();
	log4cpp::Category& category = log4cpp::Category::getInstance("Category");
	appender->setLayout(layout);
    	category.setAppender(appender);
	category.info("Pulse Oximeter Publisher Started "+current_time());
	std::stringstream ss;
	std::stringstream TimeStamp;
	std::string partition = "pulseox";
	
	SimpleDDS *simpledds;
	PulseOximeterTypeSupport_var typesupport;
	DataWriter_ptr writer;
	PulseOximeterDataWriter_var bpWriter;
	DDS::TopicQos tQos;
	tQos.durability.kind=VOLATILE_DURABILITY_QOS;
	tQos.reliability.kind=BEST_EFFORT_RELIABILITY_QOS;
	tQos.history.depth=10;
	tQos.durability_service.history_kind = KEEP_LAST_HISTORY_QOS;
	tQos.durability_service.history_depth= 1024;
	simpledds = new SimpleDDS(tQos);
	typesupport = new PulseOximeterTypeSupport();
	writer = simpledds->publish(typesupport);
	bpWriter = PulseOximeterDataWriter::_narrow(writer);
	PulseOximeter data;

	/*Connection with test data generator*/
	hostInfo = gethostbyname("127.0.0.1");
	if (hostInfo == NULL) 
	{	
		category.info("Problem interpreting By HostInfo "+current_time());
		//cout << "problem interpreting host: " << buf << "\n";
		 category.info(" Pulse Oximeter Publisher Ends "+current_time());
		exit(1);
	}
	serverPort=5000;
	cin.get(c); // dispose of the newline
	socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (socketDescriptor < 0) 
	{
		category.info("Not able create the Socket " +current_time());
		category.info(" Pulse Oximeter Publisher Ends "+current_time());
		exit(1);
		
	}
	serverAddress.sin_family = hostInfo->h_addrtype;
	memcpy((char *) &serverAddress.sin_addr.s_addr,
			hostInfo->h_addr_list[0], hostInfo->h_length);
	serverAddress.sin_port = htons(serverPort);


	if (connect(socketDescriptor,(struct sockaddr *) &serverAddress,sizeof(serverAddress)) < 0) 
	{
		category.error(current_time() +" cannot connect with server");
		category.info(current_time() +" Pulse Oximeter Publisher Ends ");
		exit(1);
	}
	strcpy(buf,"PULSEOX");

	if (send(socketDescriptor, buf, strlen(buf) + 1, 0) < 0)
	{
		category.info(current_time()+" Not able to send data");
		category.info(current_time()+" Pulse Oximeter Publisher Ends ");
		close(socketDescriptor);
		exit(1);
	}
	data.deviceID = DDS::string_dup(deviceid.c_str());
	data.deviceDomain = DDS::string_dup(domainid.c_str());
	while (1) 
	{
		while ((sizebuf=recv(socketDescriptor, buf, 50, 0)) > 0) 
		{
			buf[sizebuf]='\0';
			std::cout <<buf<<"\n";
			char * pch;
			pch = strtok (buf,":");
			data.timeOfMeasurement = atol(pch);
			pch = strtok (NULL, ":");
			data.SPO2 = (short)atoi(pch);		
			pch = strtok (NULL, ":");
			data.pulseRatePerMinute = (short)atoi (pch);
			bpWriter->write(data, NULL);
			sleep(1);
		}
	}
	category.info(current_time()+" Pulse Oximeter Publisher Ends ");
	simpledds->deleteWriter(writer);
	delete simpledds;
	return 0;
}
