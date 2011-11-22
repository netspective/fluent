#include "SimpleDDS.h"
#include <iostream>
#include <dds/dds.hpp>
#include "ccpp_bp.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctime>
#include <boost/program_options.hpp>
#include "Functions.h"
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/Configurator.hh>

using namespace DDS;
using namespace std;
namespace po = boost::program_options;
using namespace com::netspective::medigy;

int socketDescriptor;
unsigned short int serverPort;
struct sockaddr_in serverAddress;
struct hostent *hostInfo;
char buf[1024], c;
int spawn,flag,sizebuf;
string domainid,deviceid,loginfo,logdata,logconfpath,hostip;
stringstream prtemp;
int main(int argc, char* argv[]) 
{
	if (!parse_args_pub(argc, argv,hostip,domainid,deviceid,loginfo,logdata,logconfpath))
    	return 1;
	
	/*Importing log4cpp configuration and Creating category*/
	log4cpp::Category &log_root = log4cpp::Category::getRoot();
    	log4cpp::Category &bloodInfo = log4cpp::Category::getInstance( std::string(loginfo));
    	log4cpp::Category &bloodData = log4cpp::Category::getInstance( std::string(logdata));
	log4cpp::PropertyConfigurator::configure(logconfpath);
	bloodInfo.notice(" Blood Pressure Publisher Started "+deviceid);
	
	/*Initializing SimpleDDS library*/
	SimpleDDS *simpledds;
	BloodPressureTypeSupport_var typesupport;
	DataWriter_ptr writer;
	BloodPressureDataWriter_var bpWriter;

	/*Setting QoS Properties for Topic*/
	DDS::TopicQos tQos;
	tQos.durability.kind=VOLATILE_DURABILITY_QOS;
	tQos.reliability.kind=BEST_EFFORT_RELIABILITY_QOS;
	tQos.history.depth=10;
	tQos.durability_service.history_kind = KEEP_LAST_HISTORY_QOS;
	tQos.durability_service.history_depth= 1024;
	simpledds = new SimpleDDS(tQos);
	typesupport = new BloodPressureTypeSupport();
	writer = simpledds->publish(typesupport);
	bpWriter = BloodPressureDataWriter::_narrow(writer);
	hostInfo = gethostbyname(hostip.c_str());

	/*Initializing Socket Data*/
	if (hostInfo == NULL) 
	{
		bloodInfo.error(" Problem interpreting By HostInfo");
		bloodInfo.notice(" BloodPressure Publisher Ends");	
		exit(1);
	}
	serverPort=5000;
	cin.get(c); 
	socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (socketDescriptor < 0) 
	{
		bloodInfo.error(" Not able create the Socket");
		bloodInfo.notice(" BloodPressure Publisher Ends");	
		exit(1);
	}
	serverAddress.sin_family = hostInfo->h_addrtype;
	memcpy((char *) &serverAddress.sin_addr.s_addr,hostInfo->h_addr_list[0], hostInfo->h_length);
	serverAddress.sin_port = htons(serverPort);

	/*Binding Socket with Server Data Generator*/
	if (connect(socketDescriptor,(struct sockaddr *) &serverAddress,sizeof(serverAddress)) < 0) 
	{
		bloodInfo.error(" cannot connect with server");
		bloodInfo.notice(" BloodPressure Publisher Ends at ");
		exit(1);
	}

	/*Sending Commmand to The Data Generator*/
	strcpy(buf,"BP");
	if (send(socketDescriptor, buf, strlen(buf) + 1, 0) < 0)
	{
		bloodInfo.error(" Not able to send data");
		close(socketDescriptor);
		bloodInfo.notice(" BloodPressure Publisher Ends");
		exit(1);
	}

	flag=0;
	BloodPressure data;

	/*Storing Domain and Device ID*/
	data.deviceID = DDS::string_dup(deviceid.c_str());
	data.deviceDomain = DDS::string_dup(domainid.c_str());
	bloodInfo.notice("Blood Pressure Started Publishing data in DDS");
	bloodInfo.notice("Format: TIMEOFMEASURED, SYSTOLIC, DIATOLIC, PULSERATE");	
	while (1) 
	{
		
		while ((sizebuf=recv(socketDescriptor, buf, 50, 0)) > 0) 
		{
			buf[sizebuf]='\0';
			char * pch;
			pch = strtok (buf,":");
			data.timeOfMeasurement = atol(pch);
			prtemp<<data.timeOfMeasurement<<", ";
			pch = strtok (NULL, ":");
			data.systolicPressure = (short)atoi(pch);		
			prtemp<<data.systolicPressure<<", ";
			pch = strtok (NULL, ":");
			data.diastolicPressure = (short)atoi(pch);
			prtemp<<data.diastolicPressure<<", ";
			pch = strtok (NULL, ":");
			data.pulseRatePerMinute = (short)atoi (pch);
			prtemp<<data.pulseRatePerMinute;
			bloodData.info(prtemp.str().c_str());
			bpWriter->write(data, NULL);
			prtemp.str("");
		}

	}
	
	/*Deleting SimpleDDS Instance*/
	bloodInfo.notice(" BloodPressure Publisher Ends");
	simpledds->deleteWriter(writer);
	delete simpledds;
	return 0;
}

