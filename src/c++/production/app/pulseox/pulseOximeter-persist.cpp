#include <iostream>
#include "SimpleDDS.h"

#include "ccpp_pulseox.h"

//Visolve Added
#include <dbclient.h>


// -- BOOST Include
#include <boost/program_options.hpp>

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
string domainid,deviceid,loginfo,logdata,logconfpath,host,database,tablename;

void run(long time,short spo2,short pr,const char *deviceid) 
{
	DBClientConnection c;
	c.connect(host);
	BSONObj p = BSONObjBuilder().append(TIMESTAMP,(int)time).append(SPo2,spo2).append(PULSERATE,pr).append(DEVICEID,deviceid).obj();
	c.insert(tablename, p);
}


int main(int argc, char* argv[]) 
{

	if (!parse_args_sub_persist(argc, argv,domainid,deviceid,loginfo,logdata,logconfpath,host,database))
    	return 1;
	tablename = database+"."+domainid;
	
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
	getQos(tQos);

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
					prtemp <<bpList[i].deviceID<<COMMA<<bpList[i].timeOfMeasurement<<COMMA;
			 		prtemp <<bpList[i].SPO2<<COMMA<< bpList[i].pulseRatePerMinute;
					pulsePersist.info(prtemp.str().c_str());
					prtemp.str(CLEAN);
					run(bpList[i].timeOfMeasurement,bpList[i].SPO2,bpList[i].pulseRatePerMinute,temp.str().c_str());
					temp.str(CLEAN);
				} 
				catch( DBException &e ) 
				{
					temp <<e.what();
					pulseInfo.notice(temp.str());
					temp.str(CLEAN);
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
