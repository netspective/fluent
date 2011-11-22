#include <iostream>
#include "SimpleDDS.h"
#include <dds/dds.hpp>
#include "ccpp_bp.h"

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
         log4cpp::Category &bloodInfo = log4cpp::Category::getInstance( std::string(loginfo));
         log4cpp::Category &bloodEcho = log4cpp::Category::getInstance( std::string(logdata));
         log4cpp::PropertyConfigurator::configure(logconfpath);
         bloodInfo.notice(" Blood Pressure Subscriber Started " +deviceid);
	 
	 /*Initializing SimpleDDS library*/
	 SimpleDDS *simpledds;
	 BloodPressureTypeSupport_var typesupport;
    	 DataReader_ptr content_reader;
    	 BloodPressureDataReader_var bpReader;
    	 ReturnCode_t status;
	 int i=0;
	
	 /*Setting QoS Properties for Topic*/
         DDS::TopicQos tQos;
         tQos.durability.kind=VOLATILE_DURABILITY_QOS;
         tQos.reliability.kind=BEST_EFFORT_RELIABILITY_QOS;
         tQos.history.depth=10;
         tQos.durability_service.history_kind = KEEP_LAST_HISTORY_QOS;
         tQos.durability_service.history_depth= 1024;

	 /*Initializing Subscriber and DataWriter*/
         simpledds = new SimpleDDS(tQos);
	 typesupport = new BloodPressureTypeSupport();
    	 
	 /*Creating content Filtered Subscriber*/
	 StringSeq sSeqExpr;
         sSeqExpr.length(0);
	 content_reader = simpledds->filteredSubscribe(typesupport, deviceid ,devid , deviceid,sSeqExpr);
	
	 bpReader = BloodPressureDataReader::_narrow(content_reader);
   	 BloodPressureSeq  bpList;
     	 SampleInfoSeq     infoSeq;
	 
	 bloodInfo.notice("Blood Pressure Subscriber for "+deviceid);
	 bloodInfo.notice("Format: DEVICE_ID, MEASURED_TIME, SYSTOLIC, DIATOLIC, PULSERATE");
	 /*Receiving Data from DDS */
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
		        prtemp <<bpList[i].deviceID <<", "<<bpList[i].timeOfMeasurement<<", "<< bpList[i].systolicPressure;
			prtemp <<", "<<bpList[i].diastolicPressure<<", "<<bpList[i].pulseRatePerMinute;
			bloodEcho.info(prtemp.str().c_str());
			prtemp.str("");
			}
	  	}
		status = bpReader->return_loan(bpList, infoSeq);
       		checkStatus(status, "return_loan");
       
    	}

        /* We're done.  Delete everything */
	bloodInfo.notice("Blood Pressure Subscriber Ends");	
        simpledds->deleteReader(content_reader);
        delete simpledds;
        return 0;


}

