#include <iostream>
#include <iostream>
#include <dds/dds.hpp>
#include "ccpp_bp.h"

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
int sysmin,sysmax,pulsemin,pulsemax,dismin,dismax;
std::string domainid,deviceid;

REGISTER_TOPIC_TRAITS(com::netspective::medigy::BloodPressure)

bool parse_args(int argc, char* argv[])
{
  po::options_description desc("Available options for <bloodPressure-alarm> are");
  desc.add_options()
    ("help", "produce help message")
    ("domain", po::value<std::string>(), "Device Domain")
    ("device-id",po::value<std::string>(), "Device ID for identification")
    ("systolic-low", po::value<int>(), "Systolic Low Pressure Alarm Specification - default <90")
    ("systolic-high", po::value<int>(), "Systolic High Pressure Alarm Specification - default >140")
    ("diatolic-low", po::value<int>(), "Diatolic Low Pressure Alarm Specification - default <60")
    ("diatolic-high", po::value<int>(), "Diatolic High Pressure Alarm Specification - default >90")
    ("pulse-rate-low", po::value<int>(), "Pulse Low Rate Alarm Specification - default <60")
    ("pulse-rate-high", po::value<int>(), "Pulse High Rate Alarm Specification - default >90")
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

  
    if (vm.count("systolic-low"))
      sysmin = vm["systolic-low"].as<int>();
    if (vm.count("systolic-high"))
      sysmax = vm["systolic-high"].as<int>();
	
    if (vm.count("diatolic-low"))
      dismin = vm["diatolic-low"].as<int>();
    if (vm.count("diatolic-high"))
      dismin = vm["diatolic-high"].as<int>();

    if (vm.count("pulse-rate-low"))
      pulsemin = vm["pulse-rate-low"].as<int>();
    if (vm.count("pulse-rate-high"))
      pulsemax = vm["pulse-rate-high"].as<int>();

    }
  
  catch (...) {
    std::cout << desc << "\n";
    return false;
  }
  return true;
}  

int main(int argc, char* argv[]) 
{
	sysmin = 90;
	sysmax = 140;
	dismin = 60;
	dismax = 90;
	pulsemin = 60;
	pulsemax = 90;
	if (!parse_args(argc, argv))
    	return 1;
	std::string partition = "blood";
	dds::Runtime runtime(partition);
	dds::Duration cleanup_delay = {3600, 0};
	dds::TopicQos tQos;
	tQos.set_persistent();
	tQos.set_reliable();
	tQos.set_keep_last(10);
	tQos.set_durability_service(cleanup_delay, DDS::KEEP_LAST_HISTORY_QOS, 1024, 8192, 4196, 8192);

	dds::Topic<com::netspective::medigy::BloodPressure> topic(deviceid, tQos);

	dds::DataReaderQos drQos(tQos);
	dds::DataReader<com::netspective::medigy::BloodPressure> dr(topic, drQos);

	com::netspective::medigy::BloodPressureSeq data;
	DDS::SampleInfoSeq info;

	dds::Duration timeout = {60, 0};
	dr.wait_for_historical_data(timeout);

	while (true) {
		dr.take(data, info);
		for (uint32_t i = 0; i < data.length(); ++i) {
			if (data[i].systolicPressure < sysmin || data[i].systolicPressure > sysmax || data[i].diastolicPressure < dismin || data[i].diastolicPressure > dismax || data[i].pulseRatePerMinute < pulsemin || data[i].pulseRatePerMinute > pulsemax)
			{
				cout<<"\nTime : "<<data[i].timeOfMeasurement;
				cout<<"\nPatient Blood Pressure alarm ";
				cout<<"\nSystolic: "<< data[i].systolicPressure;
				cout<<"\nDiastolic: "<<data[i].diastolicPressure;
				cout<<"\nPULSE: "<<data[i].pulseRatePerMinute;
			}
			dr.return_loan(data, info);
		}
		sleep(1);
	}
	return 0;
}
