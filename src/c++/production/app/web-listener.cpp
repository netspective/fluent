#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <sys/time.h>
#include <iostream>
#define devid "deviceID"
#include "websocketpp.hpp"
#include <boost/asio.hpp>
#include "web-listener.hpp"
#include <boost/algorithm/string/replace.hpp>
#include "SimpleDDS.h"
#include "ccpp_bp.h"
#include "ccpp_pulseox.h"
#include "ccpp_temperature.h"
#include "ccpp_ecg.h"
#include <dds/runtime.hpp>
#include <dds/topic.hpp>
#include <dds/reader.hpp>
#include <dds/traits.hpp>
#include <dds/dds.hpp>

using websocketweb::web_server_handler;
using websocketpp::session_ptr;
using namespace DDS;
using namespace std;
using namespace com::netspective::medigy;

void web_server_handler::validate(session_ptr client) 
{
	std::stringstream err;
	std::cout<< "\n Client : "<<client <<" Resource :  " <<client->get_resource()<< "Origin  : "<< client->get_origin() <<"\n";
}


void web_server_handler::on_open(session_ptr client) 
{
	std::cout << "client " << client << " joined " << std::endl;
	m_connections.insert(std::pair<session_ptr,std::string>(client,get_con_id(client)));
	send_to_all(serialize_state());
	//client->send(encode_message("server","connected"));
	//send_to_all(encode_message("server",m_connections[client]+" has joined the web."));
}

void web_server_handler::on_close(session_ptr client) 
{
	std::map<session_ptr,std::string>::iterator it = m_connections.find(client);
	if (it == m_connections.end()) 
	{
		return;
	}
	std::cout << "client " << client << " left from the Web Server." << std::endl;
	const std::string alias = it->second;
	m_connections.erase(it);
	send_to_all(serialize_state());
	//send_to_all(encode_message("server",alias+" has left the web."));
}
void web_server_handler::on_message(session_ptr client,const std::string &msg) 
{
	temp_client=client;
	temp_msg=msg;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&id, &attr, startThread, this);

}

