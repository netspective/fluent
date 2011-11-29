#include "SimpleDDS.h"
#include <iostream>
#include <dds/dds.hpp>
#include "ccpp_bp.h"

// -- BOOST Include
#include <boost/program_options.hpp>

// -- DDS Include
#include <dds/runtime.hpp>
#include <dds/topic.hpp>
#include <dds/reader.hpp>
#include <dds/traits.hpp>
#define devid "deviceID"

/*Log4cpp Library*/
#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/SimpleLayout.hh>
#include "Functions.h"

using namespace std;
namespace po = boost::program_options;
using namespace com::netspective::medigy;
int sysmin,sysmax,pulsemin,pulsemax,dismin,dismax;

std::stringstream temp,prtemp;
string domainid,deviceid,loginfo,logdata,logconfpath;

int main(int argc, char* argv[]) 
{
	 sysmin = 90;
	 sysmax = 140;
	 dismin = 60;
	 dismax = 90;
	 pulsemin = 60;
	 pulsemax = 90;
	 if (!parse_args_bp_alarm(argc, argv,domainid,deviceid,sysmin,sysmax,dismin,dismax,pulsemin,pulsemax,loginfo,logdata,logconfpath))
    	 return 1;

	 /*Importing log4cpp configuration and Creating category*/
	 log4cpp::Category &log_root = log4cpp::Category::getRoot();
         log4cpp::Category &bloodInfo = log4cpp::Category::getInstance( std::string(loginfo));
         log4cpp::Category &bloodAlarm = log4cpp::Category::getInstance( std::string(logdata));
         log4cpp::PropertyConfigurator::configure(logconfpath);
         bloodInfo.notice(" Blood Pressure Alarm Subscriber Started " +deviceid);

	 /*Initializing SimpleDDS library*/
	 SimpleDDS *simpledds;
	 BloodPressureTypeSupport_var typesupport;
    	 DataReader_ptr content_reader;
    	 BloodPressureDataReader_var bpReader;
    	 ReturnCode_t status;
	 int i=0;
         
	 /*Setting QoS Properties for Topic*/
	 DDS::TopicQos tQos;
	 getQos(tQos);

         simpledds = new SimpleDDS(tQos);
	 typesupport = new BloodPressureTypeSupport();

	 /*Creating content Filtered Subscriber*/
	 StringSeq sSeqExpr;
         sSeqExpr.length(0);
	 content_reader = simpledds->filteredSubscribe(typesupport, deviceid ,devid , deviceid,sSeqExpr);

    	 bpReader = BloodPressureDataReader::_narrow(content_reader);
   	 BloodPressureSeq  bpList;
     	 SampleInfoSeq     infoSeq;

	 bloodInfo.notice("Blood Pressure alarm Subscriber for "+deviceid);
	 bloodInfo.notice("Format: DOMAIN_ID, DEVICE_ID, MEASURED_TIME, SYSTOLIC(LEVEL), DIASTOLIC(LEVEL), PULSERATE(LEVEL)");
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
			if (bpList[i].systolicPressure < sysmin || bpList[i].systolicPressure > sysmax || bpList[i].diastolicPressure < dismin || bpList[i].diastolicPressure > dismax || bpList[i].pulseRatePerMinute < pulsemin || bpList[i].pulseRatePerMinute > pulsemax)
			{
				prtemp <<bpList[i].deviceDomain<<COMMA<<bpList[i].deviceID<<COMMA;
				prtemp <<bpList[i].timeOfMeasurement<<COMMA<<alarmString(bpList[i].systolicPressure,sysmin,sysmax);
				prtemp <<COMMA<<alarmString(bpList[i].diastolicPressure,dismin,dismax)<<COMMA;
				prtemp <<alarmString(bpList[i].pulseRatePerMinute,pulsemin,pulsemax);
				bloodAlarm.info(prtemp.str().c_str());
				prtemp.str(CLEAN);
			}
			
			}
		}
		status = bpReader->return_loan(bpList, infoSeq);
        	checkStatus(status, "return_loan");
	 	
    	}
	bloodInfo.notice("Blood Pressure alarm Subscriber Ends");	
        /* We're done.  Delete everything */
        simpledds->deleteReader(content_reader);
        delete simpledds;
        return 0;
}
