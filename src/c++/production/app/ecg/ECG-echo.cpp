#include <iostream>
#include "SimpleDDS.h"
#include <dds/dds.hpp>
#include "ccpp_ecg.h"

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
         bloodInfo.notice(" ecg Subscriber Started " +deviceid);
	 
	 /*Initializing SimpleDDS library*/
	 SimpleDDS *simpledds;
	 ECGTypeSupport_var typesupport;
    	 DataReader_ptr content_reader;
    	 ECGDataReader_var ecgReader;
    	 ReturnCode_t status;
	 int i=0;
	
	 /*Setting QoS Properties for Topic*/
	 /*DDS::TopicQos tQos;
	tQos.durability.kind=PERSISTENT_DURABILITY_QOS;
	DDS::Duration_t cleanup_delay = {0, 0};
	tQos.durability_service.service_cleanup_delay = cleanup_delay;
	tQos.durability_service.history_depth=1;
	tQos.durability_service.max_samples=1;
	tQos.durability_service.max_instances=1;
	tQos.durability_service.max_samples_per_instance=1;
        tQos.reliability.kind=RELIABLE_RELIABILITY_QOS;
	DDS::Duration_t tempdura = {500, 0};
	tQos.liveliness.kind = DDS::AUTOMATIC_LIVELINESS_QOS;
    	tQos.liveliness.lease_duration = tempdura;
	tQos.lifespan.duration=tempdura;
	tQos.reliability.synchronous=true;
	tQos.history.kind=KEEP_ALL_HISTORY_QOS;
	tQos.history.depth=LENGTH_UNLIMITED;
	tQos.resource_limits.max_samples=LENGTH_UNLIMITED;
	tQos.resource_limits.max_instances=LENGTH_UNLIMITED;
	tQos.resource_limits.max_samples_per_instance=LENGTH_UNLIMITED;*/

	 //getQos(tQos);
         //tQos.durability_service.history_depth= 1024;

	 /*Initializing Subscriber and DataWriter*/
         simpledds = new SimpleDDS();
	 typesupport = new ECGTypeSupport();
    	 
	 /*Creating content Filtered Subscriber*/
	 StringSeq sSeqExpr;
         sSeqExpr.length(0);
	 content_reader = simpledds->filteredSubscribe(typesupport, deviceid ,devid , deviceid,sSeqExpr);
	
	 ecgReader = ECGDataReader::_narrow(content_reader);
   	 ECGSeq  ecgList;
     	 SampleInfoSeq     infoSeq;
	 int m_count=0;
	 bloodInfo.notice("ecg Subscriber For "+deviceid);
	 bloodInfo.notice("Format: DOMAIN, DEVICEID, TIMEOFMEASURED, TimeInSeconds, AmplitudeMillivolts, WaveformPeak, StandardDeviation, Signal");
	 /*Receiving Data from DDS */
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
			prtemp <<ecgList[i].deviceDomain<<COMMA;
		        prtemp <<ecgList[i].deviceID <<COMMA<<ecgList[i].timeOfMeasurement<<COMMA<< ecgList[i].timeInSeconds;
			prtemp <<COMMA<<ecgList[i].amplitudeMillivolts<<COMMA<<ecgList[i].waveformPeak;
			prtemp <<COMMA<<ecgList[i].standardDeviation<<COMMA<<ecgList[i].signalECG;
			bloodEcho.info(prtemp.str().c_str());
			prtemp.str(CLEAN);
			cout << m_count<<"\n";
			m_count++;
			}
	  	}
		status = ecgReader->return_loan(ecgList, infoSeq);
       		checkStatus(status, "return_loan");
       
    	}
        /* We're done.  Delete everything */
	bloodInfo.notice("ecg Subscriber Ends");	
        simpledds->deleteReader(content_reader);
        delete simpledds;
        return 0;


}

