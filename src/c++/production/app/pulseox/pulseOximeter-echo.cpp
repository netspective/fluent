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

std::stringstream temp;

REGISTER_TOPIC_TRAITS(com::netspective::medigy::PulseOximeter)

int main(int argc, char* argv[]) 
{

	std::string partition = "pulseox";

	dds::Runtime runtime(partition);
	dds::Duration cleanup_delay = {3600, 0};

	dds::TopicQos tQos;
	tQos.set_persistent();
	tQos.set_reliable();
	tQos.set_keep_last(10);
	tQos.set_durability_service(cleanup_delay, DDS::KEEP_LAST_HISTORY_QOS, 1024, 8192, 4196, 8192);

	dds::Topic<com::netspective::medigy::PulseOximeter> topic("PulseOximeter", tQos);

	dds::DataReaderQos drQos(tQos);
	dds::DataReader<com::netspective::medigy::PulseOximeter> dr(topic, drQos);

	com::netspective::medigy::PulseOximeterSeq data;
	DDS::SampleInfoSeq info;

	dds::Duration timeout = {60, 0};
	dr.wait_for_historical_data(timeout);

	while (true) {
		dr.take(data, info);
		for (uint32_t i = 0; i < data.length(); ++i) {
			std::cout <<"#######################################################\n";
			std::cout <<"\nMeasured Time : " << data[i].timeOfMeasurement;
			std::cout <<"\nSPO2 :"<< data[i].SPO2;
			std::cout <<"\nPulseRatePerMinute :"<< data[i].pulseRatePerMinute;
			std::cout <<"\n#####################################################\n";
			dr.return_loan(data, info);
		}
		//    dr.return_loan(data, info);
		sleep(1);
	}
	return 0;
}
