#include <iostream>
#include "SimpleDDS.h"
#include "ccpp_pulseox.h"
/*BOOST Include*/
#include <boost/program_options.hpp>

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
        log4cpp::Category &pulseInfo = log4cpp::Category::getInstance( std::string(loginfo));
        log4cpp::Category &pulseEcho = log4cpp::Category::getInstance( std::string(logdata));
        log4cpp::PropertyConfigurator::configure(logconfpath);
        pulseInfo.notice(" PulseOximeter Subscriber Started");
	
	/*Initializing SimpleDDS library*/
	SimpleDDS *simpledds;
	PulseOximeterTypeSupport_var typesupport;
    	DataReader_ptr content_reader;
    	PulseOximeterDataReader_var bpReader;
    	ReturnCode_t status;
	int i=0;

	/*Setting QoS Properties for Topic*/
	DDS::TopicQos tQos;
	getQos(tQos);

	/*Initializing Subsciber and DataWriter*/
        simpledds = new SimpleDDS(tQos);
	typesupport = new PulseOximeterTypeSupport();
	
	/*Creating content Filtered Subscriber*/
	StringSeq sSeqExpr;
        sSeqExpr.length(0);
	content_reader = simpledds->filteredSubscribe(typesupport, deviceid ,devid , deviceid,sSeqExpr);
    	
    	bpReader = PulseOximeterDataReader::_narrow(content_reader);
   	PulseOximeterSeq  bpList;
     	SampleInfoSeq     infoSeq;
	
	pulseInfo.notice("pulse Oximeter Subscriber for "+deviceid);
	pulseInfo.notice("Format: DOMAIN, DEVICE_ID, MEASURED_TIME, SPO2, PUSLERATE");
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
			
			 prtemp <<bpList[i].deviceDomain<<COMMA<<bpList[i].deviceID<<COMMA<<bpList[i].timeOfMeasurement<<COMMA;
			 prtemp <<bpList[i].SPO2<<COMMA<< bpList[i].pulseRatePerMinute;
			 pulseEcho.info(prtemp.str().c_str());
			 prtemp.str(CLEAN);	
			 
				
			}
			status = bpReader->return_loan(bpList, infoSeq);
        		checkStatus(status, "return_loan");
			
		}
		
	  }

        /* We're done.  Delete everything */
	pulseInfo.notice("PulseOximeter Subscriber Ends");	
        simpledds->deleteReader(content_reader);
        delete simpledds;
        return 0;
}
