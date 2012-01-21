#include <iostream>
#include "SimpleDDS.h"
#include <dds/dds.hpp>
#include "ccpp_temperature.h""
//Visolve Added
#include <mongo/client/dbclient.h>
// -- BOOST Include
#include <boost/program_options.hpp>
// -- DDS Include
#include <dds/runtime.hpp>
#include <dds/topic.hpp>
#include <dds/reader.hpp>
#include <dds/traits.hpp>
#include "Functions.h"
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
std::stringstream temp,prtemp;
string domainid,deviceid,loginfo,logdata,logconfpath,host,database,tablename;

void run(long time,short utemp,const char *deviceid) 
{
	DBClientConnection c;
	c.connect(host);
	BSONObj p = BSONObjBuilder().append(TIMESTAMP,(int)time).append(TEMPERATURE,utemp).append(DEVICEID,deviceid).obj();
	c.insert(tablename, p);
}

 

int main(int argc, char* argv[]) 
{
	 if (!parse_args_sub_persist(argc, argv,domainid,deviceid,loginfo,logdata,logconfpath,host,database))
    	 return 1;
	 tablename = database+CLEAN+domainid;
	
	 /*Importing log4cpp configuration and Creating category*/
         log4cpp::Category &log_root = log4cpp::Category::getRoot();
         log4cpp::Category &tempInfo = log4cpp::Category::getInstance( std::string(loginfo));
         log4cpp::Category &tempPersist = log4cpp::Category::getInstance( std::string(logdata));
         log4cpp::PropertyConfigurator::configure(logconfpath);
         tempInfo.notice(" Temperature Persist Subscriber Started");

	 /*Initializing SimpleDDS library*/
	 SimpleDDS *simpledds;
	 TemperatureTypeSupport_var typesupport;
    	 DataReader_ptr content_reader;
    	 TemperatureDataReader_var bpReader;
    	 ReturnCode_t status;
	 int i=0;

	 /*Setting QoS Properties for Topic*/
	 DDS::TopicQos tQos;
	 getQos(tQos);

         simpledds = new SimpleDDS(tQos);
	 typesupport = new TemperatureTypeSupport();

	 /*Creating content Filtered Subscriber*/
	 StringSeq sSeqExpr;
         sSeqExpr.length(0);
	 content_reader = simpledds->filteredSubscribe(typesupport, deviceid ,devid , deviceid,sSeqExpr);
    	 bpReader = TemperatureDataReader::_narrow(content_reader);
   	 TemperatureSeq  bpList;
     	 SampleInfoSeq     infoSeq;
	 tempInfo.notice("Temperature Persist Subscriber for "+deviceid);
	 tempInfo.notice("Format: DEVICE_ID, MEASURED_TIME, TEMPERATURE");

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
				try 
				{
					temp<<bpList[i].deviceID;
					prtemp <<bpList[i].deviceID<<COMMA<<bpList[i].timeOfMeasurement<<COMMA<<bpList[i].temp;
				 	tempPersist.info(prtemp.str().c_str());
					prtemp.str(CLEAN);
					run(bpList[i].timeOfMeasurement,bpList[i].temp,temp.str().c_str());
					temp.str(CLEAN);
			
				} 
				catch( DBException &e ) 
				{
					temp <<e.what();
					tempInfo.notice(temp.str());
					temp.str(CLEAN);
				}
				
			}


		}
		status = bpReader->return_loan(bpList, infoSeq);
        	checkStatus(status, "return_loan");
		
		 
    	}
	tempInfo.notice("Temperature Persist Subscriber Ends "+deviceid);
        /* We're done.  Delete everything */
        simpledds->deleteReader(content_reader);
        delete simpledds;
        return 0;

}

