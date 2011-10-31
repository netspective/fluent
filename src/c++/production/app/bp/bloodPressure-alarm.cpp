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
namespace po = boost::program_options;
std::stringstream temp;
int sysalarm,pulsealarm,disalarm;

REGISTER_TOPIC_TRAITS(com::netspective::medigy::BloodPressure)

bool parse_args(int argc, char* argv[])
{
  po::options_description desc("Available options for <bloodPressure-alarm> are");
  desc.add_options()
    ("help", "produce help message")
    ("SystolicAlarm", po::value<int>(), "Systolic Pressure Alarm Specification")
    ("DiatolicAlarm", po::value<int>(), "Diatolic Pressure Alarm Specification")
    ("PulseRateAlarm", po::value<int>(), "Pulse Rate Alarm Specification")
	
    ;

  try {
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help") || argc == 1) {
      std::cout << desc << "\n";
      return false;
    }

    if (vm.count("SystolicAlarm"))
      sysalarm = vm["SystolicAlarm"].as<int>();
	
    if (vm.count("DiatolicAlarm"))
      disalarm = vm["DiatolicAlarm"].as<int>();
	

    if (vm.count("PulseRateAlarm"))
      pulsealarm = vm["PulseRateAlarm"].as<int>();

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

	std::string partition = "blood";

	dds::Runtime runtime(partition);
	dds::Duration cleanup_delay = {3600, 0};

	dds::TopicQos tQos;
	tQos.set_persistent();
	tQos.set_reliable();
	tQos.set_keep_last(10);
	tQos.set_durability_service(cleanup_delay, DDS::KEEP_LAST_HISTORY_QOS, 1024, 8192, 4196, 8192);

	dds::Topic<com::netspective::medigy::BloodPressure> topic("BloodPressure", tQos);

	dds::DataReaderQos drQos(tQos);
	dds::DataReader<com::netspective::medigy::BloodPressure> dr(topic, drQos);

	com::netspective::medigy::BloodPressureSeq data;
	DDS::SampleInfoSeq info;

	dds::Duration timeout = {60, 0};
	dr.wait_for_historical_data(timeout);

	while (true) {
		dr.take(data, info);
		for (uint32_t i = 0; i < data.length(); ++i) {
			if ( data[i].systolicPressure > sysalarm || data[i].diastolicPressure < disalarm || data[i].pulseRatePerMinute < pulsealarm )
			{
				std::cout <<"\nTime : "<<data[i].timeOfMeasurement <<" Patient Blood Pressure alarm \nSystolic: "<< data[i].systolicPressure;
				std::cout <<" \nDiastolic: "<<data[i].diastolicPressure<<" \nPULSE: "<<data[i].pulseRatePerMinute;
			}
			dr.return_loan(data, info);
		}
		sleep(1);
	}
	return 0;
}

