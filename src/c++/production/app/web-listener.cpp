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
#include "ccpp_command.h"
#include "ccpp_notify.h"

#define SEMI ":"
#define COMMA ","

using websocketweb::web_server_handler;
using websocketpp::session_ptr;
using namespace DDS;
using namespace std;
using namespace com::netspective::medigy;

web_server_handler::web_server_handler(string dataip,int dataport)
{
	data_gen_ip=dataip;
	data_gen_port=dataport;


}

void web_server_handler::validate(session_ptr client) 
{
	std::stringstream err;

	cout<<"\n DATA-GEN-IP:PORT "<<data_gen_ip<<":"<<data_gen_port<<"\n\n";
	cout<< "\n Client : "<<client <<" Resource :  " <<client->get_resource()<< "Origin  : "<< client->get_origin() <<"\n";
}

void web_server_handler::on_open(session_ptr client) 
{
	std::cout << "client " << client << " joined " << std::endl;
	m_connections.insert(std::pair<session_ptr,std::string>(client,get_con_id(client)));
	send_to_all(serialize_state());
	
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

	
	if(msg.find("DLIST")!=string::npos)
        {
		client->send(encode_message("server",dlist));	
		//send_to_all(encode_message("server",dlist));
	}
	if(nofy_flag!=1)
	{
		nofy_flag=1;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		pthread_create(&notifyid, &attr,startNotifyThread, this);

		
	}
	if(liststart!=1)
	{
	if(msg.find("dynamiclist")!=string::npos)
	{
		cout<<"\n\nIN-Use-Topics Filter Started \n";
		liststart=1;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		pthread_create(&listid, &attr,startTopicsThread, this);

	}
	}
	if(msg.find("START")!=string::npos)
	{
		int start_flag=0;
		for(int px=0;px<=pthread_count;px++)
                {
			if(currentdevice[px].find(msg)!=string::npos) 
			{
				start_flag=1;
				cout<<"\n"<<msg<<"--Device is already running \n"; 
				break;	
			}

                }
		if(start_flag!=1)
		{
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		pthread_create(&pt_ary[pthread_count], &attr, startThread, this);
		stringstream checkdev;
		checkdev<<msg;
		currentdevice[pthread_count]=checkdev.str();
		/*for(int px=0;px<=pthread_count;px++)
		{
		cout<<"\n PTHREAD COUNT : "<<pthread_count <<"     PTHREAD ID : "<<pt_ary[px];
		cout<<"         DEVICE NAME : " <<currentdevice[px];
		}*/
		//cout<<"\ncount :"<<pthread_count<<"\n";
		pthread_count++;
		}
		
		
	}
	else if(msg.find("STOP")!=string::npos)
	{
		/*size_t pos;
		string str2 = msg.substr (msg.rfind(":")); 
		for(int checkn=0;checkn<pthread_count;checkn++)
		{
			
			if(currentdevice[checkn].find(str2)!=string::npos)
			{
				pthread_cancel(pt_ary[checkn]);
				currentdevice[checkn]="";
				cout<<"\n\Stopping Device  "<<currentdevice[checkn]<<"\n\n";
			}
			
		}*/

		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		pthread_create(&stopid, &attr, stopPublisher, this);
		
	}
	else
	{
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		pthread_create(&id, &attr, startThread, this);
	}


}
void *web_server_handler::startNotifyThread(void *arg)
{
			stringstream prtemp;
			SimpleDDS *dev_nofy;
			DeviceNotificationTypeSupport_var nofy_typesupport;
			DataReader_ptr nofy_reader;
			DeviceNotificationDataReader_var nofy_bpReader;
			DeviceNotificationSeq bpList;
			SampleInfoSeq infoSeq;
			dev_nofy = new SimpleDDS();
			nofy_typesupport = new DeviceNotificationTypeSupport();
			ReturnCode_t status;
			nofy_reader = dev_nofy->subscribe(nofy_typesupport);
			nofy_bpReader = DeviceNotificationDataReader::_narrow(nofy_reader);

			while (1)
			{
 				status=nofy_bpReader->take(bpList,infoSeq,LENGTH_UNLIMITED,ANY_SAMPLE_STATE,ANY_VIEW_STATE,ANY_INSTANCE_STATE);
				if (status == RETCODE_NO_DATA) 
				{
          				continue;
          			}
  				for (int i = 0; i < bpList.length(); i++) 
				{
					if(infoSeq[i].valid_data)
					{
						prtemp<<bpList[i].notification<<"  "<<bpList[i].srcDevice<<":"<<bpList[i].srcDomain;
						cout<<"\n\nNotificationTopic - " <<prtemp.str()<<"\n\n";
						prtemp.str("");
					}

					nofy_bpReader->return_loan(bpList, infoSeq);
				}
				
			}
			prtemp.str("");

}



