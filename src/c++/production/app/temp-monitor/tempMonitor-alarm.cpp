#include <iostream>
#include "SimpleDDS.h"
#include <dds/dds.hpp>
#include "ccpp_tempmonitor.h"
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

using namespace DDS;
using namespace std;
namespace po = boost::program_options;
using namespace com::netspective::medigy;
stringstream temp,prtemp;
string domainid,deviceid,loginfo,logdata,logconfpath;
int templow,temphigh,avgtime;



int main(int argc, char* argv[]) 
{
	 long timestart,timeend=0,avg=0;
	 int nrecord=0;
	 avgtime=1;
	 templow=88;
	 temphigh=92;

	 if (!parse_args_temp_alarm(argc,argv,domainid,deviceid,loginfo,logdata,logconfpath,avgtime,templow,temphigh))
    	 return 1;

	 /*Importing log4cpp configuration and Creating category*/
         log4cpp::Category &log_root = log4cpp::Category::getRoot();
         log4cpp::Category &tempInfo = log4cpp::Category::getInstance( std::string(loginfo));
         log4cpp::Category &tempAlarm = log4cpp::Category::getInstance( std::string(logdata));
         log4cpp::PropertyConfigurator::configure(logconfpath);
         tempInfo.notice(" Temperature Monitor Alarm Subscriber Started");

 	 /*Initializing SimpleDDS library*/
	 SimpleDDS *simpledds;
	 TempMonitorTypeSupport_var typesupport;
    	 DataReader_ptr content_reader;
    	 TempMonitorDataReader_var bpReader;
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
	 typesupport = new TempMonitorTypeSupport();

	 /*Creating content Filtered Subscriber*/
	 StringSeq sSeqExpr;
         sSeqExpr.length(0);
	 content_reader = simpledds->filteredSubscribe(typesupport, deviceid ,devid , deviceid,sSeqExpr);
    	 bpReader = TempMonitorDataReader::_narrow(content_reader);
   	 TempMonitorSeq  bpList;
     	 SampleInfoSeq     infoSeq;
	 tempInfo.notice("Temerature Monitor Alarm Subscriber for "+deviceid);
	 tempInfo.notice("Format: DEVICE_ID, START_TIME, END_TIME,TEMPERATURE");

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
	
				avg+=(long)bpList[i].temp;
				if(nrecord == 0)
	                        {
	                                timestart=(long)bpList[i].timeOfMeasurement;
	                        }
	                        else
	                        {
					timeend =(long) bpList[i].timeOfMeasurement;
	                                if((timeend - timestart) > (60*avgtime))
	                                {
						avg=avg/nrecord;
						if(avg < templow || avg > temphigh)
						{
							prtemp <<bpList[i].deviceID<<", "<<timestart<<", "<<timeend<<", "<<avg;
			 				tempAlarm.info(prtemp.str().c_str());
							prtemp.str("");
							
						}
						nrecord = -1;
	                               }
	                         }
				
			}


		}
		status = bpReader->return_loan(bpList, infoSeq);
        	checkStatus(status, "return_loan");
		nrecord++;
		
    	}
        /* We're done.  Delete everything */
	tempInfo.notice("Temperature Alarm Subscriber Ends "+deviceid);
        simpledds->deleteReader(content_reader);
        delete simpledds;
        return 0;
}
