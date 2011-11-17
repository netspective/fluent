#include <iostream>
#include "SimpleDDS.h"
#include <dds/dds.hpp>
#include "ccpp_pulseox.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/SimpleLayout.hh>
#include <ctime>
#include <boost/program_options.hpp>
#include "Functions.h"

using namespace DDS;
using namespace std;
namespace po = boost::program_options;
using namespace com::netspective::medigy;

int socketDescriptor;
unsigned short int serverPort;
struct sockaddr_in serverAddress;
struct hostent *hostInfo;
char buf[1024], c;
int sizebuf;
string domainid,deviceid,logfile,logcate,logcatedata,logconfpath,hostip;

int main(int argc, char* argv[]) 
{
	if (!parse_args_pub(argc, argv,hostip,domainid,deviceid,logfile,logcate,logcatedata,logconfpath))
        return 1;
	
	/*Importing log4cpp configuration and Creating category*/
	log4cpp::PropertyConfigurator::configure(replaceconfstring(logfile,logconfpath,logcate,logcatedata).c_str());
 	remove(logcate.c_str());	
	log4cpp::Category& category = log4cpp::Category::getInstance(logcate);
 	log4cpp::Category& categoryData = log4cpp::Category::getInstance(logcatedata);
	category.setAdditivity(true);
	categoryData.setAdditivity(true);
 	category.addAppender(log4cpp::Appender::getAppender(std::string("infoFILE")));
 	categoryData.addAppender(log4cpp::Appender::getAppender(std::string("dataFILE")));
        category.info(" Pulse_MOnitor  Publisher Started");
       	
	/*Initializing SimpleDDS library*/
	SimpleDDS *simpledds;
	PulseOximeterTypeSupport_var typesupport;
	DataWriter_ptr writer;
	PulseOximeterDataWriter_var bpWriter;

	/*Setting QoS Properities For Topic*/
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
	hostInfo=gethostbyname(hostip.c_str());
	PulseOximeter data;
	
	/*Initializing Socket Data*/
	if (hostInfo == NULL) 
	{	
		category.error("Problem interpreting By HostInfo ");
	
	        category.notice(" Pulse Oximeter Publisher Ends ");
		exit(1);
	}
	serverPort=5000;
	cin.get(c); 
	socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (socketDescriptor < 0) 
	{
		category.error("Not able create the Socket " );
		category.notice(" Pulse Oximeter Publisher Ends ");
		exit(1);
		
	}
	serverAddress.sin_family = hostInfo->h_addrtype;
	memcpy((char *) &serverAddress.sin_addr.s_addr,
			hostInfo->h_addr_list[0], hostInfo->h_length);
	serverAddress.sin_port = htons(serverPort);

	/*Binding Socket with Server Data Generator*/
	if (connect(socketDescriptor,(struct sockaddr *) &serverAddress,sizeof(serverAddress)) < 0) 
	{
		category.error(" cannot connect with server");
		category.notice(" Pulse Oximeter Publisher Ends ");
		exit(1);
	}

	/*Sending Command To Data Generator*/
	strcpy(buf,"PULSEOX");
	if (send(socketDescriptor, buf, strlen(buf) + 1, 0) < 0)
	{
		category.error(" Not able to send data");
		close(socketDescriptor);
		category.error(" Pulse Oximeter Publisher Ends");
		exit(1);
	}

	/*Storing Domain and Device ID*/
	data.deviceID = DDS::string_dup(deviceid.c_str());
	data.deviceDomain = DDS::string_dup(domainid.c_str());
	while (1) 
	{

		while ((sizebuf=recv(socketDescriptor, buf, 50, 0)) > 0) 
		{
			buf[sizebuf]='\0';
			categoryData.info(buf);
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

	/*Deleting SimpleDDS Instance*/
	category.info("Pulse Oximeter Publisher Ends ");
	simpledds->deleteWriter(writer);
	delete simpledds;
	return 0;
}
