#include <iostream>
#include "SimpleDDS.h"
#include <dds/dds.hpp>
#include "ccpp_bp.h"

//Visolve Added
#include <mongo/mongo/client/dbclient.h>

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

using namespace mongo;
using namespace std;
namespace po = boost::program_options;
using namespace com::netspective::medigy;

std::stringstream temp,prtemp;
string domainid,deviceid,loginfo,logdata,logconfpath;

void run(long time,short bp,short lbp,short pr,const char *deviceid) 
{
	DBClientConnection c;
	c.connect("localhost");
	BSONObj p = BSONObjBuilder().append("TIMESTAMP",(int)time).append("SYSTOLIC",bp).append("DIASTOLIC",lbp).append("PULSERATE",pr).append("DEVICEID",deviceid).obj();
	c.insert("EMR.BLOOD", p);

}


int main(int argc, char* argv[]) 
{
	 if (!parse_args_sub(argc, argv,domainid,deviceid,loginfo,logdata,logconfpath))
    	 return 1;	

	 /*Importing log4cpp configuration and Creating category*/
	 log4cpp::Category &log_root = log4cpp::Category::getRoot();
         log4cpp::Category &bloodInfo = log4cpp::Category::getInstance( std::string(loginfo));
         log4cpp::Category &bloodPersist = log4cpp::Category::getInstance( std::string(logdata));
         log4cpp::PropertyConfigurator::configure(logconfpath);
         bloodInfo.notice(" Blood Pressure Persist Subscriber Started " +deviceid);


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
         simpledds = new SimpleDDS(tQos);
	 typesupport = new BloodPressureTypeSupport();
	
	 /*Creating content Filtered Subscriber*/
	 StringSeq sSeqExpr;
         sSeqExpr.length(0);
	 content_reader = simpledds->filteredSubscribe(typesupport, deviceid ,devid , deviceid,sSeqExpr);

    	 bpReader = BloodPressureDataReader::_narrow(content_reader);
   	 BloodPressureSeq  bpList;
     	 SampleInfoSeq     infoSeq;

	 bloodInfo.notice("Blood Pressure Persist Subscriber for "+deviceid);
	 bloodInfo.notice("MONGODB Format: DEVICE_ID, MEASURED_TIME, SYSTOLIC, DIATOLIC, PULSERATE");

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
			run(bpList[i].timeOfMeasurement,bpList[i].systolicPressure,bpList[i].diastolicPressure,bpList[i].pulseRatePerMinute,temp.str().c_str());
			prtemp <<bpList[i].deviceID <<", "<<bpList[i].timeOfMeasurement<<", "<< bpList[i].systolicPressure;
			prtemp <<", "<<bpList[i].diastolicPressure<<", "<<bpList[i].pulseRatePerMinute;
			bloodPersist.info(prtemp.str().c_str());
			prtemp.str("");
				} 
				catch( DBException &e ) 
				{
				temp <<e.what();
				bloodInfo.notice(temp.str());
				}
				
				temp.str("");
			}
		}
		status = bpReader->return_loan(bpList, infoSeq);
       		checkStatus(status, "return_loan");
	}
	bloodInfo.notice("Blood Pressure Persist Subscriber Ends "+deviceid);	
	simpledds->deleteReader(content_reader);
        delete simpledds;
        return 0;
}
