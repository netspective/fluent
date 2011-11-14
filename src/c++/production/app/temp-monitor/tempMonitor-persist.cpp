#include <iostream>
#include "SimpleDDS.h"
#include <dds/dds.hpp>
#include "ccpp_tempmonitor.h"
//Visolve Added
#include <mongo/mongo/client/dbclient.h>
// -- BOOST Include
#include <boost/program_options.hpp>
// -- DDS Include
#include <dds/runtime.hpp>
#include <dds/topic.hpp>
#include <dds/reader.hpp>
#include <dds/traits.hpp>
using namespace mongo;
using namespace DDS;
using namespace std;
namespace po = boost::program_options;
using namespace com::netspective::medigy;
std::stringstream temp;
std::string domainid,deviceid;

void run(long time,short utemp) 
{
	DBClientConnection c;
	c.connect("localhost");
	BSONObj p = BSONObjBuilder().append("TimeStamp",(int)time).append("Temperature",utemp).obj();
	c.insert("EMR.patients", p);
}

bool parse_args(int argc, char* argv[])
{
  po::options_description desc("Available options for <tempMonitor-persist> are");
  desc.add_options()
    ("help", "produce help message")
    ("domain", po::value<std::string>(), "Device Domain")
    ("device-id",po::value<std::string>(), "Device ID for identification")
    ;

  try {
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help") || argc == 1) {
      std::cout << desc << "\n";
      return false;
    }

    if (vm.count("domain"))
      domainid = vm["domain"].as<std::string>();
	
    if (vm.count("device-id"))
	{
      deviceid = vm["device-id"].as<std::string>();
	//string key ("{");
        //size_t start,end;
        //string key1 ("}");
        //start=deviceid.rfind(key);
        //end=deviceid.rfind(key1);
        //deviceid = deviceid.substr(0,start)+deviceid.substr(start+1,end-start-1);
        //cout<<"\n"<<deviceid;
	}
	   

    }
  
  catch (...) {
    std::cout << desc << "\n";
    return false;
  }
  return true;
} 

int main(int argc, char* argv[]) 
{
	 if (!parse_args(argc, argv))
    	 return 1;
	 SimpleDDS *simpledds;
	 TempMonitorTypeSupport_var typesupport;
    	 DataReader_ptr reader;
    	 TempMonitorDataReader_var bpReader;
    	 ReturnCode_t status;
	 int i=0;
         DDS::TopicQos tQos;
         tQos.durability.kind=VOLATILE_DURABILITY_QOS;
         tQos.reliability.kind=BEST_EFFORT_RELIABILITY_QOS;
         tQos.history.depth=10;
         tQos.durability_service.history_kind = KEEP_LAST_HISTORY_QOS;
         tQos.durability_service.history_depth= 1024;
         simpledds = new SimpleDDS(tQos);
	 typesupport = new TempMonitorTypeSupport();
    	 reader = simpledds->subscribe(typesupport);
    	 bpReader = TempMonitorDataReader::_narrow(reader);
   	 TempMonitorSeq  bpList;
     	 SampleInfoSeq     infoSeq;
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
			if(strcmp(temp.str().c_str() , deviceid.c_str() ) == 0 )
			{
				try 
				{
					run(bpList[i].timeOfMeasurement,bpList[i].temp);
					std::cout << "connected ok" << endl;
				} 
				catch( DBException &e ) 
				{
					std::cout << "caught " << e.what() << endl;
				}
				status = bpReader->return_loan(bpList, infoSeq);
        			checkStatus(status, "return_loan");
			}
			temp.str("");
			sleep(1);
		}
		
		 
    	}

        /* We're done.  Delete everything */
        simpledds->deleteReader(reader);
        delete simpledds;
        return 0;

}