void *web_server_handler::stopPublisher(void *arg)
{

			web_server_handler test = *((web_server_handler*)arg);
			session_ptr client=test.temp_client;
			std::string &msg=test.temp_msg;			
			char buf[1024];
			
			int spawn,flag,sizebuf,port;
			stringstream prtemp,tmp_split;
			tmp_split<<msg;
			strcpy(buf,tmp_split.str().c_str());
			SimpleDDS *simpledds;
			CommandControllerTypeSupport_var typesupport;
			DataWriter_ptr writer;
			CommandControllerDataWriter_var bpWriter;
			/*DDS::TopicQos tQos;
			getQos(tQos);
				tQos.durability.kind=TRANSIENT_DURABILITY_QOS;
         			tQos.reliability.kind=BEST_EFFORT_RELIABILITY_QOS;
         			tQos.history.depth=10;
         			tQos.durability_service.history_kind = KEEP_LAST_HISTORY_QOS;
         			tQos.durability_service.history_depth= 1024;

			simpledds = new SimpleDDS(tQos);*/
			simpledds = new SimpleDDS();
			typesupport = new CommandControllerTypeSupport();
			writer = simpledds->publish(typesupport);
			bpWriter = CommandControllerDataWriter::_narrow(writer);
					
			CommandController data;
			char * pch;
			pch = strtok (buf,":");
			data.command = DDS::string_dup(pch);		
			pch = strtok (NULL, ":");
			data.targetDomain= DDS::string_dup(pch);		
			pch = strtok (NULL, ":");
			data.targetDevice =DDS::string_dup(pch);
			bpWriter->write(data, NULL);

			//simpledds->deleteWriter(writer);
			//delete simpledds;


		
}

