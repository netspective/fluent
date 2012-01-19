#include <iostream>
#include "SimpleDDS.h"
#include <dds/dds.hpp>
#include "ccpp_EbD_Sensor.h"

/* BOOST Library*/
#include <boost/program_options.hpp>

/* DDS Library*/
#include <dds/runtime.hpp>
#include <dds/topic.hpp>
#include <dds/reader.hpp>
#include <stdio.h>
#include <dds/traits.hpp>
#include "Functions.h"

/*Log4cpp Library*/
#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/SimpleLayout.hh>
#define devid "deviceID"

using namespace DDS;
using namespace std;
namespace po = boost::program_options;
using namespace com::netspective::medigy;

std::stringstream temp,prtemp;
string domainid,deviceid,loginfo,logdata,logconfpath;

int main(int argc, char* argv[]) 
{

	 if (!parse_args_sub(argc, argv,domainid,deviceid,loginfo,logdata,logconfpath))
    	 return 1;
	 
	 /*Importing log4cpp configuration and Creating category*/
	 log4cpp::Category &log_root = log4cpp::Category::getRoot();
         log4cpp::Category &sensorInfo = log4cpp::Category::getInstance( std::string(loginfo));
         log4cpp::Category &sensorEcho = log4cpp::Category::getInstance( std::string(logdata));
         log4cpp::PropertyConfigurator::configure(logconfpath);
         sensorInfo.notice(" EbD_Sensor Subscriber Started " +deviceid);
	 
	 /*Initializing SimpleDDS library*/
	 SimpleDDS *simpledds;
	 EbD_SensorTypeSupport_var typesupport;
    	 DataReader_ptr content_reader;
    	 EbD_SensorDataReader_var EbD_SensorReader;
    	 ReturnCode_t status;
	 int i=0;
	
	/*Initializing Subscriber and DataWriter*/
         simpledds = new SimpleDDS();
	 typesupport = new EbD_SensorTypeSupport();
    	 
	 /*Creating content Filtered Subscriber*/
	 StringSeq sSeqExpr;
         sSeqExpr.length(0);
	 content_reader = simpledds->filteredSubscribe(typesupport, deviceid ,devid , deviceid,sSeqExpr);
	
	 EbD_SensorReader = EbD_SensorDataReader::_narrow(content_reader);
   	 EbD_SensorSeq  EbD_SensorList;
     	 SampleInfoSeq     infoSeq;
	 
	 sensorInfo.notice("EbD_Sensor Subscriber For "+deviceid);
	 sensorInfo.notice("Format: DOMAIN_ID, DEVICE_ID, MEASURED_TIME, SYSTOLIC, DIASTOLIC, PULSERATE");
	 
	/*Receiving Data from DDS */
	 while (1) 
	 {
         	status = EbD_SensorReader->take(
            	EbD_SensorList,
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
          	for (i = 0; i < EbD_SensorList.length(); i++) 
	  	{
			if(infoSeq[i].valid_data)
			{
			prtemp <<EbD_SensorList[i].deviceDomain<<COMMA;
		        prtemp <<EbD_SensorList[i].deviceID <<COMMA<<EbD_SensorList[i].timeOfMeasurement<<COMMA;
			prtemp << EbD_SensorList[i].timeStamp<<COMMA<<EbD_SensorList[i].pAvgLoad<<COMMA<<EbD_SensorList[i].iRMS;
			prtemp <<COMMA<<EbD_SensorList[i].vRMS<<COMMA<<EbD_SensorList[i].zLoad;
			cout << prtemp.str().c_str()<<"\n";
			sensorEcho.info(prtemp.str().c_str());
			prtemp.str(CLEAN);
			}
	  	}
		status = EbD_SensorReader->return_loan(EbD_SensorList, infoSeq);
       		checkStatus(status, "return_loan");
       
    	}

        sensorInfo.notice("EbD_Sensor Subscriber Ends");	
        simpledds->deleteReader(content_reader);
        delete simpledds;
        return 0;


}

