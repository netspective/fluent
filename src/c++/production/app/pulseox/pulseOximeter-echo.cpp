#include <iostream>
#include "SimpleDDS.h"
#include <dds/dds.hpp>
#include "ccpp_pulseox.h"

// -- BOOST Include
#include <boost/program_options.hpp>

// -- DDS Include
#include <dds/runtime.hpp>
#include <dds/topic.hpp>
#include <dds/reader.hpp>
#include <dds/traits.hpp>
using namespace DDS;
using namespace std;
namespace po = boost::program_options;
using namespace com::netspective::medigy;
std::stringstream temp;
std::string domainid,deviceid;

bool parse_args(int argc, char* argv[])
{
  po::options_description desc("Available options for <Pulse Oximeter Echo> are");
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
	PulseOximeterTypeSupport_var typesupport;
    	DataReader_ptr reader;
    	PulseOximeterDataReader_var bpReader;
    	ReturnCode_t status;
	int i=0;
        DDS::TopicQos tQos;
        tQos.durability.kind=VOLATILE_DURABILITY_QOS;
        tQos.reliability.kind=BEST_EFFORT_RELIABILITY_QOS;
        tQos.history.depth=10;
        tQos.durability_service.history_kind = KEEP_LAST_HISTORY_QOS;
        tQos.durability_service.history_depth= 1024;
        simpledds = new SimpleDDS(tQos);
	typesupport = new PulseOximeterTypeSupport();
    	reader = simpledds->subscribe(typesupport);
    	bpReader = PulseOximeterDataReader::_narrow(reader);
   	PulseOximeterSeq  bpList;
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
				std::cout <<"#######################################################\n";
				std::cout <<"\nMeasured Time : " << bpList[i].timeOfMeasurement;
				std::cout <<"\nSPO2 :"<< bpList[i].SPO2;
				std::cout <<"\nPulseRatePerMinute :"<< bpList[i].pulseRatePerMinute;
				std::cout <<"\n#####################################################\n";
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