void *web_server_handler::startTopicsThread(void *arg)
{
	stringstream topictemp,devicestream;
	web_server_handler test = *((web_server_handler*)arg);
	session_ptr client=test.temp_client;
	DDS::TopicQos tQos;
	getQos(tQos);
	SimpleDDS *mgr = new SimpleDDS(tQos);
	Subscriber_var builtinSubscriber = mgr->participant->get_builtin_subscriber();
	DataReader_var reader = builtinSubscriber->lookup_datareader("DCPSTopic");
	TopicBuiltinTopicDataDataReader_var participantReader = TopicBuiltinTopicDataDataReader::_narrow(reader);
	TopicBuiltinTopicDataSeq data;
	ReturnCode_t status;
	SampleInfoSeq info;
	while(1)
	{
	 	test.name_topics="";
		//status=participantReader->take ( data, info, LENGTH_UNLIMITED,DDS::ANY_SAMPLE_STATE,DDS::ANY_VIEW_STATE,DDS::ANY_INSTANCE_STATE) ;
		status=participantReader->read(data, info, LENGTH_UNLIMITED,DDS::ANY_SAMPLE_STATE,DDS::ANY_VIEW_STATE,DDS::ANY_INSTANCE_STATE);
		if (status == RETCODE_NO_DATA) 
		{
          		continue;
          	}
	        for (int i = 0; i < data.length(); i++)
        	{
        		if(info[i].valid_data)
        	        {
        	        	if((data[i].name[0]!='D')&&(data[i].name[0]!='d'))
        	                {
					
					topictemp<<data[i].name;
					test.name_topics=test.name_topics+";"+topictemp.str();
					topictemp.str("");
					 					
					stringstream bp_prtemp;
					SimpleDDS *bp_simpledds;
					BloodPressureTypeSupport_var bp_typesupport;
    					DataReader_ptr bp_reader;
    					BloodPressureDataReader_var bp_bpReader;
    					ReturnCode_t bp_status;
					int i=0;
					DDS::TopicQos bp_tQos;
					getQos(bp_tQos);
        				bp_tQos.durability_service.history_depth= 1024;
					bp_simpledds = new SimpleDDS(bp_tQos);
					bp_typesupport = new BloodPressureTypeSupport();
    					bp_reader = bp_simpledds->subscribe(bp_typesupport);
    					bp_bpReader = BloodPressureDataReader::_narrow(bp_reader);
   					BloodPressureSeq  bp_bpList;
     					SampleInfoSeq     bp_infoSeq;

					
					stringstream pulse_prtemp;
					SimpleDDS *pulse_simpledds;
					PulseOximeterTypeSupport_var pulse_typesupport;
		    			DataReader_ptr pulse_reader;
		    			PulseOximeterDataReader_var pulse_bpReader;
		    			ReturnCode_t pulse_status;
					DDS::TopicQos pulse_tQos;
					getQos(pulse_tQos);
		        		pulse_tQos.durability_service.history_depth= 1024;
					pulse_simpledds = new SimpleDDS(pulse_tQos);
					pulse_typesupport = new PulseOximeterTypeSupport();
		    			pulse_reader = pulse_simpledds->subscribe(pulse_typesupport);
		    			pulse_bpReader = PulseOximeterDataReader::_narrow(pulse_reader);
		   			PulseOximeterSeq  pulse_bpList;
		     			SampleInfoSeq pulse_infoSeq;
					

					stringstream temp_prtemp;
					 SimpleDDS *temp_simpledds;
					 TemperatureTypeSupport_var temp_typesupport;
	    		 		 DataReader_ptr temp_reader;
				    	 TemperatureDataReader_var temp_bpReader;
				    	 ReturnCode_t temp_status;
					 DDS::TopicQos temp_tQos;
					 getQos(temp_tQos);
				         temp_simpledds = new SimpleDDS(temp_tQos);
					 temp_typesupport = new TemperatureTypeSupport();
					 temp_reader = temp_simpledds->subscribe(temp_typesupport);
			      		 temp_bpReader = TemperatureDataReader::_narrow(temp_reader);
			   	   	 TemperatureSeq  temp_bpList;
				     	 SampleInfoSeq   temp_infoSeq;


					stringstream ecg_prtemp;
					 SimpleDDS *ecg_simpledds;
			 		 ECGTypeSupport_var ecg_typesupport;
				    	 DataReader_ptr ecg_reader;
				    	 ECGDataReader_var ecg_ecgReader;
				    	 ReturnCode_t ecg_status;
			 	         ecg_simpledds = new SimpleDDS();
					 ecg_typesupport = new ECGTypeSupport();
					 ecg_reader = ecg_simpledds->subscribe(ecg_typesupport);
			    		 ecg_ecgReader = ECGDataReader::_narrow(ecg_reader);
			   	   	 ECGSeq  ecg_ecgList;
				     	 SampleInfoSeq     ecg_infoSeq;

					if((test.name_topics.find("BloodPressure"))!=string::npos)
					{	
						int loopexit=0;
						while (loopexit!=100) 
						{
      		 	bp_status = bp_bpReader->take(bp_bpList,bp_infoSeq,LENGTH_UNLIMITED,ANY_SAMPLE_STATE,ANY_VIEW_STATE,ANY_INSTANCE_STATE);
							loopexit++;
        				  		for (i = 0; i < bp_bpList.length(); i++) 
					  		{
								if(bp_infoSeq[i].valid_data)
								{
									bp_prtemp<<bp_bpList[i].deviceID;
									if((test.name_topics.find(bp_prtemp.str()))==string::npos)
									{
										test.name_topics=test.name_topics+","+bp_prtemp.str();
									}
											
									bp_prtemp.str("");
								}
							}
							bp_status =bp_bpReader->return_loan(bp_bpList, bp_infoSeq);
		       					checkStatus(bp_status, "return_loan");
							usleep(99990);
						}
       				
					}	
		
		
				if((test.name_topics.find("PulseOximeter"))!=string::npos)
				{	
					
	
					int loopexit=0;
					while (loopexit!=100) 
					{
	pulse_status = pulse_bpReader->take(pulse_bpList,pulse_infoSeq,LENGTH_UNLIMITED,ANY_SAMPLE_STATE,ANY_VIEW_STATE,ANY_INSTANCE_STATE);
       	  				loopexit++;
          				for (i = 0; i < pulse_bpList.length(); i++) 
	  				{
						if(pulse_infoSeq[i].valid_data)
						{
							pulse_prtemp<<pulse_bpList[i].deviceID;
							if((test.name_topics.find(pulse_prtemp.str()))==string::npos)
							{

								test.name_topics=test.name_topics+","+pulse_prtemp.str();
							}
							
							pulse_prtemp.str("");
							
						}
									
					}		
					pulse_status = pulse_bpReader->return_loan(pulse_bpList, pulse_infoSeq);
	       				checkStatus(pulse_status, "return_loan");
	       				usleep(99990);
			
					}

				}
			if((test.name_topics.find("Temperature"))!=string::npos)
			{

				 int loopexit=0;
				while (loopexit!=1000) 
				{
			         	temp_status = temp_bpReader->take(
        	    			temp_bpList,
        	    			temp_infoSeq,
        	    			LENGTH_UNLIMITED,
			            	ANY_SAMPLE_STATE,
        	   			ANY_VIEW_STATE,
        	    			ANY_INSTANCE_STATE);
			          	loopexit++;
        	  			for (i = 0; i < temp_bpList.length(); i++) 
		  			{
						if(temp_infoSeq[i].valid_data)
						{
							temp_prtemp<<temp_bpList[i].deviceID;
							if((test.name_topics.find(temp_prtemp.str()))==string::npos)
							{
			
								test.name_topics=test.name_topics+","+temp_prtemp.str();
							}
							
						temp_prtemp.str("");
						}
					
					}
					temp_status = temp_bpReader->return_loan(temp_bpList, temp_infoSeq);
       					checkStatus(temp_status, "return_loan");
					usleep(999);
				}
			}
		if((test.name_topics.find("ECG"))!=string::npos)
		{
			 
			 int m_count=0;
	 		 int loopexit=0;
			 while (loopexit!=1000) 
			 {
         			ecg_status = ecg_ecgReader->take(
         		   	ecg_ecgList,
         		   	ecg_infoSeq,
         		   	LENGTH_UNLIMITED,
         		   	ANY_SAMPLE_STATE,
         		  	ANY_VIEW_STATE,
         		   	ANY_INSTANCE_STATE);
        		 
				loopexit++;
         		 	for (i = 0; i < ecg_ecgList.length(); i++) 
	 		 	{
					if(ecg_infoSeq[i].valid_data)
					{
						ecg_prtemp<<ecg_ecgList[i].deviceID;
						if((test.name_topics.find(ecg_prtemp.str()))==string::npos)
						{

						test.name_topics=test.name_topics+","+ecg_prtemp.str();
						}
						
						ecg_prtemp.str("");
					}
						
				}
				ecg_status = ecg_ecgReader->return_loan(ecg_ecgList, ecg_infoSeq);
       				checkStatus(ecg_status, "return_loan");
	  		}
			
       
    		}





           			 }
                        
           		}
        	}
	//status = participantReader->return_loan(data, info);
       	//checkStatus(status, "return_loan");
	topictemp.str("");
	topictemp<<test.name_topics;
	dlist=topictemp.str();
	topictemp.str("");
	cout<<"\n\n LIST : "<<dlist<<"\n";
	}
}

