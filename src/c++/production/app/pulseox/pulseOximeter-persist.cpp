#include <iostream>
#include "SimpleDDS.h"
#include <dds/dds.hpp>
#include "ccpp_pulseox.h"

//Visolve Added
#include <mongo/mongo/client/dbclient.h>


// -- BOOST Include
#include <boost/program_options.hpp>

// -- DDS Include
#include <dds/runtime.hpp>
#include <dds/topic.hpp>
#include <dds/reader.hpp>
#include <dds/traits.hpp>
#include "Functions.h"
/*Log4cpp Library*/
#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/SimpleLayout.hh>
#define devid "deviceID"

using namespace mongo;
using namespace DDS;
using namespace std;
namespace po = boost::program_options;
using namespace com::netspective::medigy;

stringstream temp,prtemp;
string domainid,deviceid,loginfo,logdata,logconfpath;

void run(long time,short spo2,short pr,const char *deviceid) 
{
	DBClientConnection c;
	c.connect("localhost");
	BSONObj p = BSONObjBuilder().append("TIMESTAMP",(int)time).append("SPO2",spo2).append("PULSERATE",pr).append("DEVICEID",deviceid).obj();
	c.insert("EMR.PULSEOX", p);
}


int main(int argc, char* argv[]) 
{

	if (!parse_args_sub(argc, argv,domainid,deviceid,loginfo,logdata,logconfpath))
    	return 1;
	
	/*Importing log4cpp configuration and Creating category*/
        log4cpp::Category &log_root = log4cpp::Category::getRoot();
        log4cpp::Category &pulseInfo = log4cpp::Category::getInstance( std::string(loginfo));
        log4cpp::Category &pulsePersist = log4cpp::Category::getInstance( std::string(logdata));
        log4cpp::PropertyConfigurator::configure(logconfpath);
        pulseInfo.notice(" PulseOximeter Persist Subscriber Started");

	/*Initializing SimpleDDS library*/	
	SimpleDDS *simpledds;
	PulseOximeterTypeSupport_var typesupport;
    	DataReader_ptr content_reader;
    	PulseOximeterDataReader_var bpReader;
    	ReturnCode_t status;
	int i=0;

	/*Setting QoS Properties for Topic*/
        DDS::TopicQos tQos;
        tQos.durability.kind=VOLATILE_DURABILITY_QOS;
        tQos.reliability.kind=BEST_EFFORT_RELIABILITY_QOS;
        tQos.history.depth=10;
        tQos.durability_service.history_kind = KEEP_LAST_HISTORY_QOS;
        tQos.durability_service.history_depth= 1024;
        simpledds = new SimpleDDS(tQos);
	typesupport = new PulseOximeterTypeSupport();

	/*Creating content Filtered Subscriber*/
	StringSeq sSeqExpr;
        sSeqExpr.length(0);
	content_reader = simpledds->filteredSubscribe(typesupport, deviceid ,devid , deviceid,sSeqExpr);
    	bpReader = PulseOximeterDataReader::_narrow(content_reader);
   	PulseOximeterSeq  bpList;
     	SampleInfoSeq     infoSeq;
	pulseInfo.notice("pulse Oximeter Persist Subscriber for "+deviceid);
	pulseInfo.notice("Format: DEVICE_ID, MEASURED_TIME, SPO2, PUSLERATE");


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
				try 
				{
					temp<<bpList[i].deviceID;
					prtemp <<bpList[i].deviceID<<", "<<bpList[i].timeOfMeasurement<<", ";
			 		prtemp <<bpList[i].SPO2<<", "<< bpList[i].pulseRatePerMinute;
					pulsePersist.info(prtemp.str().c_str());
					prtemp.str("");
					run(bpList[i].timeOfMeasurement,bpList[i].SPO2,bpList[i].pulseRatePerMinute,temp.str().c_str());
					temp.str("");
				} 
				catch( DBException &e ) 
				{
					temp <<e.what();
					pulseInfo.notice(temp.str());
					temp.str("");
				}
				
			}


		}
		status = bpReader->return_loan(bpList, infoSeq);
        	checkStatus(status, "return_loan");
		
    	}
	pulseInfo.notice("PulseOximeter Persist Subscriber Ends "+deviceid);	
        /* We're done.  Delete everything */
        simpledds->deleteReader(content_reader);
        delete simpledds;
        return 0;
}
