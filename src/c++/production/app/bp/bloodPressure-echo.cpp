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

std::stringstream temp;

REGISTER_TOPIC_TRAITS(com::netspective::medigy::BloodPressure)

int main(int argc, char* argv[]) {
  

  std::string partition = "blood";

  dds::Runtime runtime(partition);
  dds::Duration cleanup_delay = {3600, 0};

  dds::TopicQos tQos;
  tQos.set_persistent();
  tQos.set_reliable();
  tQos.set_keep_last(10);
  tQos.set_durability_service(cleanup_delay,
			      DDS::KEEP_LAST_HISTORY_QOS,
			      1024,
			      8192,
			      4196,
			      8192);

  dds::Topic<com::netspective::medigy::BloodPressure> topic("BloodPressure", tQos);


  dds::DataReaderQos drQos(tQos);
  dds::DataReader<com::netspective::medigy::BloodPressure> dr(topic, drQos);

  com::netspective::medigy::BloodPressureSeq data;
  DDS::SampleInfoSeq info;

  dds::Duration timeout = {60, 0};
  dr.wait_for_historical_data(timeout);

  while (true) {
    dr.take(data, info);
    std::cout << "Total Data Length :"<< data.length();
    std::cout << "\n";
    for (uint32_t i = 0; i < data.length(); ++i) {
	std::cout <<"#######################################################\n";
	std::cout <<"\nMeasured Time : " << data[i].timeOfMeasurement;
	std::cout <<"\nSystolicPressure :"<< data[i].systolicPressure;
	std::cout <<"\nDiastolicPressure :"<< data[i].diastolicPressure;
	std::cout <<"\nPulseRatePerMinute :"<< data[i].pulseRatePerMinute;
	std::cout <<"\n#####################################################\n";
	dr.return_loan(data, info);
    }
//    dr.return_loan(data, info);
    sleep(1);
  }
  return 0;
}
