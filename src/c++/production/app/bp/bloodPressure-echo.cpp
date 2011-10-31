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
std::string domainid,deviceid;

REGISTER_TOPIC_TRAITS(com::netspective::medigy::BloodPressure)

bool parse_args(int argc, char* argv[])
{
  po::options_description desc("Available options for <bloodPressure-echo> are");
  desc.add_options()
    ("help", "produce help message")
    ("Domain", po::value<std::string>(), "Device Domain")
    ("DeviceID",po::value<std::string>(), "Device ID for identification")
    ;

  try {
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help") || argc == 1) {
      std::cout << desc << "\n";
      return false;
    }

    if (vm.count("Domain"))
      domainid = vm["Domain"].as<std::string>();
	
    if (vm.count("DeviceID"))
      deviceid = vm["DeviceID"].as<std::string>();
	   

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
	//std::string tmp=data[0].deviceID;
	
	while (true) {
		dr.take(data, info);
		for (uint32_t i = 0; i < data.length(); ++i) {
			temp << data[i].deviceID;
			//std::cout << data[i].deviceID<<"\n";
			if(strcmp(temp.str().c_str() , deviceid.c_str() ) == 0 )
			{
			std::cout <<"#######################################################\n";
			std::cout <<"\nDevice ID : " << data[i].deviceID;
			std::cout <<"\nMeasured Time : " << data[i].timeOfMeasurement;
			std::cout <<"\nSystolicPressure :"<< data[i].systolicPressure;
			std::cout <<"\nDiastolicPressure :"<< data[i].diastolicPressure;
			std::cout <<"\nPulseRatePerMinute :"<< data[i].pulseRatePerMinute;
			std::cout <<"\n#####################################################\n";
			dr.return_loan(data, info);
			temp.str("");
			}
		}
		sleep(1);
	}
	return 0;
}
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
std::string domainid,deviceid;

REGISTER_TOPIC_TRAITS(com::netspective::medigy::BloodPressure)

bool parse_args(int argc, char* argv[])
{
  po::options_description desc("Available options for <bloodPressure-echo> are");
  desc.add_options()
    ("help", "produce help message")
    ("Domain", po::value<std::string>(), "Device Domain")
    ("DeviceID",po::value<std::string>(), "Device ID for identification")
    ;

  try {
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help") || argc == 1) {
      std::cout << desc << "\n";
      return false;
    }

    if (vm.count("Domain"))
      domainid = vm["Domain"].as<std::string>();
	
    if (vm.count("DeviceID"))
      deviceid = vm["DeviceID"].as<std::string>();
	   

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
	//std::string tmp=data[0].deviceID;
	
	while (true) {
		dr.take(data, info);
		for (uint32_t i = 0; i < data.length(); ++i) {
			temp << data[i].deviceID;
			//std::cout << data[i].deviceID<<"\n";
			if(strcmp(temp.str().c_str() , deviceid.c_str() ) == 0 )
			{
			std::cout <<"#######################################################\n";
			std::cout <<"\nDevice ID : " << data[i].deviceID;
			std::cout <<"\nMeasured Time : " << data[i].timeOfMeasurement;
			std::cout <<"\nSystolicPressure :"<< data[i].systolicPressure;
			std::cout <<"\nDiastolicPressure :"<< data[i].diastolicPressure;
			std::cout <<"\nPulseRatePerMinute :"<< data[i].pulseRatePerMinute;
			std::cout <<"\n#####################################################\n";
			dr.return_loan(data, info);
			temp.str("");
			}
		}
		sleep(1);
	}
	return 0;
}
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
std::string domainid,deviceid;

REGISTER_TOPIC_TRAITS(com::netspective::medigy::BloodPressure)

bool parse_args(int argc, char* argv[])
{
  po::options_description desc("Available options for <bloodPressure-echo> are");
  desc.add_options()
    ("help", "produce help message")
    ("Domain", po::value<std::string>(), "Device Domain")
    ("DeviceID",po::value<std::string>(), "Device ID for identification")
    ;

  try {
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help") || argc == 1) {
      std::cout << desc << "\n";
      return false;
    }

    if (vm.count("Domain"))
      domainid = vm["Domain"].as<std::string>();
	
    if (vm.count("DeviceID"))
      deviceid = vm["DeviceID"].as<std::string>();
	   

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
	//std::string tmp=data[0].deviceID;
	
	while (true) {
		dr.take(data, info);
		for (uint32_t i = 0; i < data.length(); ++i) {
			temp << data[i].deviceID;
			//std::cout << data[i].deviceID<<"\n";
			if(strcmp(temp.str().c_str() , deviceid.c_str() ) == 0 )
			{
			std::cout <<"#######################################################\n";
			std::cout <<"\nDevice ID : " << data[i].deviceID;
			std::cout <<"\nMeasured Time : " << data[i].timeOfMeasurement;
			std::cout <<"\nSystolicPressure :"<< data[i].systolicPressure;
			std::cout <<"\nDiastolicPressure :"<< data[i].diastolicPressure;
			std::cout <<"\nPulseRatePerMinute :"<< data[i].pulseRatePerMinute;
			std::cout <<"\n#####################################################\n";
			dr.return_loan(data, info);
			temp.str("");
			}
		}
		sleep(1);
	}
	return 0;
}
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
std::string domainid,deviceid;

REGISTER_TOPIC_TRAITS(com::netspective::medigy::BloodPressure)

bool parse_args(int argc, char* argv[])
{
  po::options_description desc("Available options for <bloodPressure-echo> are");
  desc.add_options()
    ("help", "produce help message")
    ("Domain", po::value<std::string>(), "Device Domain")
    ("DeviceID",po::value<std::string>(), "Device ID for identification")
    ;

  try {
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help") || argc == 1) {
      std::cout << desc << "\n";
      return false;
    }

    if (vm.count("Domain"))
      domainid = vm["Domain"].as<std::string>();
	
    if (vm.count("DeviceID"))
      deviceid = vm["DeviceID"].as<std::string>();
	   

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
	//std::string tmp=data[0].deviceID;
	
	while (true) {
		dr.take(data, info);
		for (uint32_t i = 0; i < data.length(); ++i) {
			temp << data[i].deviceID;
			//std::cout << data[i].deviceID<<"\n";
			if(strcmp(temp.str().c_str() , deviceid.c_str() ) == 0 )
			{
			std::cout <<"#######################################################\n";
			std::cout <<"\nDevice ID : " << data[i].deviceID;
			std::cout <<"\nMeasured Time : " << data[i].timeOfMeasurement;
			std::cout <<"\nSystolicPressure :"<< data[i].systolicPressure;
			std::cout <<"\nDiastolicPressure :"<< data[i].diastolicPressure;
			std::cout <<"\nPulseRatePerMinute :"<< data[i].pulseRatePerMinute;
			std::cout <<"\n#####################################################\n";
			dr.return_loan(data, info);
			temp.str("");
			}
		}
		sleep(1);
	}
	return 0;
}