void *web_server_handler::startThread(void *arg)
{
	
	 web_server_handler test = *((web_server_handler*)arg);
	 session_ptr client=test.temp_client;
	 std::string &msg=test.temp_msg;
	 std::cout << "\n\n"<<"Recevied Message :"<<msg<<"\n";
	 char *pchsplit;
         int idev=0;
  	 char strDetails[4][20];
	 pchsplit = strtok ((char*)msg.c_str(),":");
	 while (pchsplit != NULL)
	 {
	        strcpy(strDetails[idev++],pchsplit);
		pchsplit = strtok (NULL, ":");
	 }
	 if (!strcmp(msg.c_str(),"bp"))
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

	if (!strcmp(msg.c_str(),"pulseox"))
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
	if (!strcmp(msg.c_str(),"temp"))
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
	if (!strcmp(msg.c_str(),"ecg"))
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
	if(!strcmp(strDetails[0],"START"))
	{
		cout<<"\nDomain : "<<strDetails[1]<<"   Device : "<<strDetails[2];
		if(!strcmp(strDetails[1],"BP"))
		{
			int socketDescriptor;
			unsigned short int serverPort;
			struct sockaddr_in serverAddress;
			struct hostent *hostInfo;
			char buf[1024], c;
			int spawn,flag,sizebuf,port;
			string domainid,deviceid,loginfo,logdata,logconfpath,hostip;
			stringstream prtemp,cmd_prtemp;
			SimpleDDS *simpledds;
			BloodPressureTypeSupport_var typesupport;
			DataWriter_ptr writer;
			BloodPressureDataWriter_var bpWriter;
			DDS::TopicQos tQos;
			getQos(tQos);
			simpledds = new SimpleDDS(tQos);
			typesupport = new BloodPressureTypeSupport();
			writer = simpledds->publish(typesupport);
			bpWriter = BloodPressureDataWriter::_narrow(writer);
			hostInfo = gethostbyname(data_gen_ip.c_str());
			if (hostInfo == NULL) 
			{
					
				exit(1);
			}
			
			serverPort=data_gen_port;
			cin.get(c); 
			socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
		
			if (socketDescriptor < 0) 
			{
					
				exit(1);
			}
			serverAddress.sin_family = hostInfo->h_addrtype;
			memcpy((char *) &serverAddress.sin_addr.s_addr,hostInfo->h_addr_list[0], hostInfo->h_length);
			serverAddress.sin_port = htons(serverPort);
			if (connect(socketDescriptor,(struct sockaddr *) &serverAddress,sizeof(serverAddress)) < 0) 
			{
			
				exit(1);
			}

			strcpy(buf,"BP");
			if (send(socketDescriptor, buf, strlen(buf) + 1, 0) < 0)
			{
	
				close(socketDescriptor);
				
				exit(1);
			}
		
			flag=0;
			BloodPressure data;
			data.deviceID = DDS::string_dup(strDetails[2]);
			data.deviceDomain = DDS::string_dup("BP");
			SimpleDDS *cmdsub;
			CommandControllerTypeSupport_var cmd_typesupport;
			DataReader_ptr cmd_reader;
			CommandControllerDataReader_var cmd_bpReader;
			cmdsub = new SimpleDDS();
			cmd_typesupport = new CommandControllerTypeSupport();
			cmd_reader = cmdsub->subscribe(cmd_typesupport);
			cmd_bpReader = CommandControllerDataReader::_narrow(cmd_reader);
			CommandControllerSeq  cmd_bpList;
			SampleInfoSeq     cmd_infoSeq;

			/*Nofication Topics Starts*/
			SimpleDDS *dev_nofy;
			DeviceNotificationTypeSupport_var nofy_typesupport;
			DataWriter_ptr nofy_writer;
			DeviceNotificationDataWriter_var nofy_bpWriter;
			DeviceNotification nofy_data;
			dev_nofy = new SimpleDDS();
			nofy_typesupport = new DeviceNotificationTypeSupport();
			nofy_writer = dev_nofy->publish(nofy_typesupport);
			nofy_bpWriter = DeviceNotificationDataWriter::_narrow(nofy_writer);
			nofy_data.notification = DDS::string_dup("Started Device");		
			nofy_data.srcDomain= data.deviceDomain;		
			nofy_data.srcDevice =data.deviceID;
			nofy_bpWriter->write(nofy_data, NULL);
			/*Notification Topic Ends*/
			

			while (1) 
			{
					/*Command Controller Starts*/
					cmd_prtemp.str("");
					cmd_bpReader->take(cmd_bpList,cmd_infoSeq,LENGTH_UNLIMITED,ANY_SAMPLE_STATE,ANY_VIEW_STATE,ANY_INSTANCE_STATE);
				        for (int ci = 0; ci < cmd_bpList.length(); ci++) 
				  	{			
						if(cmd_infoSeq[ci].valid_data)
						{
						cmd_prtemp <<cmd_bpList[ci].command<<","<<cmd_bpList[ci].targetDomain<<","<<cmd_bpList[ci].targetDevice;
							cout<<"\n\nCommand Controller Data : "<<cmd_prtemp.str();
							//cmd_prtemp.str("");
						}
						cmd_bpReader->return_loan(cmd_bpList, cmd_infoSeq);
	  				}
					
					if((sizebuf=recv(socketDescriptor, buf, 50, 0)) > 0)
					{
						buf[sizebuf]='\0';
						char * pch;
						prtemp<<data.deviceID;
						if(cmd_prtemp.str().find(prtemp.str())!=string::npos)
						{
							nofy_data.notification = DDS::string_dup("Stopped Device");		
							nofy_data.srcDomain= data.deviceDomain;		
							nofy_data.srcDevice =data.deviceID;
							nofy_bpWriter->write(nofy_data, NULL);
							
							break;
						}
						prtemp<<","<<data.deviceDomain<<",";
						pch = strtok (buf,":");
						data.timeOfMeasurement = atol(pch);
						prtemp<<data.timeOfMeasurement<<",";
						pch = strtok (NULL, ":");
						data.systolicPressure = (short)atoi(pch);		
						prtemp<<data.systolicPressure<<",";
						pch = strtok (NULL, ":");
						data.diastolicPressure = (short)atoi(pch);
						prtemp<<data.diastolicPressure<<",";
						pch = strtok (NULL, ":");
						data.pulseRatePerMinute = (short)atoi (pch);
						prtemp<<data.pulseRatePerMinute;
						bpWriter->write(data, NULL);
						//cout<<"\n"<<prtemp.str()<<"\n";
						prtemp.str("");
					}
	
		
			}
		
	
			//simpledds->deleteWriter(writer);
			//delete simpledds;
		}
		if(!strcmp(strDetails[1],"PULSEOX"))
		{
			int socketDescriptor;
			unsigned short int serverPort;
			struct sockaddr_in serverAddress;
			struct hostent *hostInfo;
			char buf[1024], c;
			int sizebuf,port;
			string domainid,deviceid,loginfo,logdata,logconfpath,hostip;
			stringstream prtemp,cmd_prtemp;
			SimpleDDS *simpledds;
			PulseOximeterTypeSupport_var typesupport;
			DataWriter_ptr writer;
			PulseOximeterDataWriter_var bpWriter;
			DDS::TopicQos tQos;
			getQos(tQos);

			simpledds = new SimpleDDS(tQos);
			typesupport = new PulseOximeterTypeSupport();
			writer = simpledds->publish(typesupport);
			bpWriter = PulseOximeterDataWriter::_narrow(writer);
			hostInfo=gethostbyname(data_gen_ip.c_str());
	
			if (hostInfo == NULL) 
			{	
				
			}
			serverPort=data_gen_port;
			cin.get(c); 
			socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
			if (socketDescriptor < 0) 
			{
				
				exit(1);
			}
			serverAddress.sin_family = hostInfo->h_addrtype;
			memcpy((char *) &serverAddress.sin_addr.s_addr,	hostInfo->h_addr_list[0], hostInfo->h_length);
			serverAddress.sin_port = htons(serverPort);

			if (connect(socketDescriptor,(struct sockaddr *) &serverAddress,sizeof(serverAddress)) < 0) 
			{
				exit(1);
			}

			strcpy(buf,"PULSEOX");
			if (send(socketDescriptor, buf, strlen(buf) + 1, 0) < 0)
			{
		
				close(socketDescriptor);
				exit(1);
			}

			PulseOximeter data;

		
			data.deviceID = DDS::string_dup(strDetails[2]);
			data.deviceDomain = DDS::string_dup(strDetails[1]);
			SimpleDDS *cmdsub;
			CommandControllerTypeSupport_var cmd_typesupport;
			DataReader_ptr cmd_reader;
			CommandControllerDataReader_var cmd_bpReader;
			cmdsub = new SimpleDDS();
			cmd_typesupport = new CommandControllerTypeSupport();
			cmd_reader = cmdsub->subscribe(cmd_typesupport);
			cmd_bpReader = CommandControllerDataReader::_narrow(cmd_reader);
			CommandControllerSeq  cmd_bpList;
			SampleInfoSeq     cmd_infoSeq;

			/*Nofication Topics Starts*/
			SimpleDDS *dev_nofy;
			DeviceNotificationTypeSupport_var nofy_typesupport;
			DataWriter_ptr nofy_writer;
			DeviceNotificationDataWriter_var nofy_bpWriter;
			DeviceNotification nofy_data;
			dev_nofy = new SimpleDDS();
			nofy_typesupport = new DeviceNotificationTypeSupport();
			nofy_writer = dev_nofy->publish(nofy_typesupport);
			nofy_bpWriter = DeviceNotificationDataWriter::_narrow(nofy_writer);
			nofy_data.notification = DDS::string_dup("Started Device");		
			nofy_data.srcDomain= data.deviceDomain;		
			nofy_data.srcDevice =data.deviceID;
			nofy_bpWriter->write(nofy_data, NULL);
			/*Notification Topic Ends*/

			while (1) 
			{
				/*Command Controller Starts*/
				cmd_prtemp.str("");
				cmd_bpReader->take(cmd_bpList,cmd_infoSeq,LENGTH_UNLIMITED,ANY_SAMPLE_STATE,ANY_VIEW_STATE,ANY_INSTANCE_STATE);
			        for (int ci = 0; ci < cmd_bpList.length(); ci++) 
			  	{			
					if(cmd_infoSeq[ci].valid_data)
					{
						cmd_prtemp <<cmd_bpList[ci].command<<","<<cmd_bpList[ci].targetDomain<<","<<cmd_bpList[ci].targetDevice;
						cout<<"\n\nCommand Controller Data : "<<cmd_prtemp.str();
						}
						cmd_bpReader->return_loan(cmd_bpList, cmd_infoSeq);
	  				}
				/*Command Controller Ends*/

				if ((sizebuf=recv(socketDescriptor, buf, 50, 0)) > 0) 
				{
					buf[sizebuf]='\0';
					char * pch;

					prtemp<<data.deviceID;
					if(cmd_prtemp.str().find(prtemp.str())!=string::npos)
					{
						nofy_data.notification = DDS::string_dup("Stopped Device");		
						nofy_data.srcDomain= data.deviceDomain;		
						nofy_data.srcDevice =data.deviceID;
						nofy_bpWriter->write(nofy_data, NULL);
						break;
					}
					prtemp<<","<<data.deviceDomain<<",";


					pch = strtok (buf,":");
					data.timeOfMeasurement = atol(pch);
					prtemp<<data.timeOfMeasurement<<",";
					pch = strtok (NULL,":");
					data.SPO2 = (short)atoi(pch);		
					prtemp<<data.SPO2<<",";
					pch = strtok (NULL,":");
					data.pulseRatePerMinute = (short)atoi (pch);
					prtemp<<data.pulseRatePerMinute;
					bpWriter->write(data, NULL);
					prtemp.str("");
				}
			}



			//simpledds->deleteWriter(writer);
			//delete simpledds;
		}

		if(!strcmp(strDetails[1],"TEMP"))
		{
			int socketDescriptor;
			unsigned short int serverPort;
			struct sockaddr_in serverAddress;
			struct hostent *hostInfo;
			char buf[1024], c;
			int sizebuf,port;
			string domainid,deviceid,loginfo,logdata,logconfpath,hostip;
			stringstream prtemp,cmd_prtemp;
		
			SimpleDDS *simpledds;
			TemperatureTypeSupport_var typesupport;
			DataWriter_ptr writer;
			TemperatureDataWriter_var bpWriter;

			DDS::TopicQos tQos;
			getQos(tQos);
			simpledds = new SimpleDDS(tQos);
			typesupport = new TemperatureTypeSupport();
			writer = simpledds->publish(typesupport);
			bpWriter = TemperatureDataWriter::_narrow(writer);
			hostInfo = gethostbyname(data_gen_ip.c_str());
	
			if (hostInfo == NULL) 
			{
				exit(1);
			}
			
			serverPort=data_gen_port;
			cin.get(c); 
			socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
			if (socketDescriptor < 0) 
			{
				exit(1);
			}			
			serverAddress.sin_family = hostInfo->h_addrtype;
			memcpy((char *) &serverAddress.sin_addr.s_addr,hostInfo->h_addr_list[0], hostInfo->h_length);
			serverAddress.sin_port = htons(serverPort);

			if (connect(socketDescriptor,(struct sockaddr *) &serverAddress,sizeof(serverAddress)) < 0) 
			{
				exit(1);
			}
	
			strcpy(buf,"TEMPERATURE");
			if (send(socketDescriptor, buf, strlen(buf) + 1, 0) < 0)
			{
				close(socketDescriptor);
				exit(1);
			}
			Temperature data;
			
			data.deviceID = DDS::string_dup(strDetails[2]);
			data.deviceDomain = DDS::string_dup(strDetails[1]);

			SimpleDDS *cmdsub;
			CommandControllerTypeSupport_var cmd_typesupport;
			DataReader_ptr cmd_reader;
			CommandControllerDataReader_var cmd_bpReader;
			cmdsub = new SimpleDDS();
			cmd_typesupport = new CommandControllerTypeSupport();
			cmd_reader = cmdsub->subscribe(cmd_typesupport);
			cmd_bpReader = CommandControllerDataReader::_narrow(cmd_reader);
			CommandControllerSeq  cmd_bpList;
			SampleInfoSeq     cmd_infoSeq;

			/*Nofication Topics Starts*/
			SimpleDDS *dev_nofy;
			DeviceNotificationTypeSupport_var nofy_typesupport;
			DataWriter_ptr nofy_writer;
			DeviceNotificationDataWriter_var nofy_bpWriter;
			DeviceNotification nofy_data;
			dev_nofy = new SimpleDDS();
			nofy_typesupport = new DeviceNotificationTypeSupport();
			nofy_writer = dev_nofy->publish(nofy_typesupport);
			nofy_bpWriter = DeviceNotificationDataWriter::_narrow(nofy_writer);
			nofy_data.notification = DDS::string_dup("Started Device");		
			nofy_data.srcDomain= data.deviceDomain;		
			nofy_data.srcDevice =data.deviceID;
			nofy_bpWriter->write(nofy_data, NULL);
			/*Notification Topic Ends*/
			
			while (1) 
			{
				/*Command Controller Starts*/
				cmd_prtemp.str("");
				cmd_bpReader->take(cmd_bpList,cmd_infoSeq,LENGTH_UNLIMITED,ANY_SAMPLE_STATE,ANY_VIEW_STATE,ANY_INSTANCE_STATE);
			        for (int ci = 0; ci < cmd_bpList.length(); ci++) 
			  	{			
					if(cmd_infoSeq[ci].valid_data)
					{
						cmd_prtemp <<cmd_bpList[ci].command<<","<<cmd_bpList[ci].targetDomain<<","<<cmd_bpList[ci].targetDevice;
						cout<<"\n\nCommand Controller Data : "<<cmd_prtemp.str();
						}
						cmd_bpReader->return_loan(cmd_bpList, cmd_infoSeq);
	  				}
				/*Command Controller Ends*/
				
				
				if ((sizebuf=recv(socketDescriptor, buf, 50, 0)) > 0) 
				{
				buf[sizebuf] = '\0';
				char * pch;

				prtemp<<data.deviceID;
				if(cmd_prtemp.str().find(prtemp.str())!=string::npos)
				{
					nofy_data.notification = DDS::string_dup("Stopped Device");		
					nofy_data.srcDomain= data.deviceDomain;		
					nofy_data.srcDevice =data.deviceID;
					nofy_bpWriter->write(nofy_data, NULL);
					break;
				}
				prtemp<<","<<data.deviceDomain<<",";

				pch = strtok (buf,":");
				data.timeOfMeasurement = atol(pch);
				prtemp<<data.timeOfMeasurement<<",";
				pch = strtok (NULL, ":");
				data.temp = (short)atoi(pch);
				prtemp<<data.temp;
				bpWriter->write(data, NULL);
				prtemp.str("");
				}
			}
	
		
			//simpledds->deleteWriter(writer);
			//delete simpledds;
		}

		if(!strcmp(strDetails[1],"ECG"))
		{
			int socketDescriptor;
			unsigned short int serverPort;
			struct sockaddr_in serverAddress;
			struct hostent *hostInfo;
			char buf[1024], c;
			int spawn,flag,sizebuf,port,heartbeats,ecgsample,internalsample;
			float amplitudenoise,heart_rate_mean,heart_rate_std,lowfreq,highfreq,lowfreqstd,highfreqstd,lfhfradio;
			string domainid,deviceid,loginfo,logdata,logconfpath,hostip;
			stringstream prtemp,datacommand,cmd_prtemp;
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

			SimpleDDS *simpledds;
			ECGTypeSupport_var typesupport;
			DataWriter_ptr writer;
			ECGDataWriter_var ecgWriter;
			simpledds = new SimpleDDS();
			typesupport = new ECGTypeSupport();
			writer = simpledds->publish(typesupport);
			ecgWriter = ECGDataWriter::_narrow(writer);
			hostInfo = gethostbyname(data_gen_ip.c_str());
			serverPort=data_gen_port;
			cin.get(c); 
			socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
			serverAddress.sin_family = hostInfo->h_addrtype;
			memcpy((char *) &serverAddress.sin_addr.s_addr,hostInfo->h_addr_list[0], hostInfo->h_length);
			serverAddress.sin_port = htons(serverPort);
			if (connect(socketDescriptor,(struct sockaddr *) &serverAddress,sizeof(serverAddress)) < 0) 
			{
			}

	datacommand<<"ECG"<<SEMI<<heartbeats<<SEMI<<ecgsample<<SEMI<<internalsample<<SEMI<<amplitudenoise;
	datacommand<<SEMI<<heart_rate_mean<<SEMI<<heart_rate_std<<SEMI<<lowfreq<<SEMI<<highfreq<<SEMI<<lowfreqstd<<SEMI<<highfreqstd<<SEMI<<lfhfradio;
			strcpy(buf,datacommand.str().c_str());
			if (send(socketDescriptor, buf, strlen(buf) + 1, 0) < 0)
			{
				close(socketDescriptor);
			}
	
			flag=0;
			ECG data;

			
			//data.deviceID = DDS::string_dup(strDetails[1]);
			data.deviceID = DDS::string_dup("ECG_LAB44");
			data.deviceDomain = DDS::string_dup(strDetails[2]);

			SimpleDDS *cmdsub;
			CommandControllerTypeSupport_var cmd_typesupport;
			DataReader_ptr cmd_reader;
			CommandControllerDataReader_var cmd_bpReader;
			cmdsub = new SimpleDDS();
			cmd_typesupport = new CommandControllerTypeSupport();
			cmd_reader = cmdsub->subscribe(cmd_typesupport);
			cmd_bpReader = CommandControllerDataReader::_narrow(cmd_reader);
			CommandControllerSeq  cmd_bpList;
			SampleInfoSeq     cmd_infoSeq;
			long count=0;

			/*Nofication Topics Starts*/
			SimpleDDS *dev_nofy;
			DeviceNotificationTypeSupport_var nofy_typesupport;
			DataWriter_ptr nofy_writer;
			DeviceNotificationDataWriter_var nofy_bpWriter;
			DeviceNotification nofy_data;
			dev_nofy = new SimpleDDS();
			nofy_typesupport = new DeviceNotificationTypeSupport();
			nofy_writer = dev_nofy->publish(nofy_typesupport);
			nofy_bpWriter = DeviceNotificationDataWriter::_narrow(nofy_writer);
			nofy_data.notification = DDS::string_dup("Started Device");		
			nofy_data.srcDomain= data.deviceDomain;		
			nofy_data.srcDevice =data.deviceID;
			nofy_bpWriter->write(nofy_data, NULL);
			/*Notification Topic Ends*/

			while (1) 
			{
				
				/*Command Controller Starts*/
				cmd_prtemp.str("");
				cmd_bpReader->take(cmd_bpList,cmd_infoSeq,LENGTH_UNLIMITED,ANY_SAMPLE_STATE,ANY_VIEW_STATE,ANY_INSTANCE_STATE);
			        for (int ci = 0; ci < cmd_bpList.length(); ci++) 
			  	{			
					if(cmd_infoSeq[ci].valid_data)
					{
						cmd_prtemp <<cmd_bpList[ci].command<<","<<cmd_bpList[ci].targetDomain<<","<<cmd_bpList[ci].targetDevice;
						cout<<"\n\nCommand Controller Data : "<<cmd_prtemp.str();
						}
						cmd_bpReader->return_loan(cmd_bpList, cmd_infoSeq);
	  				}
				/*Command Controller Ends*/
			
				if ((sizebuf=recv(socketDescriptor, buf, 1024,  MSG_NOSIGNAL)) > 0) 
				{
					prtemp<<data.deviceID;
					if(cmd_prtemp.str().find(prtemp.str())!=string::npos)
					{
						nofy_data.notification = DDS::string_dup("Stopped Device");		
						nofy_data.srcDomain= data.deviceDomain;		
						nofy_data.srcDevice =data.deviceID;
						nofy_bpWriter->write(nofy_data, NULL);
						break;
					}
					prtemp<<","<<data.deviceDomain<<",";

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
		

					if (RETCODE_OUT_OF_RESOURCES  == ecgWriter->write(data, NULL))
					{ cout << "\n Out of Resource ";}
					int newflag=-1;
					while(newflag <= 0)
					{
							newflag=send(socketDescriptor,prtemp.str().c_str(),1024,  MSG_NOSIGNAL);
						
							
					}
					prtemp.str("");
					fflush(stdout);
						
					}
		
		
	

				}


			//simpledds->deleteWriter(writer);
			//delete simpledds;
		}
		
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
