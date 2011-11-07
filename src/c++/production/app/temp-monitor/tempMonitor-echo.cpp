#include <iostream>
#include <iostream>
#include <dds/dds.hpp>
#include "ccpp_tempmonitor.h"
// -- BOOST Include
#include <boost/program_options.hpp>
// -- DDS Include
#include <dds/runtime.hpp>
#include <dds/topic.hpp>
#include <dds/reader.hpp>
#include <dds/traits.hpp>

std::stringstream temp;
std::string domainid,deviceid;

using namespace std;
namespace po = boost::program_options;

REGISTER_TOPIC_TRAITS(com::netspective::medigy::TempMonitor)


bool parse_args(int argc, char* argv[])
{
  po::options_description desc("Available options for <Temperature Monitor Echo> are");
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
	string key ("{");
        size_t start,end;
        string key1 ("}");
        start=deviceid.rfind(key);
        end=deviceid.rfind(key1);
        deviceid = deviceid.substr(0,start)+deviceid.substr(start+1,end-start-1);
        cout<<"\n"<<deviceid;
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

	std::string partition = "temp";
	dds::Runtime runtime(partition);
	dds::Duration cleanup_delay = {3600, 0};
	dds::TopicQos tQos;
	tQos.set_persistent();
	tQos.set_reliable();
	tQos.set_keep_last(10);
	tQos.set_durability_service(cleanup_delay,DDS::KEEP_LAST_HISTORY_QOS,1024,8192,4196,8192);
	dds::Topic<com::netspective::medigy::TempMonitor> topic(deviceid, tQos);
	dds::DataReaderQos drQos(tQos);
	dds::DataReader<com::netspective::medigy::TempMonitor> dr(topic, drQos);
	com::netspective::medigy::TempMonitorSeq data;
	DDS::SampleInfoSeq info;
	dds::Duration timeout = {60, 0};
	dr.wait_for_historical_data(timeout);
	while (true) 
	{
		dr.take(data, info);
		for (uint32_t i = 0; i < data.length(); ++i) 
		{
			std::cout <<"#######################################################\n";
			std::cout <<"\nMeasured Time : " << data[i].timeOfMeasurement;
			std::cout <<"\nTemp :"<< data[i].temp;
			std::cout <<"\n#####################################################\n";
			dr.return_loan(data, info);
		}
		sleep(1);
	}
	return 0;
}
