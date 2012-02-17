#include "SimpleDDS.h"
#include <iostream>
#include "ccpp_ecg.h"
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

int spawn,flag,sizebuf,port,heartbeats,ecgsample,internalsample;
float amplitudenoise,heart_rate_mean,heart_rate_std,lowfreq,highfreq,lowfreqstd,highfreqstd,lfhfradio;
string domainid,deviceid,loginfo,logdata,logconfpath,hostip;
stringstream prtemp,datacommand;
int main(int argc, char* argv[]) 
{
	heartbeats=256;                 
	ecgsample=256;               
	internalsample=256;
	amplitudenoise=0.0;          
	heart_rate_mean=60.0;        
	heart_rate_std=1.0;          
	lowfreq=0.1;             
	highfreq=0.25;           
	lowfreqstd=0.01;         
	highfreqstd=0.01;        
	lfhfradio=0.5; 

	if (!parse_args_pub_ecg(argc,argv,hostip,port,domainid,deviceid,loginfo,logdata,logconfpath,heartbeats,ecgsample,internalsample,
amplitudenoise,heart_rate_mean,heart_rate_std,lowfreq,highfreq,lowfreqstd,highfreqstd,lfhfradio))
    	return 1;
	
	/*Importing log4cpp configuration and Creating category*/
	log4cpp::Category &log_root = log4cpp::Category::getRoot();
    	log4cpp::Category &bloodInfo = log4cpp::Category::getInstance( std::string(loginfo));
    	log4cpp::Category &bloodData = log4cpp::Category::getInstance( std::string(logdata));
	log4cpp::PropertyConfigurator::configure(logconfpath);
	bloodInfo.notice(" ecg Publisher Started "+deviceid);
	
	/*Initializing SimpleDDS library*/
	SimpleDDS *simpledds;
	ECGTypeSupport_var typesupport;
	DataWriter_ptr writer;
	ECGDataWriter_var ecgWriter;


	simpledds = new SimpleDDS();
	typesupport = new ECGTypeSupport();
	writer = simpledds->publish(typesupport);
	ecgWriter = ECGDataWriter::_narrow(writer);
	hostInfo = gethostbyname(hostip.c_str());

	/*Initializing Socket Data*/
	if (hostInfo == NULL) 
	{
		bloodInfo.error(" Problem interpreting By HostInfo");
		bloodInfo.notice(" ecg Publisher Ends");	
		//exit(1);
	}
	serverPort=port;
	cin.get(c); 
	socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (socketDescriptor < 0) 
	{
		bloodInfo.error(" Not able create the Socket");
		bloodInfo.notice(" ecg Publisher Ends");	
		//exit(1);
	}
	serverAddress.sin_family = hostInfo->h_addrtype;
	memcpy((char *) &serverAddress.sin_addr.s_addr,hostInfo->h_addr_list[0], hostInfo->h_length);
	serverAddress.sin_port = htons(serverPort);

	/*Binding Socket with Server Data Generator*/
	if (connect(socketDescriptor,(struct sockaddr *) &serverAddress,sizeof(serverAddress)) < 0) 
	{
		bloodInfo.error(" cannot connect with server");
		bloodInfo.notice(" ecg Publisher Ends at ");
		//exit(1);
	}

	/*Sending Commmand to The Data Generator*/
	datacommand<<"ECG"<<SEMI<<heartbeats<<SEMI<<ecgsample<<SEMI<<internalsample<<SEMI<<amplitudenoise;
	datacommand<<SEMI<<heart_rate_mean<<SEMI<<heart_rate_std<<SEMI<<lowfreq<<SEMI<<highfreq<<SEMI<<lowfreqstd<<SEMI<<highfreqstd<<SEMI<<lfhfradio;
	strcpy(buf,datacommand.str().c_str());
	if (send(socketDescriptor, buf, strlen(buf) + 1, 0) < 0)
	{
		bloodInfo.error(" Not able to send data");
		close(socketDescriptor);
		bloodInfo.notice(" ecg Publisher Ends");
		//exit(1);
	}

	flag=0;
	ECG data;

	/*Storing Domain and Device ID*/
	data.deviceID = DDS::string_dup(deviceid.c_str());
	data.deviceDomain = DDS::string_dup(domainid.c_str());
	bloodInfo.notice("ecg Started Publishing Data In DDS");
	bloodInfo.notice("Format: DOMAIN, DEVICEID, TIMEOFMEASURED, TimeInSeconds, AmplitudeMillivolts, WaveformPeak, StandardDeviation, Signal");	
	long count=0;
	while (1) 
	{
		
		if ((sizebuf=recv(socketDescriptor, buf, 1024,  MSG_NOSIGNAL)) > 0) 
		{
			cout<<"\n"<<buf;
			buf[sizebuf]='\0';
			string datasplit[7];
			datasplit[0] = strtok (buf,SEMI);
			datasplit[1] = strtok (NULL,SEMI);
			datasplit[2] = strtok (NULL,SEMI);
			datasplit[3] = strtok (NULL,SEMI);
			datasplit[4] = strtok (NULL,SEMI);
			datasplit[5] = strtok (NULL,SEMI);

			prtemp<<domainid<<COMMA<<deviceid<<COMMA;

			data.timeOfMeasurement = atol(datasplit[0].c_str());
			prtemp<<data.timeOfMeasurement<<COMMA;

			data.timeInSeconds = (double)atof(datasplit[1].c_str());		
			prtemp<<data.timeInSeconds<<COMMA;

                        data.amplitudeMillivolts = (double)atof(datasplit[2].c_str());   
                        prtemp<<data.amplitudeMillivolts<<COMMA;
			
			data.waveformPeak = (short)atoi(datasplit[3].c_str());   
                        prtemp<<data.waveformPeak<<COMMA;
					
			data.standardDeviation = (double)atof(datasplit[4].c_str());
			prtemp<<data.standardDeviation<<COMMA;


			data.signalECG = (double)atof(datasplit[5].c_str());
			prtemp<<data.signalECG;

			//bloodData.info(prtemp.str().c_str());
			if (RETCODE_OUT_OF_RESOURCES  == ecgWriter->write(data, NULL))
			{ cout << "\n Out of Resource ";}

			cout<<"\n"<<prtemp.str().c_str()<<count++;
			int newflag=-1;
			while(newflag <= 0)
			{
				newflag=send(socketDescriptor,prtemp.str().c_str(),1024,  MSG_NOSIGNAL);
				
				
			}
			prtemp.str(CLEAN);
			fflush(stdout);
						
		}
		
		
	

	}
	
	/*Deleting SimpleDDS Instance*/
	bloodInfo.notice(" ecg Publisher Ends");
	simpledds->deleteWriter(writer);
	delete simpledds;
	return 0;
}