void *web_server_handler::startThread(void *arg)
{
	
	 web_server_handler test = *((web_server_handler*)arg);
	 session_ptr client=test.temp_client;
	 std::string &msg=test.temp_msg;
	 std::cout << "\n\n"<<"Recevied Message :"<<msg<<"\n";
	 if (msg == "bp") 
	 {
	 std::stringstream prtemp;
	 SimpleDDS *simpledds;
	 BloodPressureTypeSupport_var typesupport;
    	 DataReader_ptr reader;
    	 BloodPressureDataReader_var bpReader;
    	 ReturnCode_t status;
	 int i=0;
         DDS::TopicQos tQos;
	 getQos(tQos);
         tQos.durability_service.history_depth= 1024;
         simpledds = new SimpleDDS(tQos);
	 typesupport = new BloodPressureTypeSupport();
    	 reader = simpledds->subscribe(typesupport);
    	 bpReader = BloodPressureDataReader::_narrow(reader);
   	 BloodPressureSeq  bpList;
     	 SampleInfoSeq     infoSeq;
	 while(1)
	 {
			status = bpReader->take(
            	bpList,
            	infoSeq,
            	LENGTH_UNLIMITED,
            	ANY_SAMPLE_STATE,
           	ANY_VIEW_STATE,
            	ANY_INSTANCE_STATE);
         	checkStatus(status, "take");
          	if (status == RETCODE_NO_DATA) 
		{
          		continue;
          	}
          	for (i = 0; i < bpList.length(); i++) 
	  	{
			if(infoSeq[i].valid_data)
			{
			prtemp <<bpList[i].deviceDomain<<",";
		        prtemp <<bpList[i].deviceID <<","<<bpList[i].timeOfMeasurement<<","<< bpList[i].systolicPressure;
			prtemp <<","<<bpList[i].diastolicPressure<<","<<bpList[i].pulseRatePerMinute;
			client->send(test.encode_message("server",prtemp.str().c_str()));
			
			prtemp.str("");
			}
	  	}
		status = bpReader->return_loan(bpList, infoSeq);
       		checkStatus(status, "return_loan");

	}	
	simpledds->deleteReader(reader);
        delete simpledds;
	std::cout << "message from client " << client << ": " << msg << std::endl;
	}

	if (msg == "pulseox")
	{
		std::stringstream prtemp;
		SimpleDDS *simpledds;
		PulseOximeterTypeSupport_var typesupport;
	    	DataReader_ptr reader;
	    	PulseOximeterDataReader_var bpReader;
	    	ReturnCode_t status;
		int i=0;
	        DDS::TopicQos tQos;
	        tQos.durability.kind=VOLATILE_DURABILITY_QOS;
	        tQos.reliability.kind=BEST_EFFORT_RELIABILITY_QOS;
	        tQos.history.depth=10;
	        tQos.durability_service.history_kind = KEEP_LAST_HISTORY_QOS;
	        tQos.durability_service.history_depth= 1024;
	        simpledds = new SimpleDDS(tQos);
		typesupport = new PulseOximeterTypeSupport();
	    	reader = simpledds->subscribe(typesupport);
	    	bpReader = PulseOximeterDataReader::_narrow(reader);
	   	PulseOximeterSeq  bpList;
	     	SampleInfoSeq     infoSeq;
		while (1) 
		{
	         	status = bpReader->take(
	            	bpList,
	            	infoSeq,
	            	LENGTH_UNLIMITED,
	            	ANY_SAMPLE_STATE,
	           	ANY_VIEW_STATE,
	            	ANY_INSTANCE_STATE);
	         	checkStatus(status, "take");
	          	if (status == RETCODE_NO_DATA) 
			{
	          		continue;
	          	}
	          	for (i = 0; i < bpList.length(); i++) 
		  	{
				if(infoSeq[i].valid_data)
				{
					prtemp <<bpList[i].deviceDomain<<",";
				        prtemp <<bpList[i].deviceID <<","<<bpList[i].timeOfMeasurement<<",";
					prtemp <<bpList[i].SPO2<<","<<bpList[i].pulseRatePerMinute;
					client->send(test.encode_message("server",prtemp.str().c_str()));	
					prtemp.str("");
				
				}
				sleep(1);
			}
			status = bpReader->return_loan(bpList, infoSeq);
	        	checkStatus(status, "return_loan");
		
	  	}
   	     simpledds->deleteReader(reader);
             delete simpledds;

	}
	if (msg == "temp")
	{
		std::stringstream prtemp;
	 	
		SimpleDDS *simpledds;
		TemperatureTypeSupport_var typesupport;
		DataReader_ptr reader;
		TemperatureDataReader_var bpReader;
    	 	ReturnCode_t status;
	 	int i=0;
	       	DDS::TopicQos tQos;
         	tQos.durability.kind=VOLATILE_DURABILITY_QOS;
         	tQos.reliability.kind=BEST_EFFORT_RELIABILITY_QOS;
         	tQos.history.depth=10;
         	tQos.durability_service.history_kind = KEEP_LAST_HISTORY_QOS;
         	tQos.durability_service.history_depth= 1024;
         	simpledds = new SimpleDDS(tQos);
	
		typesupport = new TemperatureTypeSupport();
    	 	reader = simpledds->subscribe(typesupport);
    	 	bpReader = TemperatureDataReader::_narrow(reader);
   	 	TemperatureSeq  bpList;
     	 	SampleInfoSeq     infoSeq;
	 	while (1) 
	 	{
         		status = bpReader->take(
         	   	bpList,
         	   	infoSeq,
         	   	LENGTH_UNLIMITED,
         	   	ANY_SAMPLE_STATE,
         	  	ANY_VIEW_STATE,
         	   	ANY_INSTANCE_STATE);
         		checkStatus(status, "take");
         	 	if (status == RETCODE_NO_DATA) 
			{
         	 		continue;
         	 	}
         	 	for (i = 0; i < bpList.length(); i++) 
	 	 	{

				if(infoSeq[i].valid_data)
				{
					prtemp <<bpList[i].deviceDomain<<",";
				        prtemp <<bpList[i].deviceID <<","<<bpList[i].timeOfMeasurement<<",";
					prtemp <<bpList[i].temp;
					client->send(test.encode_message("server",prtemp.str().c_str()));
					prtemp.str("");
				}
				sleep(1);
				
	
			}
			status = bpReader->return_loan(bpList, infoSeq);
       			checkStatus(status, "return_loan");
		
		}
		simpledds->deleteReader(reader);
       		delete simpledds;
	}	
	if (msg == "ecg")
	{
		std::stringstream prtemp;
	 	
		SimpleDDS *simpledds;
	 	ECGTypeSupport_var typesupport;
    	 	DataReader_ptr reader;
    	 	ECGDataReader_var ecgReader;
    	 	ReturnCode_t status;
	 	int i=0;
	
	 	/*Initializing Subscriber and DataWriter*/
         	simpledds = new SimpleDDS();
	 	typesupport = new ECGTypeSupport();
	
		reader = simpledds->subscribe(typesupport);
    	 	ecgReader = ECGDataReader::_narrow(reader);
   	 	ECGSeq  ecgList;
     	 	SampleInfoSeq     infoSeq;
		long m_count=0;
	 	while (1) 
		{
         		status = ecgReader->take(
            		ecgList,
            		infoSeq,
            		LENGTH_UNLIMITED,
            		ANY_SAMPLE_STATE,
           		ANY_VIEW_STATE,
            		ANY_INSTANCE_STATE);
         		checkStatus(status, "take");
          		if (status == RETCODE_NO_DATA) 
			{
          			continue;
          		}
          		for (i = 0; i < ecgList.length(); i++) 
	  		{
				if(infoSeq[i].valid_data)
				{
					prtemp <<ecgList[i].deviceDomain<<",";
		        		prtemp <<ecgList[i].deviceID <<","<<ecgList[i].timeOfMeasurement<<","<< ecgList[i].timeInSeconds;
					prtemp <<","<<ecgList[i].amplitudeMillivolts<<","<<ecgList[i].waveformPeak;
					client->send(test.encode_message("server",prtemp.str().c_str()));
					//cout << prtemp.str().c_str()<< "\n"<<m_count++<<"\n";
					prtemp.str("");
									
				}
		  	}
			status = ecgReader->return_loan(ecgList, infoSeq);
	       		checkStatus(status, "return_loan");
       
    		}
		simpledds->deleteReader(reader);
       		delete simpledds;
	}

	
	if (msg[0] == '/') {
		client->send(test.encode_message("server","unrecognized command"));
	}
	
	// create JSON message to send based on msg
	test.send_to_all(test.encode_message(test.m_connections[client],msg));
	
    pthread_exit(NULL);
	
}

