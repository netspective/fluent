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
#include <dds/traits.hpp>
#include "Functions.h"

/*Log4cpp Library*/
#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/SimpleLayout.hh>


using namespace DDS;
using namespace std;
namespace po = boost::program_options;
using namespace com::netspective::medigy;

std::stringstream temp,prtemp;
string domainid,deviceid,logfile,logconfpath;

int main(int argc, char* argv[]) 
{

	 if (!parse_args_sub(argc, argv,domainid,deviceid,logfile,logconfpath))
    	 return 1;
	 
	 /*Importing log4cpp configuration and Creating category*/
         log4cpp::PropertyConfigurator::configure(logconfpath);
         log4cpp::Category& category = log4cpp::Category::getInstance(domainid+"."+deviceid);
         category.info(" Blood Pressure Subscriber Started");
	 
	 /*Initializing SimpleDDS library*/
	 SimpleDDS *simpledds;
	 BloodPressureTypeSupport_var typesupport;
    	 DataReader_ptr reader;
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
    	 reader = simpledds->subscribe(typesupport);
    	 bpReader = BloodPressureDataReader::_narrow(reader);
   	 BloodPressureSeq  bpList;
     	 SampleInfoSeq     infoSeq;
	 
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
			temp << bpList[i].deviceID;
			cout << bpList[i].deviceID <<"-------------"<<deviceid.c_str()<<"\n";
			if(strcmp(temp.str().c_str() , deviceid.c_str() ) == 0 )
			{
		        prtemp << "Device ID : " <<bpList[i].deviceID <<" Measured Time : "<<bpList[i].timeOfMeasurement<<" Systolic : "<< bpList[i].systolicPressure<< "Diastolic : "<<bpList[i].diastolicPressure<<" PulseRate : "<<bpList[i].pulseRatePerMinute;
			category.info(prtemp.str().c_str());
			prtemp.str("");
			//status = bpReader->return_loan(bpList, infoSeq);
       			//checkStatus(status, "return_loan");
			}
			status = bpReader->return_loan(bpList, infoSeq);
			checkStatus(status, "return_loan");
			temp.str("");
			sleep(1);
	  }
       
    	}

        /* We're done.  Delete everything */
	category.info("Blood Pressure Subscriber Ends");	
        simpledds->deleteReader(reader);
        delete simpledds;
        return 0;


}
