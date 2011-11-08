#include <iostream>
#include <iostream>
#include <dds/dds.hpp>
#include "ccpp_pulseox.h"

// -- BOOST Include
#include <boost/program_options.hpp>

// -- DDS Include
#include <dds/runtime.hpp>
#include <dds/topic.hpp>
#include <dds/reader.hpp>
#include <dds/traits.hpp>
using namespace std;
namespace po = boost::program_options;
std::stringstream temp;
std::string domainid,deviceid;
REGISTER_TOPIC_TRAITS(com::netspective::medigy::PulseOximeter)
int splow,sphigh;
bool parse_args(int argc, char* argv[])
{
  po::options_description desc("Available options for <bloodPressure-alarm> are");
  desc.add_options()
    ("help", "produce help message")
    ("domain", po::value<std::string>(), "Device Domain")
    ("device-id",po::value<std::string>(), "Device ID for identification")
    ("spo2-low", po::value<int>(), "SPO2 Low level Alarm Alarm Specification - default <88")
    ("spo2-high", po::value<int>(), "SPO2 High Level Alarm Specification - default >92")
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

  
    if (vm.count("spo2-low"))
      splow = vm["spo2-low"].as<int>();
    if (vm.count("spo2-high"))
      sphigh = vm["spo2-high"].as<int>();
	
    
    }
  
  catch (...) {
    std::cout << desc << "\n";
    return false;
  }
  return true;
} 


int main(int argc, char* argv[]) 
{
	splow=88;
	sphigh=92;
	if (!parse_args(argc, argv))
    	return 1;
	std::string partition = "pulseox";

	dds::Runtime runtime(partition);
	dds::Duration cleanup_delay = {3600, 0};

	dds::TopicQos tQos;
	tQos.set_persistent();
	tQos.set_reliable();
	tQos.set_keep_last(10);
	tQos.set_durability_service(cleanup_delay, DDS::KEEP_LAST_HISTORY_QOS, 1024, 8192, 4196, 8192);

	dds::Topic<com::netspective::medigy::PulseOximeter> topic(deviceid, tQos);

	dds::DataReaderQos drQos(tQos);
	dds::DataReader<com::netspective::medigy::PulseOximeter> dr(topic, drQos);

	com::netspective::medigy::PulseOximeterSeq data;
	DDS::SampleInfoSeq info;

	dds::Duration timeout = {60, 0};
	dr.wait_for_historical_data(timeout);

	while (true) {
		dr.take(data, info);
		for (uint32_t i = 0; i < data.length(); ++i) {
			if (data[i].SPO2 <  splow || data[i].SPO2 > sphigh)
			{
				std::cout << "\n SPO2 alarm : " << data[i].SPO2;
			}
			dr.return_loan(data, info);
		}
		sleep(1);
	}
	return 0;
}
