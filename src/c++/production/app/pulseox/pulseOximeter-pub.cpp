			//PULSEOX -PUB//
#include <iostream>
#include "SimpleDDS.h"
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
#include<log4cpp/Configurator.hh>

using namespace DDS;
using namespace std;
namespace po = boost::program_options;
using namespace com::netspective::medigy;

int socketDescriptor;
unsigned short int serverPort;
struct sockaddr_in serverAddress;
struct hostent *hostInfo;
char buf[BUFFERSIZE], c;
int sizebuf,port;
string domainid,deviceid,loginfo,logdata,logconfpath,hostip;
stringstream prtemp;
int main(int argc, char* argv[]) 
{
	if (!parse_args_pub(argc, argv,hostip,port,domainid,deviceid,loginfo,logdata,logconfpath))
        return 1;
	
	/*Importing log4cpp configuration and Creating category*/
        log4cpp::Category &log_root = log4cpp::Category::getRoot();
        log4cpp::Category &pulseInfo = log4cpp::Category::getInstance( std::string(loginfo));
        log4cpp::Category &pulseData = log4cpp::Category::getInstance( std::string(logdata));
        log4cpp::PropertyConfigurator::configure(logconfpath);
        pulseInfo.notice(" Pulse Oximeter Publisher Started"+deviceid);
       	
	/*Initializing SimpleDDS library*/
	SimpleDDS *simpledds;
	PulseOximeterTypeSupport_var typesupport;
	DataWriter_ptr writer;
	PulseOximeterDataWriter_var bpWriter;

	/*Setting QoS Properties for Topic*/
	DDS::TopicQos tQos;
	getQos(tQos);

	simpledds = new SimpleDDS(tQos);
	typesupport = new PulseOximeterTypeSupport();
	writer = simpledds->publish(typesupport);
	bpWriter = PulseOximeterDataWriter::_narrow(writer);
	hostInfo=gethostbyname(hostip.c_str());
	
	/*Initializing Socket Data*/
	if (hostInfo == NULL) 
	{	
		pulseInfo.error("Problem interpreting By HostInfo ");
	        pulseInfo.notice(" Pulse Oximeter Publisher Ends ");
		
	}
	serverPort=port;
	cin.get(c); 
	socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (socketDescriptor < 0) 
	{
		pulseInfo.error("Not able create the Socket " );
		pulseInfo.notice("Pulse Oximeter Publisher Ends ");
		exit(1);
		
	}
	serverAddress.sin_family = hostInfo->h_addrtype;
	memcpy((char *) &serverAddress.sin_addr.s_addr,	hostInfo->h_addr_list[0], hostInfo->h_length);
	serverAddress.sin_port = htons(serverPort);

	/*Binding Socket with Server Data Generator*/
	if (connect(socketDescriptor,(struct sockaddr *) &serverAddress,sizeof(serverAddress)) < 0) 
	{
		pulseInfo.error(" cannot connect with server");
		pulseInfo.notice(" Pulse Oximeter Publisher Ends ");
		exit(1);
	}

	/*Sending Command To Data Generator*/
	strcpy(buf,"PULSEOX");
	if (send(socketDescriptor, buf, strlen(buf) + 1, 0) < 0)
	{
		pulseInfo.error(" Not able to send data");
		close(socketDescriptor);
		pulseInfo.notice(" Pulse Oximeter Publisher Ends");
		exit(1);
	}

	PulseOximeter data;

	/*Storing Domain and Device ID*/
	data.deviceID = DDS::string_dup(deviceid.c_str());
	data.deviceDomain = DDS::string_dup(domainid.c_str());
	pulseInfo.notice("Pulse Oximeter  Started Publishing Data In DDS");
        pulseInfo.notice("Format: TIMEOFMEASURED, OXYGEN_SATURATION, PULSERATE");

	while (1) 
	{

		while ((sizebuf=recv(socketDescriptor, buf, 50, 0)) > 0) 
		{
			buf[sizebuf]='\0';
			char * pch;
			pch = strtok (buf,SEMI);
			data.timeOfMeasurement = atol(pch);
			prtemp<<data.timeOfMeasurement<<COMMA;
			pch = strtok (NULL,SEMI);
			data.SPO2 = (short)atoi(pch);		
			prtemp<<data.SPO2<<COMMA;
			pch = strtok (NULL,SEMI);
			data.pulseRatePerMinute = (short)atoi (pch);
			prtemp<<data.pulseRatePerMinute;
			bpWriter->write(data, NULL);
			pulseData.info(prtemp.str().c_str());
			prtemp.str(CLEAN);
		}
	}

	/*Deleting SimpleDDS Instance*/
	pulseInfo.notice("Pulse Oximeter Publisher Ends "+deviceid);
	simpledds->deleteWriter(writer);
	delete simpledds;
	return 0;
}