std::string web_server_handler::serialize_state() 
{
	std::stringstream s;
	s << "{\"type\":\"participants\",\"value\":[";
	std::map<session_ptr,std::string>::iterator it;
	for (it = m_connections.begin(); it != m_connections.end(); it++) 
	{
		s << "\"" << (*it).second << "\"";
		if (++it != m_connections.end()) {
			s << ",";
		}
		it--;
	}
	s << "]}";
	return s.str();
}

// {"type":"msg","sender":"<sender>","value":"<msg>" }
std::string web_server_handler::encode_message(std::string sender,std::string msg,bool escape) 
{
	std::stringstream s;
	// escape JSON characters
	boost::algorithm::replace_all(msg,"\\","\\\\");
	boost::algorithm::replace_all(msg,"\"","\\\"");
	// escape HTML characters
	if (escape) {
		boost::algorithm::replace_all(msg,"&","&amp;");
		boost::algorithm::replace_all(msg,"<","&lt;");
		boost::algorithm::replace_all(msg,">","&gt;");
	}
	s << "{\"type\":\"msg\",\"sender\":\"" << sender<< "\",\"value\":\"" << msg << "\"}";
	return s.str();
}

std::string web_server_handler::get_con_id(session_ptr s) 
{
	std::stringstream endpoint;
	endpoint << s->socket().remote_endpoint();
	return endpoint.str();
}

void web_server_handler::send_to_all(std::string data) 
{
	std::map<session_ptr,std::string>::iterator it;
	for (it = m_connections.begin(); it != m_connections.end(); it++) 
	{
		(*it).first->send(data);
	}
}
