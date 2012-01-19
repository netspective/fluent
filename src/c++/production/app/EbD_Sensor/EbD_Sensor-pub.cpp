#include "SimpleDDS.h"
#include <iostream>
#include <dds/dds.hpp>
#include "ccpp_EbD_Sensor.h"
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
char buf[BUFFERSIZE], c;
int spawn,flag,sizebuf,port;
string domainid,deviceid,loginfo,logdata,logconfpath,hostip;
stringstream prtemp;
int main(int argc, char* argv[]) 
{
	if (!parse_args_pub(argc, argv,hostip, port,domainid,deviceid,loginfo,logdata,logconfpath))
    	return 1;
	
	/*Importing log4cpp configuration and Creating category*/
	log4cpp::Category &log_root = log4cpp::Category::getRoot();
    	log4cpp::Category &sensorInfo = log4cpp::Category::getInstance( std::string(loginfo));
    	log4cpp::Category &sensorData = log4cpp::Category::getInstance( std::string(logdata));
	log4cpp::PropertyConfigurator::configure(logconfpath);
	sensorInfo.notice(" EbD_Sensor Publisher Started "+deviceid);
	
	/*Initializing SimpleDDS library*/
	SimpleDDS *simpledds;
	EbD_SensorTypeSupport_var typesupport;
	DataWriter_ptr writer;
	EbD_SensorDataWriter_var EbD_SensorWriter;

	simpledds = new SimpleDDS();
	typesupport = new EbD_SensorTypeSupport();
	writer = simpledds->publish(typesupport);
	EbD_SensorWriter = EbD_SensorDataWriter::_narrow(writer);
	hostInfo = gethostbyname(hostip.c_str());

	/*Initializing Socket Data*/
	if (hostInfo == NULL) 
	{
		sensorInfo.error(" Problem interpreting By HostInfo");
		sensorInfo.notice(" EbD_Sensor Publisher Ends");	
		exit(1);
	}
	serverPort=port;
	cin.get(c); 
	socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (socketDescriptor < 0) 
	{
		sensorInfo.error(" Not able create the Socket");
		sensorInfo.notice(" EbD_Sensor Publisher Ends");	
		exit(1);
	}
	serverAddress.sin_family = hostInfo->h_addrtype;
	memcpy((char *) &serverAddress.sin_addr.s_addr,hostInfo->h_addr_list[0], hostInfo->h_length);
	serverAddress.sin_port = htons(serverPort);

	/*Binding Socket with Server Data Generator*/
	if (connect(socketDescriptor,(struct sockaddr *) &serverAddress,sizeof(serverAddress)) < 0) 
	{
		sensorInfo.error(" cannot connect with server");
		sensorInfo.notice(" EbD_Sensor Publisher Ends at ");
		exit(1);
	}

	/*Sending Commmand to The Data Generator*/
	strcpy(buf,"sensor");
	if (send(socketDescriptor, buf, strlen(buf) + 1, 0) < 0)
	{
		sensorInfo.error(" Not able to send data");
		close(socketDescriptor);
		sensorInfo.notice(" EbD_Sensor Publisher Ends");
		exit(1);
	}

	flag=0;
	EbD_Sensor data;

	/*Storing Domain and Device ID*/
	data.deviceID = DDS::string_dup(deviceid.c_str());
	data.deviceDomain = DDS::string_dup(domainid.c_str());
	sensorInfo.notice("EbD_Sensor Started Publishing Data In DDS");
	sensorInfo.notice("Format: DOMAIN, DEVICEID, TIMEOFMEASURED, SYSTOLIC, DIASTOLIC, PULSERATE");	
	while (1) 
	{
		
		while ((sizebuf=recv(socketDescriptor, buf, 1024, MSG_NOSIGNAL)) > 0) 
		{
			buf[sizebuf]='\0';
			char * pch;
			prtemp<<domainid<<COMMA<<deviceid<<COMMA;

			pch = strtok (buf,SEMI);
			data.timeOfMeasurement = atol(pch);
			prtemp<<data.timeOfMeasurement<<COMMA;

			pch = strtok (NULL, SEMI);
			data.pAvgLoad = (long)atoi(pch);
			prtemp<<data.pAvgLoad<<COMMA;

			pch = strtok (NULL, SEMI);
			data.iRMS = (long)atol (pch);
			prtemp<<data.iRMS<<COMMA;

			pch = strtok (NULL, SEMI);
			data.vRMS = (long)atol (pch);
			prtemp<<data.vRMS<<COMMA;

			pch = strtok (NULL, SEMI);
			data.zLoad = (long)atol (pch);
			prtemp<<data.zLoad<<COMMA;

			pch = strtok (NULL, SEMI);
			data.timeStamp = (float)atof(pch);		
			prtemp<<data.timeStamp;

			cout <<"\n\n"<<prtemp.str().c_str();
			sensorData.info(prtemp.str().c_str());
			EbD_SensorWriter->write(data, NULL);
			prtemp.str(CLEAN);
			int newflag=-1;
			while(newflag <= 0)
			{
				newflag=send(socketDescriptor,"recv",3,  MSG_NOSIGNAL);
				
				
			}

		}

	}
	
	/*Deleting SimpleDDS Instance*/
	sensorInfo.notice(" EbD_Sensor Publisher Ends");
	simpledds->deleteWriter(writer);
	delete simpledds;
	return 0;
}

